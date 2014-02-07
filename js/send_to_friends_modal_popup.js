/**
 * @file
 *
 * Template for ctools pop up window.
 */

/**
 * Provide the HTML to create the modal dialog.
 */
Drupal.theme.prototype.STFModalFormPopup = function () {
  var html = '';

  html += '<div id="ctools-modal" class="popups-box">';
  html += '  <div class="ctools-modal-content modal-forms-modal-content ' + Drupal.CTools.Modal.currentSettings['class'] + '">';
  html += '    <div class="modal-top">';
  html += '      <a class="close" href="#">' + Drupal.CTools.Modal.currentSettings.closeImage + '</a>';
  html += '      <span id="modal-title" class="modal-title"> </span>';
  html += '    </div>';
  html += '    <div id="modal-content" class="modal-content"></div>';
  html += '  </div>';
  html += '</div>';

  return html;
}
