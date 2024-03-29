<?php

/**
 * @file
 */

/**
 * Implements hook_rules_action_info().
 */
function custom_pickup_rules_action_info() {
  return array(
    'custom_pickup_send_driver_alerts' => array(
      'label' => 'Send driver alerts',
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t('Node')),
      ),
      'group' => t('System'),
    ),
  );
}

/**
 * Rules action callback to login the user provided in the function parameter
 * @param $node
 * @param int $uid
 * @return null
 */
function custom_pickup_send_driver_alerts($node, $uid = 1) {
  $nid = $node->nid;
  if ($node->type == 'ride_booker') {
    $rid[0] = 4; // TODO this needs to be found dynamically of towncar, hybrid, or whatever.

    watchdog('Driver alerts', '$node = ' . print_r($node, TRUE), array(), WATCHDOG_DEBUG, 'node/' . $nid);


    foreach ($node->field_car_type[$node->language] as $term_wrapper) {
      $tid = $term_wrapper['tid'];
      watchdog('Driver alerts', 'tid = ' . $tid, array(), WATCHDOG_DEBUG, 'node/' . $nid);
    }

    if (!isset($tid)) {
      watchdog(
        'notifications',
        //'<pre>' . htmlentities(print_r($wrapper, TRUE)) . '</pre>' .
        '<pre>' . $tid . '</pre>',
        array(),
        WATCHDOG_ERROR,
        'node/' . $nid
      );
    }

    $roles = db_query("SELECT DISTINCT rid FROM {related_roles_role} WHERE tid = :tid ",
      array(
        ':tid' => $tid,
      ))->fetchAll();
    $x = count($roles);
    $i = 0;
    foreach ($roles as $role) {
      $rid[$i] = $role->rid;
      $i++;
    }
      // Temporary debug statement.

      watchdog(
        'notifications',
        //'<pre>' . htmlentities(print_r($wrapper, TRUE)) . '</pre>' .
        //'<pre>' . htmlentities(print_r($term_wrapper, TRUE)) . '</pre>',
        '<pre>' . htmlentities(print_r($rid, TRUE)) . '</pre>' .
        '<pre>' . htmlentities(print_r($node, TRUE)) . '</pre>',
        array(),
        WATCHDOG_INFO,
        NULL
      );
  }
  else {
    return NULL;
  }
  custom_pickup_driver_alert($nid, $uid, $rid);
  return NULL;
}

/**
 * Implements hook_default_rules_configuration_alter().
 */
function custom_pickup_default_rules_configuration_alter(&$configs) {
  if (!empty($configs['custom_pickup'])) {
    $configs['custom_pickup']->action(rules_action('custom_pickup_send_driver_alerts', array('node:select' => 'node-fetched:0')));
  }
}
