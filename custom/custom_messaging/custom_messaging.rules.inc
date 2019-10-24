<?php

/**
 * @file
 */

/**
 * Implements hook_rules_action_info().
 */
function custom_messaging_rules_action_info() {
  return array(
    'custom_messaging_send_user_alerts' => array(
      'label' => 'Send user alerts',
      'parameter' => array(
        'user' => array('type' => 'user', 'label' => t('User')),
        'text' => array('type' => 'text', 'label' => t('Message to send')),
      ),
      'group' => t('System'),
    ),
  );
}

/**
 * Rules action callback to login the user provided in the function parameter
 */
function custom_messaging_send_user_alerts($uid=1, $message) {
  custom_messaging_user_alert($uid, $message);
}

/**
 * Implements hook_default_rules_configuration_alter()
 */
function custom_messaging_default_rules_configuration_alter(&$configs) {
  if (!empty($configs['custom_messaging'])) {
    $configs['custom_messaging']->action(rules_action('custom_messaging_send_user_alerts', array('user:select' => 'user-fetched:0', 'text:select' => 'text-fetched:0')));
  }
}
