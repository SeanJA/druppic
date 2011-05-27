<?php

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' &raquo; ', $breadcrumb) .'</div>';
  }
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  $output = '';

  if ($primary = menu_primary_local_tasks()) {
    $output .= "<ul class=\"tabs primary\">\n". $primary ."</ul>\n";
  }

  return $output;
}

/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $type = null) {
  static $node_type;
  if (isset($type)) $node_type = $type;

  if (!$content || $node_type == 'forum') {
    return '<div id="comments">'. $content . '</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2>'. $content .'</div>';
  }
}

/**
 * Define JournalCrunch variables
 */
variable_set('isFirstNoStickyNode', false);

function phptemplate_preprocess_page(&$vars) {
    $vars['template_files'][] = 'page-' . $vars['node']->type;
}

function journalcrunch_im_preprocess(&$vars, $hook) {
	if (arg(0) != 'admin' && $hook == "page") {
		// Get an array of all JavaScripts that have been added
		$javascript = drupal_add_js(NULL, NULL, 'header');

		// Remove the original jQuery library and Drupal's JS include
		unset($javascript['core']['misc/jquery.js']);
		unset($javascript['core']['misc/drupal.js']);

		// Add in our new jQuery library and Drupal's JS include
		// We do it this way to keep the includes in the same order
		$core = array(
			//Alternative jQuery
			drupal_get_path('theme', 'journalcrunch_im').'/js/jquery-1.5.2.min.js' => array(
				'cache' => TRUE,
				'defer' => FALSE,
			),
				'misc/drupal.js' => array(
				'cache' => TRUE,
				'defer' => FALSE,
			)
		);

		// Merge back into the array of core JavaScripts
		$javascript['core'] = array_merge($javascript['core'], $core);

		// Rerender the block of JavaScripts
		$vars['scripts'] = drupal_get_js(NULL, $javascript);
	}
}