<?php

	/**
	 * Implementation of hook_admin_settings().
	 *		Purpose: Setup form items for the module admin settings
	 */
function imagefetchr_admin_settings() {
	$form = array();

    $form['instructions'] = array(
      '#type'=> 'item',
      '#title'=>'',
      '#value'=>'This module will allows the ability to associate an image with nodes by scanning form fields for any URLs. Using a similar process to Facebook Share, the module uses Javascript to locate any URLs in the content-type fields when adding/editing content.  Once a URL is found the module will retreive all the images from the link and allow one to be selected.',
    );

	$form['imagefetchr_node_types'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Select the node types'),
		'#description' => t('The module will create a fieldgroup named \'group_imagefetchr\' with a field named \'field_image\' for each of the node types selected.'),
		'#default_value' => variable_get('imagefetchr_node_types', array()),
		'#options' => node_get_types('names'),
	);


	/*  TODO: Allow user to select which fields from node types; This will be used to tell Javascript which fields to scan for URLs
     
     $nodeTypes = node_get_types('names');	//Get Node Types
        //Cycle thru each node type
      foreach($nodeTypes as $type) {
        $form['imagefetchr_node_type_'.$type] = array(
          '#type' => 'checkbox',
          '#title' => t($type),
          '#description' => t('Select the node types to be used for ImageFetchr'),
          //'#default_value' => variable_get('imagefetchr_node_types', array()),
          //'#options' => node_get_types('names'),
        );

          //Get fields for each node type
      }

      foreach(node_get_types('names') as $nodetype) {
		
        $default_fields = imagefetchr_get_node_fields($nodetype);           // Get default fields for content type
        $cck_fields = array_keys(content_fields(NULL, $nodetype));          // Get array just for field name stored in array key
        $fields = array_merge((array)$default_fields, (array)$cck_fields);  // Merge array as one for checkboxes.

		$form['imagefetchr_'.$nodetype.'_fields'] = array(
			'#type' => 'checkboxes',
			'#title' => t($nodetype.' Node fields'),
			'#description' => t('Select fields to be searched for images used in ImageFetchr'),
			'#default_value' => variable_get('imagefetchr_'.$nodetype.'_fields', array()),
			'#options' => $fields,
		);
	}*/
	
	$form['imagefetchr_thumbnail_height'] = array(
		'#type' => 'textfield',
		'#title' => t('Min. Thumbnail Height'),
		'#description' => t('The minimuim height of images to return from url'),
		'#default_value' => variable_get('imagefetchr_thumbnail_height', '125'),
	);
	
	$form['imagefetchr_thumbnail_width'] = array(
		'#type' => 'textfield',
		'#title' => t('Min. Thumbnail Width'),
		'#description' => t('The minimuim width of images to return from url'),
		'#default_value' => variable_get('imagefetchr_thumbnail_width', '125'),
	);
	
	$form['imagefetchr_bluga_apikey'] = array(
			'#type' => 'textfield',
			'#title' => t('Bluga API Key'),
			'#description' => t('API key from your account on Webthumb.Bluga.net. '). l("Get Account","http://webthumb.bluga.net"),
			'#default_value' => variable_get('imagefetchr_bluga_apikey', ''),
	);
	
	$form['imagefetchr_bluga_userid'] = array(
			'#type' => 'textfield',
			'#title' => t('User ID'),
			'#default_value' => variable_get('imagefetchr_bluga_userid', ''),
	);

	$form['#submit'][] = 'imagefetchr_admin_settings_submit';

	 return system_settings_form($form);
}


	/**
	 *  Additional submit function for admin settings form
	 *	
	 */
function imagefetchr_admin_settings_submit($form, &$form_state) {
  
      // Need to load the CCK include file where content_field_instance_create() is defined.
	module_load_include('inc', 'content', 'includes/content.crud');

		// Get the node types defined by admin settings
	$nodeTypes = $form_state['values']['imagefetchr_node_types'];
	
		// Loop thru and add new field to each 
	foreach ($nodeTypes as $type) {

        // load existing groups for node type
      $groupExists = fieldgroup_groups($type);
          // if group doesnt exist, create it
      if(!$groupExists['group_imagefetchr']) {
          $group = previewgenerator_get_group_param();
          $group['fields']['field_image']['type_name'] = $type;
          if(module_exists('fieldgroup')){
            fieldgroup_save_group($type, $group);
          }
      }

          // Load field parameters from defaults and set $type
      $field = imagefetchr_get_field_param(); 
      $field['type_name'] = $type;

        // Check if field already exists
      $fieldExist = content_fields('field_image', $type);
 
        // If field doesn't exist, create it otherwise update it
      if(!$fieldExist) {
        content_field_instance_create($field);
      } else {
        content_field_instance_update($field, FALSE);
      }
      
        // Place field into fieldgroup
      if(module_exists('fieldgroup')){
        fieldgroup_update_fields($field);
      }
	}

      // Clear content cache
    content_clear_type_cache();
    
}


	/**
	*	Construct the field to store image
	*/
function imagefetchr_get_field_param(){

	$output = array(
		'field_name' => 'field_image',
		'type_name' => '',
        'group' => 'group_imagefetchr',
        'display_settings' => array (
			'weight' => '1',
			'label' => array (
				'format' => 'hidden',
			),
			'teaser' => array (
				'format' => 'image_nodelink',
				'exclude' => 1,
			),
			'full' => array (
				'format' => 'image_plain',
				'exclude' => 1,
			),
			4 => array (
				'format' => 'thumbnail_linked',
				'exclude' => 0,
			),
		),
		'widget_active' => '1',
		'type' => 'filefield',
		'required' => '0',
		'multiple' => '0',
		'db_storage' => '1',
		'module' => 'filefield',
		'active' => '1',
		'locked' => '0',
		'columns' => array (
			'fid' => array (
				'type' => 'int',
				'not null' => false,
			),
			'list' => array (
				'type' => 'int',
				'size' => 'tiny',
				'not null' => false,
			),
			'data' => array (
				'type' => 'text',
				'serialize' => true,
			),
		),
		'list_field' => '0',
		'list_default' => 1,
		'description_field' => '0',
		'widget' => array (
			'file_extensions' => 'jpg jpeg png gif',
			'file_path' => '',
			'max_filesize_per_file' => '',
			'max_filesize_per_node' => '',
			'max_resolution' => '0',
			'min_resolution' => '0',
			'alt' => '',
			'custom_alt' => 0,
			'title' => '',
			'custom_title' => 0,
			'default_image' => NULL,
			'use_default_image' => 0,
			'label' => 'Upload Thumbnail',
			'weight' => '2',
			'description' => '',
			'type' => 'imagefield_widget',
			'module' => 'imagefield',
		),
	);

	return $output;
}


    /**
	*	Construct the fieldgroup to store image
	*/
function previewgenerator_get_group_param(){

	$output = array(
    'group_type' => 'standard',
    'type_name' => '',
    'group_name' => 'group_imagefetchr',
    'label' => 'Select Image/Thumbnail',
    'settings' => array (
      'form' => array (
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array (
        'description' => '',
        'label' => 'above',
        'teaser' => array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        4 => array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        2 => array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        3 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '4',
  );

  return $output;

}