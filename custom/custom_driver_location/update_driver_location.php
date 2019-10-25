<?php
/**
 * @file
 * Updates the driver location.
 *
 */

// get lat and lon and die if not valid.
$error_message = 'Invalid value.';

$go_to_error = FALSE;
if (!isset($_POST['data'])) {
  die('$_POST[\'data\'] not set');
}

$data = json_decode($_POST['data']);

if (isset($_POST['nid'])) {
  $nid = intval($_POST['nid']);
}
else {
  $nid = 0;
}
if (isset($data->coords)){
  if(isset($data->coords->latitude)){
    $lat = $data->coords->latitude;
  }
  else {
    die($error_message . ' No $data->coords->latitude ');
  }
  if(isset($data->coords->longitude)){
    $lon = $data->coords->longitude;
  }
  else {
    die($error_message . ' No $data->longitude ');
  }
  if(isset($data->coords->speed)){
    $speed = $data->coords->speed;
  }
  else {
    $speed = 0;
  }
  if(isset($data->coords->altitude)){
    $altitude = $data->coords->altitude;
  }
  else {
    $altitude = 1;
  }
  if(isset($data->coords->altitudeAccuracy)){
    $altitude_accuracy = $data->coords->altitudeAccuracy;
  }
  else {
    $altitude_accuracy = 3;
  }
  if(isset($data->coords->accuracy)){
    $accuracy = $data->coords->accuracy;
  }
  else {
    die($error_message);
  }
  if (isset($data->timestamp)){
    $time_stamp = $data->timestamp / 1000;
  }
  else {
    die($error_message);
  }
}
else {
  $error_message .= print_r($data, true);
  die($error_message . ' No $data->coords '. "\n" .  $_POST['data'] );
}
if ($lat > 90 ) {
  die($error_message . ' $lat > 90 ');
}
if ($lat < -90 ) {
  die($error_message . ' $lat < -90 ');
}
if ($lon < -180 ) {
  die($error_message . ' $lon > -180 ');
}
if ($lon > 180 ) {
  die($error_message . ' $lon > 180 ');
}



// Set the working directory to your Drupal root.
chdir('../../../../..'); 

$_SERVER['HTTP_HOST'] = 'localhost'; // or the hostname of the drupal site you want to acces
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
define('DRUPAL_ROOT', getcwd());

//require the bootstrap include 
require_once DRUPAL_ROOT . '/includes/bootstrap.inc'; 

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

if (!isset($_POST['data'])) {
  watchdog('Update Driver location', '$_POST[\'data\'] is not set', array(), WATCHDOG_ERROR);
  die($error_message);
}
else {
  $data = json_decode($_POST['data']);
}

global $user;

$profile = profile2_load_by_user($user);
$pid = $profile['driver']->pid;
$driver = profile2_load($pid);
$driver->field_car_location['und'][0]['lon'] = $lon;
$driver->field_car_location['und'][0]['lat'] = $lat;
profile2_save($driver);

// check if we have a nid, and if the user has an rid of 9 (with customer)
// If so, we will need to call trip_logger_record($way_point) {

$with_customer = TRUE;
/*
foreach ($user->role as $role) {
  if ($role === 'with customer') {
    $with_customer = TRUE;
  }
}
//*/
if ($with_customer === TRUE && $nid > 0) {
  $way_point = new stdClass();
  $way_point->nid = $nid;
  $way_point->time_stamp = $time_stamp;
  $way_point->accuracy = $accuracy;
  $way_point->speed = $speed;
  $way_point->altitude = $altitude;
  $way_point->alt_accuracy = $altitude_accuracy;
  $way_point->lat = $lat;
  $way_point->lon = $lon;
  trip_logger_record($way_point);
}

print $driver->field_car_location['und'][0]['lat'];
print ":";
print $driver->field_car_location['und'][0]['lon'];
