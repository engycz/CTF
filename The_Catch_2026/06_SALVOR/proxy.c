#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

#define BUFFER_SIZE 8192
#define LOGBUF_SIZE 256

#define LISTEN_PORT 3001
#define TARGET_HOST "127.0.0.1"
#define TARGET_PORT 3000
#define MAX_CHILDREN 100
#define CONFIG_FILE "proxy-filter.conf"

// #define DEBUG 1

static int running = 1;
static int child_count = 0;
char request[BUFFER_SIZE];
char response[BUFFER_SIZE];

typedef struct ip_node
{
    struct sockaddr_storage addr;
    struct ip_node *next;
} ip_node_t;

static ip_node_t *allowed = NULL;

#if defined(DEBUG) && DEBUG == 1
void hexdump(const void *data, size_t size)
{
    const unsigned char *b = data;
    for (size_t i = 0; i < size; i += 16)
    {
        printf("%08zx  ", i);
        for (size_t j = 0; j < 16; j++)
            printf(i + j < size ? "%02x " : "   ", b[i + j]), j == 7 && printf(" ");
        printf(" |");
        for (size_t j = 0; j < 16 && i + j < size; j++)
            printf("%c", isprint(b[i + j]) ? b[i + j] : '.');
        printf("|\n");
    }
}
#endif

void process_trace_request(int client_fd, char *buffer)
{
    char base_response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n";
    send(client_fd, base_response, strlen(base_response), 0);
#if defined(DEBUG) && DEBUG == 1
    printf("client_fd %d\n", client_fd);
    printf("request addr %p\n", request);
    printf("response addr %p\n", response);
    printf("buffer arg addr %p\n", buffer);
#endif

    int len = snprintf(response, BUFFER_SIZE, buffer);
    send(client_fd, response, len, 0);
    close(client_fd);
}

void process_proxy_request(int client_fd, char *buffer, ssize_t bytes_read)
{
    int target_fd;
    struct sockaddr_in target_addr;

    /* Connect to target */
    target_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (target_fd < 0)
    {
        perror("socket");
        close(client_fd);
        return;
    }

    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(TARGET_PORT);
    inet_pton(AF_INET, TARGET_HOST, &target_addr.sin_addr);

    if (connect(target_fd, (struct sockaddr *)&target_addr, sizeof(target_addr)) < 0)
    {
        perror("connect");
        const char *err = "HTTP/1.1 502 Bad Gateway\r\n\r\n";
        send(client_fd, err, strlen(err), 0);
        close(target_fd);
        close(client_fd);
        return;
    }

    /* Send request to target */
    struct timeval tv = {.tv_sec = 1, .tv_usec = 0};
    setsockopt(target_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    printf("[*] Forwarding %zd bytes to target\n", bytes_read);
    send(target_fd, buffer, bytes_read, 0);

    /* Forward response back */
    printf("[*] Forwarding response back to client\n");
    while ((bytes_read = recv(target_fd, response, BUFFER_SIZE, 0)) > 0)
    {
        send(client_fd, response, bytes_read, 0);
    }

    printf("[*] Closing connections\n");
    close(target_fd);
    close(client_fd);
}

void handle_client(int client_fd, char *client_ip)
{
    char logbuf[LOGBUF_SIZE];
    int logbuf_len = 0;
    int req_line_len = 0;
    time_t now;
    struct tm *timeinfo;
    ssize_t bytes_read;

    /* Read incoming request */
    bytes_read = recv(client_fd, request, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0)
    {
        close(client_fd);
        return;
    }
    request[bytes_read] = '\0';
#if defined(DEBUG) && DEBUG == 1
    hexdump(request, bytes_read);
#endif

    /* prepare logging */
    time(&now);
    timeinfo = localtime(&now);
    strftime(logbuf, sizeof(logbuf), "%Y-%m-%d %H:%M:%S ", timeinfo);
    logbuf_len = strlen(logbuf);
    logbuf_len += snprintf(logbuf + logbuf_len, sizeof(logbuf) - logbuf_len, "%s ", client_ip);
    req_line_len = memchr((void *)request, '\n', bytes_read) - (void *)request;
    memcpy(logbuf + logbuf_len, request, req_line_len);
    logbuf[LOGBUF_SIZE - 1] = '\0';

    /* handle request */
    if (strncmp(request, "TRACE ", 6) == 0)
    {
        printf("[+] Received %zd bytes, handling trace request\n", bytes_read);
        process_trace_request(client_fd, request);
    }
    else
    {
        printf("[+] Received %zd bytes, forwarding to %s:%d\n", bytes_read, TARGET_HOST, TARGET_PORT);
        process_proxy_request(client_fd, request, MIN(bytes_read, BUFFER_SIZE));
    }

    /* log request*/
    printf("%s\n", logbuf);
}

void signal_handler(int sig)
{
    (void)sig;
    running = 0;
}

static void reap_children(int sig)
{
    (void)sig;
    int saved = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
        child_count--;
    }
    errno = saved;
}

void setup_signal_handlers()
{
    // general handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, SIG_IGN);

    // child handlers
    struct sigaction sa;
    sa.sa_handler = reap_children;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; // restart accept(), ignore stopped kids
    sigaction(SIGCHLD, &sa, NULL);
}

bool ip_list_add(ip_node_t **head, const char *ip_str)
{
    ip_node_t *node = calloc(1, sizeof(ip_node_t));
    if (!node)
        return false;

    struct sockaddr_storage *sa = &node->addr;

    if (inet_pton(AF_INET6, ip_str, &((struct sockaddr_in6 *)sa)->sin6_addr) == 1)
        sa->ss_family = AF_INET6;
    else if (inet_pton(AF_INET, ip_str, &((struct sockaddr_in *)sa)->sin_addr) == 1)
        sa->ss_family = AF_INET;
    else
    {
        fprintf(stderr, "Invalid IP: %s\n", ip_str);
        free(node);
        return false;
    }

    node->next = *head;
    *head = node;
    return true;
}

bool is_client_allowed(const ip_node_t *head, const struct sockaddr_storage *client)
{
    for (const ip_node_t *n = head; n != NULL; n = n->next)
    {
        const struct sockaddr_storage *a = &n->addr;

        if (client->ss_family != a->ss_family)
            continue;

        if (a->ss_family == AF_INET6)
        {
            if (memcmp(&((struct sockaddr_in6 *)client)->sin6_addr,
                       &((struct sockaddr_in6 *)a)->sin6_addr,
                       sizeof(struct in6_addr)) == 0)
                return true;
        }
        else
        {
            if (((struct sockaddr_in *)client)->sin_addr.s_addr ==
                ((struct sockaddr_in *)a)->sin_addr.s_addr)
                return true;
        }
    }
    return false;
}

void ip_list_free(ip_node_t *head)
{
    while (head)
    {
        ip_node_t *tmp = head;
        head = head->next;
        free(tmp);
    }
}

bool ip_list_load_from_file(ip_node_t **head, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        fprintf(stderr, "Failed to open config file: %s (%s)\n", filename, strerror(errno));
        return false;
    }

    char line[INET6_ADDRSTRLEN + 64];
    int count = 0;

    while (fgets(line, sizeof(line), fp))
    {
        /* Skip comments and empty lines */
        char *p = line;
        while (*p && isspace((unsigned char)*p))
            p++;
        if (*p == '#' || *p == '\0')
            continue;

        /* Strip trailing whitespace/newline */
        char *end = p + strlen(p) - 1;
        while (end > p && isspace((unsigned char)*end))
            end--;
        *(end + 1) = '\0';

        if (ip_list_add(head, p))
            count++;
    }

    fclose(fp);
    printf("[*] Loaded %d IP(s) from %s\n", count, filename);
    return count > 0;
}

void sockaddr_to_str(const struct sockaddr_storage *addr, char *buf, socklen_t buf_len) {
    memset(buf, 0, buf_len);
    if (addr->ss_family == AF_INET6)
        inet_ntop(AF_INET6, &((struct sockaddr_in6 *)addr)->sin6_addr, buf, buf_len);
    else
        inet_ntop(AF_INET,  &((struct sockaddr_in  *)addr)->sin_addr,  buf, buf_len);
}

int main(int argc, char *argv[])
{
    int server_fd;
    struct sockaddr_in6 server_addr;
    int port = LISTEN_PORT;
    int opt = 1;
    int ipv6only = 0;

    int client_fd;
    struct sockaddr_storage client_addr;
    socklen_t client_len = sizeof(client_addr);
    char client_ip[INET6_ADDRSTRLEN];

    int max_children = MAX_CHILDREN;

    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IOLBF, 0);

    if (argc > 1)
        port = atoi(argv[1]);
    if (argc > 2)
        max_children = atoi(argv[2]);

    ip_list_add(&allowed, "127.0.0.1");
    ip_list_add(&allowed, "::1");
    ip_list_add(&allowed, "::ffff:127.0.0.1");
    ip_list_load_from_file(&allowed, CONFIG_FILE);

    setup_signal_handlers();

    server_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(server_fd, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6only, sizeof(ipv6only));
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 128) < 0)
    {
        perror("listen");
        return 1;
    }

    printf("TLS HTTP Proxy\n");
    printf("Listening on: [::]:%d (dual-stack)\n", port);
    printf("Forwards to:  %s:%d\n", TARGET_HOST, TARGET_PORT);

    while (running)
    {
        if (child_count >= max_children)
        {
            printf("[*] Max children limit reached\n");
            sleep(1);
            continue;
        }

        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0)
            continue;

        sockaddr_to_str(&client_addr, client_ip, sizeof(client_ip));
        printf("[*] Connection from %s\n", client_ip);
        if (allowed && !is_client_allowed(allowed, &client_addr))
        {
            printf("[*] DENIED: %s\n", client_ip);
            close(client_fd);
            continue;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            close(server_fd);
            // TODO: Add TLS wrap
            handle_client(client_fd, client_ip);
            _exit(0);
        }
        else if (pid > 0)
        {
            child_count++;
            close(client_fd);
        }
    }

    close(server_fd);
    ip_list_free(allowed);
    return 0;
}
