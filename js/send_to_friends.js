/**
 * @file
 * JS code.
 */

(function ($) {
  /**
   * Set url where modal window was opened.
   */
  $.fn.send_to_friends_set_page_path = function() {
    $(this).attr('value', window.location.pathname + window.location.search + window.location.hash);
  }
})(jQuery);
