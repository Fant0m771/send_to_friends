<?php

/**
 * @file
 * Helper Class
 */

/**
 * Helper class.
 */
class SendToFriends {

  /**
   * Sending mail.
   *
   * @param string $name
   *   Sender name.
   * @param array $param
   *   Array keys:
   *   - addresses: Email addresses mail will be send.
   *   - comment: sender comment (optional).
   *   - page_path: page path which will send to friends.
   * @param string $friend_name
   *   Friend name.
   * @param bool $js
   *   Variable which determines if javascrip is enabled.
   */
  public static function SendEmail($name = NULL, $param = array(), $friend_name = NULL, $js = FALSE) {
    global $base_url;
    $subject = variable_get("send_to_friends_subject", t('You need to discover this!'));
    $body    = variable_get('send_to_friends_body', t("Hi !friend_name,\n\n!name thinks that you may be interested in this web page:\n\n!url \n\n"));
    $body   .= !is_null($param['comment']) ? t("\rAnd leave comment: \n\n") . $param['comment'] : NULL;
    $headers = array('Content-Type' => 'text/html; charset=UTF-8; format=flowed');

    $url = self::GetCurrentUrl($param['page_path'], $base_url, variable_get("send_to_friends_get_url_setting", 'server'));
    foreach ($param['addresses'] as $mail) {
      $placeholders = array(
        '!friend_name' => $friend_name,
        '!url'         => $url,
        '!mail'        => $mail,
        '!name'        => $name,
      );

      $params = array(
        'subject'      => check_plain(t($subject, $placeholders)),
        'body'         => check_markup(t($body, $placeholders)),
        'headers'      => $headers,
      );

      $from_email = variable_get('site_mail', ini_get('sendmail_from'));
      drupal_mail('send_to_friends', "send_to_friends", $mail, language_default(), $params, $from_email);
      // Save log message to reports.
      $watchdog_message = t("!name has send the word to !mail on !url", $placeholders);
      watchdog('send_to_friends', $watchdog_message);
    }
  }

  /**
   * Add settings to modal form.
   */
  public static function ModalFormSettings() {
    static $added = FALSE;

    if ($added == FALSE) {
      $added = TRUE;
      $popup_class = variable_get('send_to_friends_theme', 'default') . " send_to_friends";

      // Include the CTools tools that we need.
      ctools_include('modal');
      ctools_include('ajax');
      ctools_modal_add_js();

      // Array with modal window settings.
      $window_style = array(
        'sent-to-friends-style' => array(
          'modalSize' => array(
            'type' => 'fixed',
            'width' => variable_get('send_to_friends_window_width', '600') + 'px',
            'height' => 'auto',
          ),
          'modalOptions' => array(
            'opacity' => (float) 0.6,
            'background-color' => '#000000',
          ),
          'modalTheme' => 'STFModalFormPopup',
          'class' => $popup_class,
          'closeText' => '',
          'closeImage' => theme('image', array(
              'path' => drupal_get_path('module', 'send_to_friends') . '/images/close.png',
              'alt' => t('Close window'),
              'title' => t('Close window'),
            )
          ),
          'loadingText' => t('Loading'),
          'animation' => variable_get('send_to_friends_animation', 'fadeIn'),
          'animationSpeed' => variable_get('send_to_friends_animation_speed', 'slow'),
        ),
      );
      // Include settings for modal window.
      drupal_add_js($window_style, 'setting');
      // Include template for modal window.
      ctools_add_js('send_to_friends_modal_popup', 'send_to_friends');
    }
  }

  /**
   * Get current page url.
   *
   * @param string $arg
   *   urls sent from form.
   * @param string $domain
   *   site domain address.
   * @param string $method
   *   How to get url address  using js or $_SERVER variable.
   *
   * @return mixed
   *   url that ready to send.
   */
  protected static function GetCurrentUrl($arg, $domain, $method) {
    // Default url will get using global array SERVER.
    if ($method == 'server') {
      $path = $_SERVER['HTTP_REFERER'];
    }
    elseif ($method == 'js') {
      $path = $domain . trim($arg);
    }
    return self::CleanPathString($path);
  }

  /**
   * Clean url string for security reason.
   *
   * @param string $input
   *   url that need to be striped from disallowed symbols.
   *
   * @return string
   *   url.
   */
  protected static function CleanPathString($input) {
    $disallowed = variable_get("send_to_friends_disallowed_symbols", "< > ' [ ^ ] + { } $ % ( ) ;");
    $disallowed = explode(" ", $disallowed);
    $input = str_replace($disallowed, "", $input);
    return $input;
  }
}
