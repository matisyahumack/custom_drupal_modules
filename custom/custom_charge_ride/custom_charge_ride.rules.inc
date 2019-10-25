<?php

/**
 * @file
 * The rules and the functions that the rules need to add pricing.
 */

/**
 * Implements hook_rules_action_info().
 */
function custom_charge_ride_rules_action_info() {
  return array(
    'custom_charge_ride_add_products' => array(
      'label' => 'Add time and distance',
      'parameter' => array(
        'order' => array('type' => 'commerce_order', 'label' => t('Order')),
        'node' => array('type' => 'node', 'label' => t('Ride')),
        'distance' => array('type' => 'commerce_product', 'label' => t('Distance SKU')),
        'time' => array('type' => 'commerce_product', 'label' => t('Time SKU')),
      ),
      'group' => t('Commerce'),
    ),
  );
}

/**
 * Rules action callback to add the correct quantity of products to the order.
 */
function custom_charge_ride_add_products($order, $node, $distance, $time) {
  $uid = $order->uid;
  $nid = $node->nid;
  $total_time = custom_charge_ride_get_time($nid);
  //if ($total_time < 1) {$total_time = 1;}
  $time_product = commerce_product_load_by_sku($time->sku);
  commerce_cart_product_add_by_id($time_product->product_id, $total_time, FALSE, $uid);
  $total_distance = custom_charge_ride_get_distance($nid);
  // if ($total_distance < 1) {$total_distance = 1;}
  $distance_product = commerce_product_load_by_sku($distance->sku);
  commerce_cart_product_add_by_id($distance_product->product_id, $total_distance, FALSE, $uid);
  // add T $time_sku to $order
}

function custom_charge_ride_get_distance($nid) {
  $min_accuracy = $GLOBALS['conf']['trip_logger_accuracy'];
  $min_speed = $GLOBALS['conf']['trip_logger_speed'];
  $result = db_query('select sum(distance) distance from {trip_logger} where nid = :nid and accuracy < :accuracy and speed >= :speed', 
    array(
      ':nid' => $nid,
      ':accuracy' => $min_accuracy,
      ':speed' => $min_speed,
    ));
  $record = $result->fetchObject();
  watchdog('Ride billing', 'custom_charge_ride_get_distance result = ' . print_r($record, TRUE), array(), WATCHDOG_DEBUG, NULL);


  $distance = $record->distance;
  return number_format($distance, 2, '.', NULL);
}

function custom_charge_ride_get_time($nid) {
  $min_accuracy = $GLOBALS['conf']['trip_logger_accuracy'];
  $min_speed = $GLOBALS['conf']['trip_logger_speed'];

  $result = db_query('select sum(delta_time) time from {trip_logger} where nid = :nid and accuracy < :accuracy and speed >= :speed',
    array(
      ':nid' => $nid,
      ':accuracy' => $min_accuracy,
      ':speed' => $min_speed,
    ));
  $record = $result->fetchObject();
  if ($record->time === NULL) {
    $drive_time = 0;
  }
  else {
    $drive_time = (int) $record->time;
  }


  $result = db_query('select max(time_stamp) - min(time_stamp) total_time from {trip_logger} where nid = :nid', array(':nid' => $nid) );
  $record = $result->fetchObject();
  $total_time = (int) $record->total_time;
  $time = (string) (($total_time - $drive_time) / 60);
  watchdog('Billing', '$total_time = ' . (string) print_r($total_time, true) . ' $drive_time = ' . (string) $drive_time . ' $time = ' . $time , array(), WATCHDOG_DEBUG, NULL);
  return number_format($time, 1, '.', NULL);
}
/**
 * Implements hook_default_rules_configuration_alter().
 */
function custom_charge_ride_default_rules_configuration_alter(&$configs) {
  if (!empty($configs['custom_charge_ride'])) {
    $configs['custom_charge_ride']->action(rules_action('custom_charge_ride_add_products', array(
      'order:select' => 'order-fetched:0',
      'node:select' => 'node-fetched:0',
      'distance:select' => 'distance-fetched:0',
      'time:select' => 'time-fetched:0',
    )));
  }
}
