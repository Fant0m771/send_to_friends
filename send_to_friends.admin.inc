<?php

/**
 * @file
 * Admin settings.
 */

/**
 * Admin settings.
 */
function send_to_friends_general_settings() {
  $form = array();

  $form['send_to_friends_link_text'] = array(
    '#title' => t('Link text'),
    '#type'  => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('send_to_friends_link_text', t('Send this page to your friends')),
    '#description' => t('The link which will be shown on "Send To Friends" block.'),
  );

  $form['send_to_friends_subject'] = array(
    '#title' => t('Subject'),
    '#type'  => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('send_to_friends_subject', t('You need to discover this!')),
    '#description' => t('The subject of the e-mail.'),
  );

  $form['send_to_friends_body'] = array(
    '#title' => t('Body'),
    '#type'  => 'textarea',
    '#required' => TRUE,
    '#default_value' => variable_get('send_to_friends_body', t("Hi, !friend_name \n\n!name thinks that you may be interested in this web page: !url\n\n")),
    '#description' => t("This is the text of message. You can use the following placeholders to adjust it:<br /><em>!friend_name</em> : friend name<br /><em>!url</em> : web page from which the user submitted the form or the destination URL if set<br/><em>!name</em> : name who sent email"),
  );

  $form['send_to_friends_thanks_message'] = array(
    '#title' => t('Thank you message'),
    '#type'  => 'textarea',
    '#default_value' => variable_get('send_to_friends_thanks_message', t('Thank you.')),
  );

  $form['send_to_friends_thanks_message_in_popup'] = array(
    '#title' => '',
    '#type'  => 'radios',
    '#options' => array(
      'popup' => t('Show thank you message into pop up window.'),
      'status' => t('Show thank you message like status message.'),
    ),
    '#default_value' => variable_get('send_to_friends_thanks_message_in_popup', 'popup'),
  );

  $form['send_to_friends_how_many_addresses'] = array(
    '#type' => 'select',
    '#title' => t('How many email addresses user can add.'),
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5)),
    '#default_value' => variable_get('send_to_friends_how_many_addresses', 1),
  );

  $form['visibility_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['visibility_settings']['send_to_friends_show_friend_field'] = array(
    '#title' => t("Show 'Friend's name' field"),
    '#type'  => 'radios',
    '#options' => array(
      'show' => t('Yes'),
      'hide' => t('No'),
    ),
    '#default_value' => variable_get('send_to_friends_show_friend_field', 'show'),
  );

  $form['visibility_settings']['send_to_friends_show_comment'] = array(
    '#type' => 'radios',
    '#title' => t('Comment'),
    '#description' => t("Add possibility live comment into message."),
    '#options' => array(
      'show' => t('Yes'),
      'hide' => t('No'),
    ),
    '#default_value' => variable_get('send_to_friends_show_comment', 'show'),
  );

  $form['popup_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Popup settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['popup_settings']['send_to_friends_window_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Window width'),
    '#size' => 4,
    '#maxlength' => 4,
    '#default_value' => variable_get('send_to_friends_window_width', '600'),
    '#description' => t('Width in pixels (ex. 500). Value should be greater than 239'),
  );

  $form['popup_settings']['send_to_friends_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#options' => array(
      'default' => t('Default'),
      'simple' => t('Simple'),
    ),
    '#default_value' => variable_get('send_to_friends_theme', 'default'),
  );

  $form['popup_settings']['send_to_friends_animation'] = array(
    '#type' => 'select',
    '#title' => t('Animation'),
    '#options' => array(
      'show'      => t('Show'),
      'fadeIn'    => t('Fade in'),
      'slideDown' => t('Slide down'),
    ),
    '#default_value' => variable_get('send_to_friends_animation', 'fadeIn'),
  );

  $form['popup_settings']['send_to_friends_animation_speed'] = array(
    '#type' => 'select',
    '#title' => t('Animation Speed'),
    '#options' => array(
      'fast'    => t('Fast'),
      'slow'    => t('Slow'),
    ),
    '#default_value' => variable_get('send_to_friends_animation_speed', 'fast'),
  );

  $form['expert_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Expert settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['expert_settings']['send_to_friends_get_url_setting'] = array(
    '#type' => 'radios',
    '#options' => array(
      'server' => t('$_SERVER'),
      'js' => t('JS'),
    ),
    '#title' => t("What use to get page path"),
    '#description' => t("What need to use to get page path that will be sent to your friends: <br /><b> global variable SERVER</b> - recommended OR <b>javascript</b> (insecure)."),
    '#default_value' => variable_get('send_to_friends_get_url_setting', 'server'),
  );

  $form['expert_settings']['send_to_friends_disallowed_symbols'] = array(
    '#type' => 'textfield',
    '#title' => t('Disallowed symbols in URL'),
    '#default_value' => variable_get('send_to_friends_disallowed_symbols', "< > ' [ ^ ] + { } $ % ( ) & ;"),
    '#description' => t("A list of symbols that can not be used in URL which will be trimmed for security reason. Separate extensions with a space and do not include the leading dot or comma."),
  );
  $form['#validate'] = array('send_to_friends_general_settings_validate');
  return system_settings_form($form);
}

/**
 * Form validate.
 *
 * @param array $form
 *   form elements.
 * @param array $form_state
 *   form state elements.
 */
function send_to_friends_general_settings_validate($form, &$form_state) {
  // Is entered value for field count is number?
  if (!is_numeric($form_state['values']['send_to_friends_window_width'])) {
    form_set_error('send_to_friends_window_width', t('You must enter a numeric value only.'));
  }
  elseif ($form_state['values']['send_to_friends_window_width'] < 240) {
    form_set_error('send_to_friends_window_width', t('Value should be greater than 239.'));
  }
}
