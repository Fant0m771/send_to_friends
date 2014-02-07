<?php

/**
 * @file
 * Main form.
 */

/**
 * Build the form.
 *
 * @param array $form
 *   form elements.
 * @param array $form_state
 *   state form elements
 *
 * @return mixed
 *   form elements.
 */
function send_to_friends_form($form, &$form_state) {
  if (variable_get('send_to_friends_show_friend_field', 'show') == 'show') {
    $form['friend_name'] = array(
      '#type'   => 'textfield',
      '#title'  => t("Friend's Name:"),
      '#size'   => 40,
      '#attributes' => array(
        'placeholder' => array(
          t('Friend name'),
        ),
      ),
    );
  }

  $max_email_count = variable_get('send_to_friends_how_many_addresses', 1);
  $mail_description = $max_email_count > 1 ? t('Up to @count email addresses, separate extensions with a space.', array('@count' => $max_email_count)) : '';
  $form['to']["send-to-friends-mail"] = array(
    '#type' => 'textfield',
    '#title' => t("Friend's E-mail Address:"),
    '#required' => TRUE,
    '#description' => check_plain($mail_description),
    '#size' => 40,
    '#attributes' => array(
      'class' => array(
        'email',
      ),
      'placeholder' => array(
        t('Friends e-mail address'),
      ),
    ),
  );

  $form['sender_name'] = array(
    '#type'  => 'textfield',
    '#title' => t('Your Name:'),
    '#size'  => 40,
    '#required' => TRUE,
    '#attributes' => array(
      'placeholder' => array(
        t('Your name'),
      ),
    ),
  );

  if (variable_get('send_to_friends_show_comment', 'show') == 'show') {
    // Container for comment block.
    $form['comment_area'] = array(
      '#title'  => "",
      '#prefix' => '<div id="comment_area">',
      '#suffix' => '</div>',
      '#type'   => 'fieldset',
    );

    $form['add_comments'] = array(
      '#type' => 'checkbox',
      '#title' => '',
      '#suffix' => '<span class="suffix">Add comment.</span>',
      '#ajax' => array(
        'callback' => 'send_to_friends_add_comment_callback',
        'wrapper'  => 'comment_area',
        'effect' => 'fade',
      ),
    );

    if (!empty($form_state['values']['add_comments']) && $form_state['values']['add_comments']) {
      $form['comment_area']['comment_to_friends'] = array(
        '#type'  => 'textarea',
        '#title' => t('Your comment:'),
        '#resizable' => FALSE,
      );
    }
  }

  $form['page_path'] = array(
    '#type' => 'hidden',
    '#default_value' => $_SERVER['HTTP_REFERER'],
    '#attributes' => array(
      'class' => 'send_to_friends_page_path',
    ),
  );
  $form['#attributes'] = array('class' => 'send-to-friends-form');
  $form['#validate'] = array('send_to_friends_form_validate');
  $form['#submit'] = array('send_to_friends_form_submit');

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Send'),
  );
  return $form;
}

/**
 * Form validate.
 */
function send_to_friends_form_validate($form, &$form_state) {
  // Check if the user enter their name.
  if ($form_state['values']['sender_name'] == '') {
    form_set_error('sender_name', t('Please enter Your Name'));
  }

  $emails = explode(" ", trim($form_state['values']['send-to-friends-mail']));
  foreach ($emails as $email) {
    if (!valid_email_address($email)) {
      form_set_error('send-to-friends-mail', t('@mail address is not valid', array('@mail' => $email)));
    }
  }
}

/**
 * Form submit.
 */
function send_to_friends_form_submit($form, &$form_state) {
  $elements = array();
  if (array_key_exists('send-to-friends-mail', $form_state['values'])) {
    $emails = explode(" ", trim($form_state['values']['send-to-friends-mail']));
    foreach ($emails as $email) {
      $elements['addresses'][] = $email;
    }
  }
  // Page path.
  $elements['page_path'] = $form_state['values']['page_path'];
  // User comments.
  $elements['comment'] = isset($form_state['values']['comment_to_friends']) ? $form_state['values']['comment_to_friends'] : NULL;
  SendToFriends::SendEmail($form_state['values']['sender_name'], $elements, $form_state['values']['friend_name']);
  drupal_set_message(variable_get('send_to_friends_thanks_message', t('Thank you.')), 'status');
}

/**
 * Call back for checkbox "Add comment".
 *
 * @param array $form
 *   form elements.
 * @param array $form_state
 *   state form elements.
 *
 * @return mixed
 *   Piece of form.
 */
function send_to_friends_add_comment_callback($form, &$form_state) {
  return $form['comment_area'];
}
