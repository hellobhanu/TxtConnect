<?php

	/**
	 * Implementation of hook_admin_settings().
	 *		Purpose: Setup form items for the module admin settings
	 */
function previewgenerator_admin_settings() {
	$form = array();

	/*$nodeTypes = node_get_types('names');	//Get Node Types
	
		//Cycle thru each node type
	foreach($nodeTypes as $type) {
		
		
		//Get fields for each node type
	}
	*/
	
	$form['previewgenerator_node_types'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Node types'),
		'#description' => t('Select the node types to be used for ImageFetchr'),
		'#default_value' => variable_get('previewgenerator_node_types', array()),
		'#options' => node_get_types('names'),
	);
	
	foreach(node_get_types('names') as $nodetype) {
		
		$fields = content_fields(NULL, $nodetype);
		//echo "<pre>"; print_r($fields); echo "</pre>";
		$form['previewgenerator_'.$nodetype.'_fields'] = array(
			'#type' => 'checkboxes',
			'#title' => t($nodetype.' Node fields'),
			'#description' => t('Select fields to be searched for images used in ImageFetchr'),
			'#default_value' => variable_get('previewgenerator_'.$nodetype.'_fields', array()),
			'#options' => $fields,
		);
	}	
	
	
	
	
	$form['previewgenerator_thumbnail_height'] = array(
		'#type' => 'textfield',
		'#title' => t('Min. Thumbnail Height'),
		'#description' => t('The minimuim height of images to return from url'),
		'#default_value' => variable_get('previewgenerator_thumbnail_height', '125'),
	);
	
	$form['previewgenerator_thumbnail_width'] = array(
		'#type' => 'textfield',
		'#title' => t('Min. Thumbnail Width'),
		'#description' => t('The minimuim width of images to return from url'),
		'#default_value' => variable_get('previewgenerator_thumbnail_width', '125'),
	);
	
	$form['previewgenerator_bluga_apikey'] = array(
			'#type' => 'textfield',
			'#title' => t('Bluga API Key'),
			'#description' => t('API key from your account on Bluga.net'),
			'#default_value' => variable_get('previewgenerator_bluga_apikey', ''),
	);
	
	$form['previewgenerator_bluga_userid'] = array(
			'#type' => 'textfield',
			'#title' => t('User ID'),
			'#default_value' => variable_get('previewgenerator_bluga_userid', ''),
	);

	$form['#submit'][] = 'previewgenerator_admin_settings_submit';

	 return system_settings_form($form);
}


	/**
	 *  Additional submit function for admin settings form
	 *	
	 */
function previewgenerator_admin_settings_submit($form, &$form_state) {
  
			// Need to load the CCK include file where content_field_instance_create() is defined.
	module_load_include('inc', 'content', 'includes/content.crud');

		// Get the node types defined by admin settings
	$nodeTypes = $form_state['values']['previewgenerator_node_types'];
	
		// Loop thru and add new field to each 
	foreach ($nodeTypes as $type) {
  		// load existing groups for node type
  	$groupExists = fieldgroup_groups($type);	
  		// if group doesnt exist, create it
  	if(!$groupExists['group_imagefetchr']) {
  		$group = previewgenerator_get_group_param();
  		$group['fields']['field_image']['type_name'] = $type;
  		fieldgroup_save_group($type, $group);
  	}
  		// Load existing field
  	$fieldExist = content_fields('field_image', $type);
  		// make sure field doesn't already exists	
  	if(!$fieldExist) {
			$field = previewgenerator_get_field_param();
			$field['type_name'] = $type;
			content_field_instance_create($field);
			//fieldgroup_update_fields($field);
  	} else {
  		$field = previewgenerator_get_field_param();
			$field['type_name'] = $type;
			content_field_instance_update($field, FALSE);
  	}
	}
	
}


	/**
	*	Construct the field to store image
	*/
function previewgenerator_get_field_param(){

	$output = array(
		'field_name' => 'field_image',
		'type_name' => '',
		'display_settings' => array (
			'weight' => '1',
			'parent'=> array(
				'default_value'=> 'group_imagefetchr',
				'value'=> 'group_imagefetchr',
			),
			'prev_parent'=> array(
				'value'=> 'group_imagefetchr',
			),
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
		'parent'=> array(
			'default_value'=> 'group_imagefetchr',
			'value'=> 'group_imagefetchr',
		),
		
		'prev_parent'=> array(
			'value'=> 'group_imagefetchr',
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


function asdfasdf(){

	
		//Remove Field group
	fieldgroup_remove_group(&$form_state, $type_name, $group_name);

}