<?php
  session_start();

  if (!isset($_SESSION['logged']) || !isset($_COOKIE['permissions']) || !in_array($_COOKIE['permissions'], ['user', 'admin'])) {
    header('Location: index.php');
    exit;
  }

  if (isset($_GET['logout'])) {
    session_destroy();

    unset($_COOKIE['permissions']);
    setcookie('permissions', '', 1, '/');

    header('Location: index.php');
    exit;
  }

  if (isset($_GET['section'])) {
    $section = $_GET['section'];

    if ($section == 'repository') {
      if ($_COOKIE['permissions'] == 'admin') {
        $sectionFilename = 'repository';
      }
      else {
        http_response_code(403);
        exit('Access denied.');
      }
    }
    else if ($section == 'settings') {
      $sectionFilename = 'settings';
    }

    $sectionFilename = $_GET['section'];
  }
  else {
    $sectionFilename = 'homepage';
  }

  $sectionFilepath = __DIR__ . '/../includes/';
  $sectionFilename .= '.inc.php';

  require $sectionFilepath . $sectionFilename;
?>
<!doctype html>
<html lang="en" data-bs-theme="dark">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <title>Quantum Keys TCC Repository</title>

    <link href="dist/css/bootstrap.min.css" rel="stylesheet">
    <link href="dist/css/bootstrap-icons.css" rel="stylesheet">
  </head>
  <body class="bg-body-tertiary">
    <nav class="navbar navbar-expand-lg bg-dark border-bottom border-body fixed-top" data-bs-theme="dark" aria-label="Main navigation">
      <div class="container">
        <a href="system.php" class="navbar-brand d-flex align-items-center gap-2">
          <i class="bi bi-activity text-warning"></i>
          <span>Quantum Keys TCC Repository</span>
        </a>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#mainNavbar" aria-controls="mainNavbar" aria-expanded="false" aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="mainNavbar">
          <ul class="navbar-nav ms-auto align-items-lg-center">
            <li class="nav-item">
              <a class="nav-link active" aria-current="page" href="system.php">
                <i class="bi bi-house me-1" aria-hidden="true"></i>
                Home
              </a>
            </li>
            <li class="nav-item<?php if ($_COOKIE['permissions'] !== 'admin') echo ' visually-hidden';?>">
              <a class="nav-link" href="?section=repository">
                <i class="bi bi-database me-1" aria-hidden="true"></i>
                Repository
              </a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="?section=settings">
                <i class="bi bi-gear me-1" aria-hidden="true"></i>
                Settings
              </a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="?section=management">
                <i class="bi bi-terminal me-1" aria-hidden="true"></i>
                Management
              </a>
            </li>
            <li class="nav-item  ms-lg-2">
              <a class="btn btn-sm btn-outline-warning mt-2 mt-lg-0" href="?logout">
                <i class="bi bi-box-arrow-right me-1" aria-hidden="true"></i>
                Logout
              </a>
            </li>
          </ul>
        </div>
      </div>
    </nav>

    <div class="container py-4" style="margin-top: 64px;">
      <?php require $sectionFilepath . 'templates/' . $sectionFilename; ?>
    </div>

    <script src="dist/js/bootstrap.bundle.min.js"></script>
  </body>
</html>