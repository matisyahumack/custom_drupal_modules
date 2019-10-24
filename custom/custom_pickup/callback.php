<?php

/**
 * @file
 */

if (isset($_SERVER['PANTHEON_ENVIRONMENT']) &&
  $_SERVER['PANTHEON_ENVIRONMENT'] === 'dev'
) {
  $base_url = 'https://dev-rc.gotpantheon.com';
  $_SERVER['HTTP_HOST'] = 'dev.rc.gotpantheon.com';
  $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
}
else {
  if (isset($_SERVER['PANTHEON_ENVIRONMENT']) &&
    $_SERVER['PANTHEON_ENVIRONMENT'] === 'test'
  ) {
    $base_url = 'https://test-rc.gotpantheon.com';
    $_SERVER['HTTP_HOST'] = 'test-rc.gotpantheon.com';
    $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
  }
  else {
    if (isset($_SERVER['PANTHEON_ENVIRONMENT']) &&
      $_SERVER['PANTHEON_ENVIRONMENT'] === 'live'
    ) {
      $base_url = 'https://www.rideclever.com';
      $_SERVER['HTTP_HOST'] = 'www.rideclever.com';
      $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
    }
    else {
      $base_url = 'https://dev-rc.gotpantheon.com';
      $_SERVER['HTTP_HOST'] = 'dev-rc.gotpantheon.com';
      $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
    }
  }
}

chdir('../../../../..');
$current_drupal_root = getcwd();

define('DRUPAL_ROOT', $current_drupal_root);
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);


if ($_GET['key'] != 'vUKtuYvHt3yT8CDBVu3QHq6IB92MwL6w') {
  trigger_error('Bad key ', E_USER_ERROR);
}

$nid = intval($_GET['nid']);
$uid = intval($_GET['uid']);
$rid = array(intval($_GET['rid']),);

if ($nid > 1) { // garbage values of nid will commonly return 1 or 0 and the nid of the ride will always be greater than 1
  custom_pickup_driver_alert($nid, $uid, $rid);
  print "Success.\n";
}


