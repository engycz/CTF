<?php
  session_start();

  if (!isset($_SESSION['csrf'])) {
    $_SESSION['csrf'] = bin2hex(random_bytes(32));
  }

  if (isset($_POST['login'])) {
    $csrf = $_POST['csrf'] ?? '';
    $username = trim($_POST['username'] ?? '');
    $password = $_POST['password'] ?? '';

    if (!hash_equals($_SESSION['csrf'], $csrf)) {
      $message = 'Your session has expired. Please try again.';
    }
    elseif (empty($username)) {
      $message = 'Username is required!';
    }
    elseif (empty($password)) {
      $message = 'Password is required!';
    }
    else {
      if ($username == getenv('APP_USERNAME') && $password == getenv('APP_PASSWORD')) {
        session_regenerate_id(true);

        $_SESSION['logged'] = true;
        setcookie('permissions', 'user', time() + 3000, '/');

        header('Location: system.php');
        exit;
      }
      else {
        $message = 'Invalid username or password.';
      }
    }
  }
?>
<!DOCTYPE html>
<html lang="en" data-bs-theme="dark">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <title>Quantum Keys TCC Repository</title>

  <link href="dist/css/bootstrap.min.css" rel="stylesheet">
  <link href="dist/css/bootstrap-icons.css" rel="stylesheet">
</head>

<body class="bg-body-tertiary">
  <main class="container min-vh-100 d-flex align-items-center justify-content-center py-4">
    <div class="col-12 col-sm-10 col-md-8 col-lg-5 col-xl-4">
      <section class="card shadow border-0" aria-labelledby="login-heading">
        <div class="card-body p-4 p-md-5">
          <header class="text-center mb-4">
            <i class="bi bi-database-lock d-block text-warning mb-3" style="font-size: 3.5rem" aria-hidden="true"></i>

            <h1 id="login-heading" class="h3 mb-2">
              Quantum Keys<br>TCC Repository
            </h1>

            <p class="text-body-secondary mb-0">
              Enter your credentials to continue.
            </p>
          </header>

          <?php if (!empty($message)): ?>
          <div class="alert alert-danger" role="alert" aria-live="polite">
            <i class="bi bi-exclamation-triangle-fill me-2" aria-hidden="true"></i>
            <?= $message ?>
          </div>
          <?php endif; ?>

          <form method="post">
            <input type="hidden" name="csrf" value="<?= htmlspecialchars($_SESSION['csrf'] ?? '') ?>">

            <div class="mb-3">
              <label for="username" class="form-label">
                Username
              </label>

              <div class="input-group">
                <span class="input-group-text" aria-hidden="true">
                  <i class="bi bi-person"></i>
                </span>

                <input type="text" name="username" id="username" class="form-control" autocomplete="username" autocapitalize="none" spellcheck="false" required autofocus>
              </div>
            </div>

            <div class="mb-4">
              <label for="password" class="form-label">
                Password
              </label>

              <div class="input-group">
                <span class="input-group-text" aria-hidden="true">
                  <i class="bi bi-lock"></i>
                </span>

                <input type="password" name="password" id="password" class="form-control" autocomplete="current-password" required>
              </div>
            </div>

            <button type="submit" name="login" class="btn btn-warning btn-lg w-100">
              <i class="bi bi-box-arrow-in-right me-2" aria-hidden="true"></i>
              Sign in
            </button>
          </form>
        </div>
      </section>
    </div>
  </main>
</body>
</html>