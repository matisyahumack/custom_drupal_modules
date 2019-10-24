<?php

/**
 * @file
 */

/**
 * Implements hook_rules_action_info().
 */
function custom_order_ride_rules_action_info() {
  return array(
    'custom_order_ride_set_driver' => array(
      'label' => 'Assign driver',
      'parameter' => array(
        'user' => array('type' => 'user', 'label' => t('User')),
        'node' => array('type' => 'node', 'label' => t('Ride')),
      ),
      'group' => t('System'),
    ),
    'custom_order_ride_unset_driver' => array(
      'label' => 'Unassign driver',
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t('Ride')),
      ),
      'group' => t('System'),
    ),
  );
}

/**
 * Rules action callback to login the user provided in the function parameter
 */
function custom_order_ride_set_driver($driver, $ride) {
  $node = node_load($ride->nid);
  $user = user_load($driver->uid);
  $uid = $user->uid;
  $node->field_car_driver['und'][0]['target_id'] = $uid;
  $node = node_submit($node);
  node_save($node);
}

/**
 * @param $ride
 */
function custom_order_ride_unset_driver($ride) {
  //$wrapper = entity;
  $node = node_load($ride->nid);
  unset($node->field_car_driver);
  $node = node_submit($node);
  node_save($node);
}

/**
 * Implements hook_default_rules_configuration_alter()
 */
function custom_order_ride_default_rules_configuration_alter(&$configs) {
  if (!empty($configs['custom_order_ride'])) {
    $configs['custom_order_ride']->action(rules_action('custom_order_ride_set_driver', array(
      'driver:select' => 'account-fetched:0',
      'ride:select' => 'node-fetched:0',
    )));
    $configs['custom_order_ride']->action(rules_action('custom_order_ride_unset_driver', array('ride:select' => 'node-fetched:0')));
  }
}
