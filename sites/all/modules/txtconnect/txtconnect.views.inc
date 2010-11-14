<?php

/*
 * Reference
 * - http://nagarajan-drupal.blogspot.com/2009/01/creating-views-for-used-defined-tables.html
 * - http://drupal.org/node/305248
 */


/**
 * Implementation of hook_views_data()
 *  
 *  Setup the ability to choose Txt Connect fields in Views
 */
function txtconnect_views_data() {

  // Create Group Name
  $data['txtconnect_relationships']['table']['group']  = t('Txt Connect');

  	// Setup Join
  $data['txtconnect_relationships']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'users' => array(
      'left_field' => 'uid',
      'field' => 'from_uid',
      //'type' => 'INNER',
    ),
  );

  	// Setup UID Field
  $data['txtconnect_relationships']['from_uid'] = array(
        'title' => t('User ID'),
        'help' => t('The user assigned to the social media account'),
        'field' => array(
          'handler' => 'views_handler_field_user',
          //'click sortable' => TRUE,
        ),
        'relationship' => array(
          'base' => 'users',
          'field' => 'uid',
          'handler' => 'views_handler_relationship',
          'label' => t('User'),
        ),
        'argument' => array(
      		'handler' => 'views_handler_argument_user_uid',
    	),
      ); //End uid

  	// Setup To Field
 $data['txtconnect_relationships']['to_uid'] = array(
        'title' => t('To User'),
        'help' => t('The user receiving sms'),
       'field' => array(
          'handler' => 'views_handler_field_user',
          //'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'base' => 'users',
          'field' => 'uid',
          'handler' => 'views_handler_relationship',
          'label' => t('User'),
        ),
        'argument' => array(
      		'handler' => 'views_handler_argument_user_uid',
    	),
      ); //End uid

	// Setup Service Name Field
  $data['txtconnect_relationships']['message'] = array(
    'title' => t('Message'),
    'help' => t('Message'),
    'field' => array(
      'handler' => 'views_handler_field',
      //'click sortable' => TRUE,
    ),
  );


  // Setup Count Field
  $data['txtconnect_relationships']['date_created'] = array(
    'title' => t('Date Created'),
    'help' => t('Date relationship created'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}



/*function txtconnect_views_data_alter(&$data) {
  $data['users']['table']['join']['txtconnect_relationships'] = array(
    'left_table' => 'users',
    'left_field' => 'uid',
    'field' => 'uid',
  );
  /*$data['users']['table']['join']['twitter_account'] = array(
    'left_table' => 'twitter_user',
    'left_field' => 'uid',
    'field' => 'uid',
  );
  $data['users']['table']['join']['twitter_user'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );
  *
}
*/
/**
 * @}
 */
