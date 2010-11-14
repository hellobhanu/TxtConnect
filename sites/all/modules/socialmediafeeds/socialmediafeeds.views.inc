<?php
// $Id:  $

/**
 * @file
 * Provide views data and handlers for twitter.module
 */

/**
 * @defgroup views_twitter_module twitter.module handlers
 *
 * Includes the ability to create views of just the twitter table.
 * @{
 */

/**
 * Implementation of hook_views_handlers()

function socialmediafeeds_views_handlers() {
  return array(
    'handlers' => array(
      'twitter_views_handler_field_profile_image' => array(
        'parent' => 'views_handler_field',
        'file' => 'twitter_views_field_handlers.inc',
      ),
      'twitter_views_handler_field_xss' => array(
        'parent' => 'views_handler_field',
        'file' => 'twitter_views_field_handlers.inc',
      ),
    ),
  );
}
 */


/**
 * Implementation of hook_views_data()
 *  
 *  Setup the ability to choose Social Media Feed fields in Views
 */
function socialmediafeeds_views_data() {

		// Create Group Name
  $data['socialmediafeeds_useraccounts']['table']['group']  = t('Social Media Feeds');

		// Advertise this table as a possible base table
  $data['socialmediafeeds_useraccounts']['table']['base'] = array(
    'field' => 'sid',
    'alias'=> 'smf_users',
    'title' => t('Social Media Account'),
    'help' => t('Social Media Accounts'),
    'weight' => 10,
  );
  
  	// Setup Join
  $data['socialmediafeeds_useraccounts']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      'type' => 'INNER',
    ),
  );
  
  
  	// Setup UID Field
  $data['socialmediafeeds_useraccounts']['uid'] = array(
        'title' => t('User ID'),
        'help' => t('The user assigned to the social media account'),

        'field' => array(
          'handler' => 'views_handler_field_user',
          'click sortable' => TRUE,
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
  
  	// Setup ScreenName Field
  $data['socialmediafeeds_useraccounts']['screen_name'] = array(
    'title' => t('Login name'),
    'help' => t('The login account of the Social Media user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    
  );

		// Setup Service Name Field
  $data['socialmediafeeds_useraccounts']['service'] = array(
    'title' => t('Service Name'),
    'help' => t('Service Name'),
    'field' => array(
      'handler' => 'views_handler_field',
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


   // Setup Placement Field
  $data['socialmediafeeds_useraccounts']['placement'] = array(
    'title' => t('Placement'),
    'help' => t('Location of Status Block'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  
  // Setup Count Field
  $data['socialmediafeeds_useraccounts']['count'] = array(
    'title' => t('Count'),
    'help' => t('Number of entries to retrieve'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  return $data;
}



function socialmediafeeds_views_data_alter(&$data) {
  $data['users']['table']['join']['socialmediafeeds_useraccounts'] = array(
    'left_table' => 'uid',
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
  */
}

/**
 * @}
 */
