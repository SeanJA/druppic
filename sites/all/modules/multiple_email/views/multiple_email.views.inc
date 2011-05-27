<?php
// $Id: multiple_email.views.inc,v 1.1.2.6 2011/01/11 04:35:44 shawndearmond Exp $

/**
 * Implementation of hook_views_handlers().
 */
function multiple_email_views_handlers() {
  $handlers = array();
  $handlers['info'] = array('path' => drupal_get_path('module', 'multiple_email') . '/views');
  $handlers['handlers'] = array();
  if (module_exists('email')) {
    $handlers['handlers']['multiple_email_handler_filter_current'] = array(
      'parent' => 'views_handler_filter_boolean_operator',
    );
  }

  return $handlers;
}

function multiple_email_views_data_alter(&$views_data) {
  foreach ($views_data as $table_key => $table) {
    foreach ($table as $key => $data) {
      if ($key != 'table' && $key != 'delta' && isset($data['field']['content_field_name'])) {
        if ($field = content_fields($data['field']['content_field_name'])) {
          if ($field['type'] == 'email') {
            $views_data[$table_key][$data['field']['content_field_name'] .'_match'] = array(
              'title' => $data['title short'] .' ('. $field['field_name'] .') - '. t('match'),
              'help' => t("Match current user's multiple e-mail addresses to CCK E-mail field."),
              'group' => t('Content'),
              'filter' => $data['filter'],
            );
            $views_data[$table_key][$field['field_name'] .'_match']['filter']['handler'] = 'multiple_email_handler_filter_current';
            //$views_data[$table_key][$field['field_name'] .'_match']['filter']['handler'] = 'content_handler_filter_many_to_one';
          }
        }
      }
    }
  }
}

/**
 * @file
 * multiple_email views integration file
 */
function multiple_email_views_data() {
  $data = array();

  // Basic table information.
  $data['multiple_email']['table']['group'] = t('Multiple E-mails');

  // Join to 'user' as a base table.
  $data['multiple_email']['table']['join'] = array(
    'user' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'node' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['multiple_email']['table']['base'] = array(
    'field' => 'eid',
    'title' => t('Multiple E-mail Addresses'),
    'help' => t('List each e-mail address as a separate row.'),
    'weight' => 10,
  );

  // ----------------------------------------------------------------
  // Fields

  // E-mail address
  $data['multiple_email']['email'] = array(
    'title' => t('Multiple E-mail Address'),
    'help' => t('The multiple e-mail addresses that the user may have.'),
    'field' => array(
      'handler' => 'views_handler_field_user_mail',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  if (module_exists('email')) {
    // Match E-mail addresses to CCK e-mail field
    $data['multiple_email']['email_match'] = array(
      'title' => t('Match E-mail Addresses'),
      'help' => t("Match current user's multiple e-mail addresses to CCK E-mail field."),
      'real_field' => 'email',
      'filter' => array(
        'handler' => 'multiple_email_views_handler_filter_email_current',
        'allow empty' => TRUE,
      ),
    );
  }






/**
  $data['multiple_email'] = array(
    'eid' => array(
      'title' => t('Multiple E-mail ID'),
      'help' => t('multiple_email.eid'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'uid' => array(
      'title' => t('User ID'),
      'help' => t('multiple_email.uid'),
      'field' => array(
        'handler' => 'views_handler_field_user',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_user_name',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_user_uid',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'email' => array(
      'title' => t('E-mail Address'),
      'help' => t('multiple_email.email'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'time_registered' => array(
      'title' => t('Time E-mail Registered'),
      'help' => t('multiple_email.time_registered'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_date',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
    ),
    'confirmed' => array(
      'title' => t('Time E-mail Confirmed'),
      'help' => t('multiple_email.confirmed'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'table' => array(
      'group' => t('Multiple E-mail'),
      'join' => array(
        'users' => array(
          'table' => 'multiple_email',
          'left_field' => 'uid',
          'field' => 'uid',
        ),
      ),

    ),
  );
 */

  return $data;
}

