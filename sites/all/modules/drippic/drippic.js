Drupal.behaviors.drippic = function () {
  // Hide the tags field on the node add form.
  // Then add hashtags to the tags field.
  jQuery('#edit-taxonomy-tags-1-wrapper').hide();
  jQuery('#edit-title').keyup(function(){
    var tweet = jQuery('#edit-title').val();
    var matches = tweet.match(/[#]+[A-Za-z0-9-_]+/g);
    var tags = '';
    for (i in matches) {
      tags = tags + matches[i].replace('#','') + ', ';
    }
    jQuery('#edit-taxonomy-tags-1').val(tags);
  });
}
