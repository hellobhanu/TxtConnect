<?php
// $Id: template.php,v 1.16.2.1 2009/02/25 11:47:37 goba Exp $

//drupal_rebuild_theme_registry();


function debugMe($txt){
	echo "<pre>"; print_r($txt); echo "</pre>";
}


/******************
* txtconnect THEME
*  Implementation of hook_theme.
*
* Purpose:  Register custom theme functions.
* 
* Defined:
*   - txtconnect_blog_node_form($form, &form_state)
*		
*		inherit:
*		- txtconnect_preprocess_blod_node_form($form, &form_state)
*		- blog_node_form_submit($form, &form_state)
*/
function txtconnect_theme_theme() {
   return array(
     
	 	//Theme function for the blog node form
     'blog_node_form' => array(
       // Forms always take the form argument.
       'arguments' => array('form' => NULL),
	   'template' => 'node-blog-form',
     ),
	 /*'user_login' => array(
		'template' => 'user-login',
      	'arguments' => array('form' => NULL),
     ),*/
	 
	/* 'user_profile_form' => array(
      // Forms always take the form argument.
      'arguments' => array('form' => NULL),
	  'template' => 'user-edit-profile',
    ),*/
   );
}


	/*
	* Sets the body-tag class attribute.
	*	- Taken from Garland Theme

	* Adds 'sidebar-left', 'sidebar-right' or 'sidebars' classes as needed.
	*/
function phptemplate_body_class($left, $right) {
  if ($left != '' && $right != '') {
    $class = 'sidebars';
  }
  else {
    if ($left != '') {
      $class = 'sidebar-left';
    }
    if ($right != '') {
      $class = 'sidebar-right';
    }
  }

	$class .= " ".checkHost();

  if (isset($class)) {
    print ' class="'. $class .'"';
  }
}


/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' › ', $breadcrumb) .'</div>';
  }
}



/**
 *  Customize RSS feed icon
 *
 * @param $url
 *   URL for RSS icon to link to
 * @param $title
 *   Title for RSS icon
 * @return a string containing the RSS icon output.
 */
function txtconnect_theme_feed_icon($url, $title){
	if ($image = theme('image', '/misc/feed.png', t('RSS Feed'), $title)) {
  
    	$output = '<div id="top-feed-container"><a href="'. check_url($url) .'" class="feed-icon">'. $image .'</a> ';
    	$output .= '<a href="'. check_url($url) .'" class="feed-icon">RSS Feed</a></div>';
    	return $output;
	}
}


/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $node) {
  if (!$content || $node->type == 'forum') {
    return '<div id="comments">'. $content .'</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2>'. $content .'</div>';
  }
}



/**
 * Override or insert PHPTemplate variables into the templates.
 */
function txtconnect_theme_preprocess_page(&$vars) {
   
  //echo "<pre>"; print_r($vars['tabs']); echo "</pre>";

 
  $vars['tabs2'] = menu_secondary_local_tasks();

  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
 
}


function checkHost(){
	$host = $_SERVER['HTTP_HOST'];
	$subdomain = explode(".",$host);
	
	switch($host) {
		case "synapse.local":
			$output = "testborder";
		break;
	}
	
	switch($subdomain[0]) {
		case "wallofcool-testing":
			$output = "testborder";
		break;
		case "wallofcool-staging":
			$output = "stageborder";
		break;
	}
	
	return $output;
}


/*****************************
PREPROCESS USER EDIT FORM
*/
function txtconnect_theme_preprocess_user_profile_form(&$vars) {
  
  //echo "<pre>"; print_r ($vars); echo "</pre>";
  
  /*if (function_exists('_password_policy_load_active_policy')) {
    $policy = _password_policy_load_active_policy();
    if ($policy && isset($policy['description'])) {
      $form['account']['pass']['#description'] = '<p>'. $form['account']['pass']['#description'] .'</p><p>'. $policy['description'] . '</p>';
    }
  }*/
  //return drupal_render($form);
}



/*****************************
PREPROCESS USER PROFILE PAGE
*/
function phptemplate_preprocess_user_profile(&$vars) {

		//Determine which name field to use, look for profile name if not found use account username
	if($vars['account']->profile_name){
		$name = $vars['account']->profile_name;
	} else {
		$name = $vars['account']->name;
	}
	
		//If picture attached to profile, use imagecache to display large version
	if ($vars['account']->picture) {
        $vars['profile_pic'] = theme('imagecache', 'user_profile_lrg', $vars['account']->picture);	// Set template with image themed using high res
    }
	

		//Assign Profile block to profile page
	/*$variables['profile'] = array();
	  // Sort sections by weight
	uasort($variables['account']->content, 'element_sort');
	  // Provide keyed variables so themers can print each section independantly.
	foreach (element_children($variables['account']->content) as $key) {
		$variables['profile'][$key] = drupal_render($variables['account']->content[$key]);
	}
	  // Collect all profiles to make it easier to print all items at once.
	  $variables['user_profile'] = implode($variables['profile']);
	  $variables['profile_block'] = theme('blocks', 'profile_block');
  */
	
		
	drupal_set_title($name); 				// Overide page title and set to Profile Name
	$vars['name'] = $name;					// Send name variable to template
	$vars['bio'] = $vars['account']->content['Personal Information']['profile_bio']['#value'];				// set bio field to template
	$vars['job_title'] = $vars['account']->content['Personal Information']['profile_jobtitle']['#value'];	// set job title to template
	
	//$account['profile_education']
	
	
}
 
 
/*****************************
CUSTOMIZE USERNAME OUTPUT
*/
function phptemplate_username($object) {
 
  if ($object->uid && $object->name) {
    // Shorten the name when it is too long or it will break many tables.
    
    $name = $object->name;
    
    $profile = user_load(array('uid' => $object->uid));
    if ($profile->profile_name) {
      $name = $profile->profile_name;
    }
 
    if (user_access('access user profiles')) {
      $output = l($name, 'user/'. $object->uid, array('title' => t('View user profile.')));
    }
    else {
      $output = check_plain($name);
    }
  }
  else if ($object->name) {
    // Sometimes modules display content composed by people who are
    // not registered members of the site (e.g. mailing list or news
    // aggregator modules). This clause enables modules to display
    // the true author of the content.
    if ($object->homepage) {
      $output = l($object->name, $object->homepage);
    }
    else {
      $output = check_plain($object->name);
    }
 
    $output .= ' ('. t('not verified') .')';
  }
  else {
    $output = variable_get('anonymous', t('Anonymous'));
  }
 
  return $output;
} 


/****************
* PREPROCESS NODE

 Purpose: Preprocess Node Variables
*/
function phptemplate_preprocess_node(&$vars, $hook) {
 
	
	$node = $vars['node'];
	
	switch ($node->type){
	 
        case "project":
          //echo "<pre>"; print_r($node); echo "</pre>";

              //Define thumbnail image field as imagecache version
			if ($node->field_image[0]['fid'] && file_exists($node->field_image[0]['filepath'])) {
			  $vars['thumbnail'] = theme('imagecache', "list_page", $node->field_image[0]['filepath']);
			  $vars['image_lrg'] = theme('imagecache', "single_page", $node->field_image[0]['filepath']);
			}

          break;
	 		//NODE TYPE EQUALS BLOG
		case "blog":
	 		//echo "<pre>"; print_r ($node->field_link[uid]); echo "</pre>";
			
			$links = $node->links;
			
				//Define thumbnail image field as imagecache version
			if ($node->field_image[0]['fid'] && file_exists($node->field_image[0]['filepath'])) {
			  $vars['thumbnail'] = theme('imagecache', "thumbnail", $node->field_image[0]['filepath']);
			   $vars['image_lrg'] = theme('imagecache', "rssimage-lrg", $node->field_image[0]['filepath']);
			}
			
			if($node->field_link[0]['url']) {
				$links['source'] = array(
										 "title"=>"Source",
										 "href"=>$node->field_link[0]['display_url'],
										 "attributes"=>array('title' => "Go to Source URL", "target"=>"_blank"),
										);
				$vars['link_url'] = $node->field_link[0]['display_url'];
				$vars['link_target'] = 'target="_blank"';
 			}
		
			if (module_exists('fivestar')) {
				$fivestarwidget = fivestar_widget_form($node);
				$vars['fivestar'] = $fivestarwidget;
			}
				/* 
				FORMAT LINKS for BLOG NODES
					Reference:
						- http://www.seo-expert-blog.com/blog/drupal-block-snippet-recent-comments-by
				
				*/
				
			//echo "<pre>"; print_r ($vars['node']->links); echo "</pre>";
				
			
					
				//Remove comment from links and place into $comment_link for template
			/*if (isset($links['comment_comments'])) {
				$vars['comment_link'] = l($links['comment_comments']['title'], $node->path, array('title' => $links['comment_comments']['attributes']['title']),
				NULL, 'comments', FALSE, TRUE);
				unset($links['comment_comments']);
			}
			if (isset($links['comment_add'])) {
				$vars['comment_link'] = l($links['comment_add']['title'], $links['comment_add']['href'], array('title' => t('Add your comment')), NULL, 'comment-form', FALSE, TRUE);
				unset($links['comment_add']);
		 	}*/
			
				//Format Read More link
			if(isset($links['node_read_more'])) {
					//set class for blog links
				//$vars['more_link'] = theme('links', $links['node_read_more'], array('class' => 'readmore inline'));
				$vars['more_link'] = l("Continue reading...", $links['node_read_more']['href'], array('title' => $links['node_read_more']['attributes']['title']),
				NULL, 'commentblock');
				unset($links['node_read_more']);
			}
			
			unset($links['blog_usernames_blog']);
			
			
			$vars['links'] = theme('links', $links, array('class' => 'links inline'));
			
			//echo "<pre>"; print_r ($vars['node']->links); echo "</pre>";
	 	break;
	 
	}
 
 }
 

function phptemplate_service_links_node_format($links) {
  $output = '<div class="service-links">';
   //$output .= '<div class="service-label">'. t('Share') .' </div>';
//   $output .= '<div class="service-container">';
//   $output .= '<div class="service-tabs"><ul class="service-tabs-list"><li class="selected"><a href="#">Social</a></li><li><a href="">Email</a></li></ul><div class="closebtn"><a href="#">Close</a></div></div>';
//   $output .= '<div class="windows-container">';
   $output .= '<div class="container-social">'. theme('links', $links) .'</div>';
//   $output .= '<div class="container-email"></div>';
//   $output .= '</div>';
//   $output .= '</div>';
   $output .= '</div>';
   
   return $output;
}


	/*
	 PREPROCESS BLOG NODE FORM
	 Purpose: Preprocess variables sent to Blog Node Form
	*/
function phptemplate_preprocess_blog_node_form(&$vars) {
	
	$form = $vars['form'];
	
	
	
	//$form['buttons']['submit']['#attributes']['title'] = 'submit-blog-button';
	
	//$form['#attributes'] = array('class' => 'search-form');
	
	
	//echo "<pre>"; print_r($form['buttons']); echo "</pre>";
	
	/*foreach($cookie as $key=>$value) {
		if($key == "edit[title]") {
			$vars['form']['title']['#value'] = $value;
		}
		
		if($key == "edit[field_link][0][url]") {
			$vars['edit']['field_link'][0]['url'] = $value;
		}
		//echo $key .":". $value;
		//$_GET[$key] = $value;	
	}*/
	//echo "<pre>";print_r($vars['edit']['title']); echo "</pre>";
	//unset($vars['form']['buttons']['preview']);	//remove preview button
	//$vars['form']['buttons']['submit']['#weight'] = 100;
	//$vars['form']['buttons']['delete']['#weight'] = 100;
	//$vars['form']['#submit'] = array("txtconnect_user_login_submit");
	
	//echo "<pre>"; print_r($vars); echo "</pre>";
	//echo "<pre>"; print_r($vars['form']['#validate']); echo "</pre>";
	//echo "<pre>"; print_r(get_defined_vars()); echo "</pre>";
	
		//
	//$vars['form']['category'][] = $vars['form']['taxonomy']['2'];
	//$vars['form']['category']['#weight'] = 0;
	//unset($vars['form']['taxonomy']['2']);
	//$vars['form']['keywords']['tags'] = $vars['form']['taxonomy']['tags'];
	//$vars['form']['keywords']['#weight'] = 75;
	//unset($vars['form']['taxonomy']['tags']);
	//echo "<pre>"; print_r($vars['form']['taxonomy']['tags'][1]); echo "</pre>";
	return $vars;
}


	/*
	 PREPROCESS USER LOGIN
	 Purpose: preprocess variables sent to user login node
	*/
/*function txtconnect_preprocess_user_login(&$variables) {
  
 
}*/



	/*
	 BLOG NODE SUBMIT FORM
	
	 Purpose: Define custom events when BLOG NODE FORM is submitted
	
	 SourceCode: 
	  - Programatically creating filefield entries (http://drupal.org/node/330421)
	*/
/*function blog_node_form_submit($form, &$form_state){
	
	//INITIATE PROCESS TO GENERATE THUMBNAIL FROM URL
		
	//echo "<pre>"; print_r($form_state['values']); echo "</pre>";
	
	if(!$form_state['values']['field_thmbimage'][0][fid]) {	
			//Check if url exists in link field
		if($form_state['values']['previewgenerator_file']) {
			
				//Call function to create local image thumbnail from url provided  //Set true to recreate image thumbnail
			$image = txtconnect_getImage($form_state['values']['previewgenerator_file'], false);
			
				//Create file array of details to be inserted into DB 
			$file = new stdClass();
			
				//Search DB for fid of file just created
			$file->fid = db_result(db_query("SELECT fid FROM {files} WHERE filename = '%s'", basename($image)));
			
			drupal_set_message("Check File ID:". $file->fid);
			
			if(!$file->fid) {
			
			  $file->filename = basename($image);
			  $file->filepath = $image;
			  $file->filemime = "image/jpeg";
			  $file->filesize = filesize($image);
			  $file->uid = 3;
			  $file->status = 1;
			  $file->timestamp = time();
			  
				//insert file object into database
			  drupal_write_record('files', $file);
			  
				//Search DB for fid of file just created
			  $file->fid = db_result(db_query("SELECT fid FROM {files} WHERE filename = '%s'",  array(
				  '%s' => basename($image),
				)));
			}
				//Assign newly created file object to blog node
			$form_state['values']['field_thmbimage'][0] = array(
					'fid' => $file->fid,
					'title' => basename($image),
					'filename' => basename($image),
					'description' => basename($image),
					'filepath' => $image,
					'filesize' => filesize($image),
					'list'=> 1,
				);
		}
	}
		//USED FOR DEBUG
	$form_state['redirect'] = "";
	//echo "<pre>"; print_r($form); echo "</pre>";
	echo "<pre>"; print_r($form_state); echo "</pre>";
}*/



	 /*
	 	FILE AUTO VERSION FILE NAME
	 	Purpose: Enable control of client side browser-caching by naming files according to their last modified date
	 	 - Appends last modfied date of file to the filename
	 
	  	$url - relative path to root of file to be auto versioned
	 */
function phptemplate_fileAutoVer($url){

 	$path = pathinfo($url);
	$ver = '_'. date("mdYgis", filemtime($_SERVER['DOCUMENT_ROOT'].$url)) .'.';
	$filename = str_replace('.', $ver, $path['basename']);
	return $path['dirname'].'/'. $filename;
}

	 /**
	 *  Enable client side browser-caching control for URLS
	 		- Appends last modfied date of file to URL
	 *
	 * $url - URI of file to be auto versioned
	 */
function phptemplate_urlAutoVer($url){

	$path = pathinfo($url);	//Get path attributes to be used to replace filename and construct return url
	$headerArray = (get_headers($url,1));	//Get headers from URL 
	$lastModTime = '_'. date("mdYgis", strtotime($heaerArray['Last-Modified'])) .'.';	//Get last modified date from header, format into mysql date format
	$filename = str_replace('.', $lastModTime, $path['basename']);		//Replace . in filename of url provided with last modified date
	return $path['dirname'].'/'. $filename;	//Return URL

}



	/**
	 * Returns the rendered local tasks. The default implementation renders
	 * them as tabs. Overridden to split the secondary tasks.
	 *
	 * @ingroup themeable
	 */
function phptemplate_menu_local_tasks() {
  return menu_primary_local_tasks();
}




	/*
		FORMAT BLOG AUTHOR/DATE DETAILS	
		Purpose: Format each blog author and date details
	
		Reference: http://www.kirkdesigns.co.uk/format-submitted-text-drupal-6-nodes-and-comments
	*/
function phptemplate_node_submitted($node) {
  return t('Posted by !username on @date',
    array(
      '!username' => theme('username', $node),
      '@date' => format_date($node->created, 'custom', 'M d, Y')
    ));
}


	/*
		FORMAT BLOG COMMENT AUTHOR/DATE DETAILS
		Purpose: Format each blog comment's author and date detials
	*/
function phptemplate_comment_submitted($comment) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $comment),
      '!datetime' => format_date($comment->timestamp)
    ));
}


	/*
	 	GET IE STYLES
	 	Purpose: Generates IE CSS links for LTR and RTL languages.
	 */
function phptemplate_get_ie_styles() {
  global $language;

  $iecss = '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . path_to_theme() .'/fix-ie.css" />';
  if ($language->direction == LANGUAGE_RTL) {
    $iecss .= '<style type="text/css" media="all">@import "'. base_path() . path_to_theme() .'/fix-ie-rtl.css";</style>';
  }

  return $iecss;
}
	
	
	/*
		CUSTOMIZE SEARCH BLOCK FORM
	*/
function phptemplate_search_block_form(&$form) { 
  $form['search_block_form']['#title']=NULL;
  $form['submit']['#value']='Search';
 // echo "<pre>"; print_r($form); echo "</pre>";
  
  return drupal_render($form);
  
}


/* UNNEEDED VIEWS PREPROCESS FUNCTIONS*/
/*function txtconnect_preprocess_views_view_unformatted__member_list(&$variables) {
}

function txtconnect_preprocess_views_view_fields__member_list(&$variables) {
//echo "<pre>"; print_r($variables['fields']['rid']->handler->items); echo "</pre>";

} */
//
//class ogur_handler_field_og_users_roles extends views_handler_field_prerender_list {
//  function construct() {
//    parent::construct();
//    $this->additional_fields['uid'] = array('table' => 'users', 'field' => 'uid');
//    $this->additional_fields['gid'] = array('table' => 'og', 'field' => 'nid');
//  }
//
//  function query() {
//    $this->add_additional_fields();
//    $this->field_alias = $this->aliases['uid'];
//  }
//
//  function pre_render($values) {
//    $uids = array();
//    $this->items = array();
//    $group_id = false;
//
//    foreach ($values as $result) {
//      $uids[] = $result->{$this->aliases['uid']};
//      if(!$group_id)
//     {
//        $group_id = $result->{$this->aliases['gid']};
//     }
//    }
//
//    if ($uids) {
//      $result = db_query("SELECT u.uid, u.rid, r.name FROM {role} r INNER JOIN {og_users_roles} u ON u.rid = r.rid WHERE u.uid IN (" . implode(', ', $uids) . ") AND u.gid = {$group_id} ORDER BY r.name");
//      while ($role = db_fetch_object($result)) {
////        $this->items[$role->uid][$role->rid] = check_plain($role->name);
//          $this->items[$role->uid][$role->rid] = l(t($role->name), "og/users/$group_id/roles", array('query' => drupal_get_destination()));
//      }
//    }
//  }
//}


/*function txtconnect_links($node){
	return theme("links",$node->links, array('class' => 'mylinks'));
}*/
?>