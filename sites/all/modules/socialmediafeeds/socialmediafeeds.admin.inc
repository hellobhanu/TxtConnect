<?php

	/**
	*	Implimentation of hook_admin_settings
	*
	*	Setup module admin settings form fields	
	*/
function socialmediafeeds_admin_settings() {
	$form = array();

		//Create form for node types
	$form['socialmediafeeds_node_types'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Node types'),
		'#description' => t('Select the node types for SocialShare links to display.'),
		'#default_value' => variable_get('socialmediafeeds_node_types', array()),
		'#options' => node_get_types('names'),
	);
	
		// Create form field for social media services
	$form['socialmediafeeds_services'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Social Services'),
    '#options' => array(
						'Twitter'=>'Twitter',
						'Facebook'=>'Facebook', 
						'LinkedIn'=>'LinkedIn'),
    '#default_value' => variable_get('socialmediafeeds_services', array()),
  );
	
	$form['socialmediafeeds_status_messages'] = array(
			'#type'=>'fieldset',
			'#title'=>t('Status Messages'),
			'#collapsible'=>TRUE,
			'#collapsed'=>FALSE,
		);
	
		// Create form field for default status message
	$form['socialmediafeeds_status_messages']['socialmediafeeds_default_format'] = array(
		'#type' => 'textfield',
		'#title' => t('Default'),
		'#maxlength' => 140,
		'#description' => t('You can use !url, !url-alias, !tinyurl, !title, and !user as replacement text.'),
		'#default_value' => variable_get('socialmediafeeds_default_format', 'I just posted to my blog. !tinyurl'),
	);
	
		// Create form field for default status message
	$form['socialmediafeeds_status_messages']['socialmediafeeds_global_status'] = array(
		'#type' => 'textfield',
		'#title' => t('Global Accounts'),
		'#maxlength' => 140,
		'#description' => t('You can use !url, !url-alias, !tinyurl, !title, and !user as replacement text.'),
		'#default_value' => variable_get('socialmediafeeds_global_status', 'I just posted to my blog. !tinyurl'),
	);
	
		// Create form field for disable override global accounts
	$form['socialmediafeeds_status_messages']['socialmediafeeds_disableoveride_global'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable overriding global accounts'),
    '#return_value' => 1,
    '#default_value' => variable_get('socialmediafeeds_disableoveride_global', ''),
  );
  
  	// Create form field for disable global accounts
	$form['socialmediafeeds_status_messages']['socialmediafeeds_hide_global'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide global accounts from view'),
    '#return_value' => 1,
    '#default_value' => variable_get('socialmediafeeds_hide_global', ''),
  );
	
	$serviceList = socialmediafeeds_get_service_list();	// Get service list
	
		//For each service, create api & secret key fields
	foreach($serviceList as $service) {
		$form['socialmediafeeds_'.$service.'_settings'] = array(
			'#type'=>'fieldset',
			'#title'=>t($service.' Settings'),
			'#collapsible'=>TRUE,
			'#collapsed'=>FALSE,
		);
		
		$form['socialmediafeeds_'.$service.'_settings']['socialmediafeeds_'.strtolower($service).'_apikey'] = array(
			'#type' => 'textfield',
			'#title' => t($service.' API Key'),
			'#default_value' => variable_get('socialmediafeeds_'.strtolower($service).'_apikey', ''),
		);
		
		$form['socialmediafeeds_'.$service.'_settings']['socialmediafeeds_'.strtolower($service).'_secretkey'] = array(
			'#type' => 'textfield',
			'#title' => t($service.' Secret Key'),
			'#default_value' => variable_get('socialmediafeeds_'.strtolower($service).'_secretkey', ''),
		);	
	
	}
	
	 return system_settings_form($form);
}



	/**
	* Get global accounts for admin
	* 
	*/
function socialmediafeeds_admin_global_accounts(){
	$output = '';
	$socialmediafeeds_accounts = socialmediafeeds_get_global_accounts();
	$account_type = "global";
	if (!empty($socialmediafeeds_accounts)) {
		$output .= drupal_get_form('socialmediafeeds_useredit_account_list', $socialmediafeeds_accounts, $account_type);
	} else {
		$output .= '<div class="noaccounts">'. t("No Accounts Found.") .'</div>';
	}
 	$output .= drupal_get_form('socialmediafeeds_add_account', $account, $account_type);
	

	return $output;

}

	/*
	*	Social network user edit settings
	*/
function socialmediafeeds_user_settings($account) {
 
  $output = '';
  // This is directly calling a hook implementation, which is bad and naughty,
  // but oh well. We'll fix this in the next reshuffling when user account
  // management gets an overhaul.
  $socialmediafeeds_accounts = socialmediafeeds_get_accounts($account);
 if (!empty($socialmediafeeds_accounts)) {

   $output .= drupal_get_form('socialmediafeeds_useredit_account_list', $socialmediafeeds_accounts, 'personal');
 } else {
 		$output .= '<div class="noaccounts">'. t('You do not currently have any social media accounts') .'</div>';
 }
 $output .= drupal_get_form('socialmediafeeds_add_account', $account, 'personal');

  return $output;
}


	/*
	*	Create Service List from admin settings
	*/
function socialmediafeeds_get_service_list(){
	$availServices = array();
	$services = variable_get('socialmediafeeds_services', 0);
	
	foreach($services as $sid=>$service) {
		if($service) {
			$availServices[$service] = $service;
		}	
	}
	
	return $availServices;
	
}

	/*
	*	Get social network account list
	*/
function socialmediafeeds_useredit_account_list($form_state, $socialmediafeeds_accounts = array(), $admin = 'personal') {
 	
	$form['#tree'] = TRUE;	//Make sure form is set to tree
	$form['accounts'] = array();	//Create new form element to hold accounts 
	if($admin == 'global'){
		$form['globalacct'] = array(
			'#type'=>'hidden',
			'#return_value'=>1,
			'#default_value'=>1,
		);
	}
		//Loop thru each and display in row
	foreach ($socialmediafeeds_accounts as $socialmediafeeds_account) {
		$form['accounts'][] = socialmediafeeds_account_list_row($socialmediafeeds_account);
	}

		//If accounts found, display save button
	if (!empty($socialmediafeeds_accounts)) {
		$form['buttons']['submit'] = array(
		  '#type' => 'submit',
		  '#value' => t('Save Changes'),
		);
	}

	return $form;
}
	
	/*
	*  format socialnetworking account row
	*/
function socialmediafeeds_account_list_row($account) {
  //echo"<pre>"; print_r($account); echo "</pre>";
  
  $form['#account'] = $account;

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account['uid'],
  );

  $form['screen_name'] = array(
    '#type' => 'value',
    '#value' => $account['screen_name'],
  );
  
  $form['visible_name'] = array(
    '#type' => 'markup',
    '#value' => $account['screen_name'],
  );

  $form['service'] = array(
    '#type' => 'value',
    '#value' => $account['service'],
  );
  
  $form['service_display'] = array(
    '#type' => 'markup',
    '#value' => $account['service'],
  );
  
  $form['placement'] = array(
    '#type' => 'select',
    '#default_value' => $account['placement'],
	'#options'=>array('none'=>'none','left'=>'left', 'right'=>'right'),
	//'#return_value'=> 1,
  );
  
  $form['count'] = array(
    '#type' => 'select',
    '#default_value' => $account['count'],
	'#options'=>array('1','2','3','4','5','6','7','8','9','10'),
	//'#return_value'=> 1,
  );
  
  $form['autopost'] = array(
    '#type' => 'checkbox',
    '#default_value' => $account['autopost'],
	'#return_value'=> 1,
  );

  $form['delete'] = array(
    '#type' => 'checkbox',
  );
  
  //echo"<pre>"; print_r($form); echo "</pre>";
  return $form;
}

	/*
	*	Themeable function to display existing account list
	*/
function theme_socialmediafeeds_useredit_account_list($form) {
  $header = array(t('Account'), t('Service'),t('Placement'),t('Count'),t('Auto Post'), t('Delete'));
  $rows = array();
  foreach (element_children($form['accounts']) as $key) {
    $element = &$form['accounts'][$key];
    $rows[] = array(
      drupal_render($element['uid']) . drupal_render($element['screen_name']) . drupal_render($element['visible_name']),
     drupal_render($element['service']) . drupal_render($element['service_display']),drupal_render($element['placement']), drupal_render($element['count']),
	 drupal_render($element['autopost']),
      drupal_render($element['delete']),
    );
	
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

	/*
	*	submit handler for account list table
	*/
function socialmediafeeds_useredit_account_list_submit($form, &$form_state) {
 
 	$admin = false;

	if($form_state['values']['globalacct']){
		$admin = true;
	}
 
  	$accounts = $form_state['values']['accounts'];
	
	foreach($accounts as $account) {
		if (empty($account['delete'])) {
		  socialmediafeeds_user_save($account, $admin);
		} else {
	  	socialmediafeeds_user_delete($account['uid'], $account['service'], $account['screen_name'], $admin);
		}
	}
}


	/*
	*	Generate form to add social media account
	*	
	*/
function socialmediafeeds_add_account($form_state, $account = NULL, $admin = 'personal') {

     // Get List of available services from Admin Settings
  $serviceList = socialmediafeeds_get_service_list();

      // Check if any services have been selected from admin settings
  if(!isset($serviceList)){
      return false;	//return false to hide add account form
  }

      // Check of account passed otherwise set to logged in user
  if (empty($account)) {
      global $user;
      $account = $user;
  }
  
  	// Check if SSL is on or not for Facebook API
  if(isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') {
  	$ssl = True;
  	$fbReceiver = 'xd_receiver_ssl.htm';
  } else {
  	$ssl = False;
  	$fbReceiver = 'xd_receiver.htm';
  }
  
   	// Add module specific files as well as facebook dependancies
  drupal_add_js(drupal_get_path('module', 'socialmediafeeds') .'/socialmediafeeds.js', 'module');
  drupal_add_css(drupal_get_path('module', 'socialmediafeeds') .'/socialmediafeeds.css', 'module');
  
  	// Check of server using SSL or not
  if($ssl) {
  	drupal_set_html_head('<script type="text/javascript" src="https://ssl.connect.facebook.com/js/api_lib/v0.4/FeatureLoader.js.php"></script>');
  } else {
  	drupal_set_html_head('<script src="http://static.ak.connect.facebook.com/js/api_lib/v0.4/FeatureLoader.js.php/en_US" type="text/javascript"></script>');
  }
	
    // Include API PHP libraries
  module_load_include('php', 'socialmediafeeds', 'twitter/twitteroauth');
  module_load_include('php', 'socialmediafeeds', 'linkedin/linkedinoauth');
	
  
    // Create new form element
  $form = array();
	
  if($admin == 'global'){
      $form['globalacct'] = array(
          '#type'=>'hidden',
          '#return_value'=>1,
          '#default_value'=>1,
      );
  }
	
    // Create
  $form['newaccount'] = array (
      '#type' => 'fieldset',
      '#title' => 'Add New Account',
      '#collapsible' 	=> TRUE,
      '#description' => '<p>Javascript must be enabled</p>'
  );
	
  $form['newaccount']['uid'] = array(
  '#type' => 'value',
  '#value' => $account->uid,
  );
	
  $form['newaccount']['service'] = array(
    '#type'	=>	'select',
    '#title'	=>	'Service',
    '#options'	=>	$serviceList,
    '#default_value' => $serviceid,
  );

  $form['newaccount']['screen_name'] = array(
      '#type' => 'hidden',
      '#required' => TRUE,
      '#title' => t('Username'),
      '#description'=>t('Enter your '.$serviceid.' username'),
      '#default_value'=> $form_state['newaccount']['screen_name'],
  );

  $form['newaccount']['password'] = array(
      '#type' => 'hidden',
      '#title' => t('Password'),
      //'#description' => t("If your Twitter account is protected, or you wish to post to Twitter from Drupal, you must enter the Twitter account's password.")
  );
	
		/* Check if there is a Twitter Response */
  if(isset($_SESSION['taccess_oauth_token']) && $_SESSION['toauth_verifier'] == 'verified'){

          /* Remove verified session */
      unset($_SESSION['toauth_verifier']);
      //unset($_SESSION['taccess_oauth_token']);
      //unset($_SESSION['taccess_token_secret']);

       /* Create Twitter Obj w/ twitter api keys */
      $twitter = new TwitterOAuth(variable_get('socialmediafeeds_twitter_apikey', ''), variable_get('socialmediafeeds_twitter_secretkey', ''), $_SESSION['taccess_oauth_token'], $_SESSION['taccess_token_secret']);
      //echo "<pre>"; print_r($twitter); echo "</pre>";
      $authRequest = $twitter->get('account/verify_credentials');
      //echo "<pre>"; print_r($authRequest); echo "</pre>";

      //$twitterInfo = simplexml_load_string($authRequest);
      switch($twitter->http_code) {
          case 200:
              //$usertimeline = $twitter->get('statuses/user_timeline', array("count"=>1));
              $screen_name = $authRequest->screen_name;
              $password = $_SESSION['taccess_oauth_token'] ."|". $_SESSION['taccess_token_secret'];

              $step1_value = '
                  <div class="title">Step #1: Select Account</div><div id="twitter-accountdetails"><img src="'.$authRequest->profile_image_url.'" border="0" align="absmiddle"/> <a href="http://www.twitter.com/'.$screen_name.'" target="_blank">'.$screen_name.'</a> (<a href="http://www.twitter.com/logout">Logout</a>)</div>
                  <script>
                    jQuery(document).ready(function() {
                      replaceAccount("Twitter", "'.$screen_name.'","'.$password.'");

                    });
                  </script>';
              $step2_value = '<div class="title">Step 2: Settings</div>';
              $step3_value = '<div class="title">Step 3: Add</div>';
              $hideContainer = '';
          break;

          default:
              $text = $step1_value = 'Unable to verify credentials for Twitter account, server returned error #'.$twitter->http_code;
              drupal_set_message($text);
              watchdog('Social Media Feeds', $text);
          break;
      }

      /* Check if there is a LinkedIn Response */
  } else if($_SESSION['laccess_oauth_token'] && $_SESSION['loauth_verifier'] == 'verified') {

          /* Remove verified session */
      unset($_SESSION['loauth_verifier']);

          /* Create LinkedIn Obj with api keys */
      $linkedin = new LinkedinOAuth(variable_get('socialmediafeeds_linkedin_apikey', ''), variable_get('socialmediafeeds_linkedin_secretkey', ''), $_SESSION['laccess_oauth_token'], $_SESSION['laccess_token_secret']);

          /* Request LinkedIn profile fields */
      $authRequest = $linkedin->get('v1/people/~:(id,first-name,last-name,picture-url,site-standard-profile-request:(url))');

          /* Check to see if http status returned successful */
      switch($linkedin->http_status) {
          case 200:
              $screen_name = $authRequest['first-name']." ".$authRequest['last-name'];	// Set username from linkedin profile
              $password = $_SESSION['laccess_oauth_token'] ."|". $_SESSION['laccess_token_secret'];	/* Set oauth token and token secret as password for  */
                  /* Check if profile picture exists; if so create html */
              if(!empty($authRequest['picture-url'])) {
                  $picture = '<img src="'.$authRequest['picture-url'].'" width="80" height="80" border="0" align="absmiddle"/> ';
              }

              $step1_value = '
                  <div class="title">Step #1: Select Account</div><div id="linkedin-accountdetails">'.$picture.'<a href="'.$authRequest['site-standard-profile-request']['url'].'" target="_blank">'.$screen_name.'</a> (<a href="https://www.linkedin.com/secure/login?session_full_logout=&trk=hb_signout">Logout</a>)</div>
                  <script>
                    jQuery(document).ready(function() {
                      replaceAccount("LinkedIn","'.$screen_name.'","'.$password.'");
                    });
                  </script>';
              $step2_value = '<div class="title">Step 2: Settings</div>';
              $step3_value = '<div class="title">Step 3: Add</div>';
              $hideContainer = '';
          break;

          default:
              $text = $step1_value = 'Unable to verify credentials for LinkedIn account, server returned error #'.$linkedin->http_status;
              drupal_set_message($text);
              watchdog('Social Media Feeds', $text);
          break;
      }

      //Initiate Twitter Response if none exists
  } else {

    $account_output = "";
    $module_path = drupal_get_path('module', 'socialmediafeeds');
    foreach ($serviceList as $key=>$service) {
      switch(strtolower($service)) {
        case "twitter":
          $account_output .= '<a href="/socialmediafeeds/'.strtolower($service).'_confirm/'.$account->uid.'/'.$admin.'"><img src="/'.$module_path.'/twitter/twitter_button.gif" alt="Login with Twitter" border="0" /></a>';
          break;
        case "facebook":
          $account_output .= '
              <fb:login-button length="long" size="medium" background="light" onlogin="userConnected();" autologutlink=�false�></fb:login-button>
              <script>
                  function initFacebook() {
                      //FB.init("'.variable_get('socialmediafeeds_facebook_apikey', '').'", "/'.$module_path.'/facebook/xd_receiver.htm", {"ifUserConnected":userConnected,"ifUserNotConnected":onNotConnected});
                      FB.init("'.variable_get('socialmediafeeds_facebook_apikey', '').'", "/'.$module_path.'/facebook/'.$fbReceiver.'");
                  }
                  jQuery(document).ready(function() {
                        initFacebook();
                  });
              </script>';
          break;
        case "linkedin":
          $account_output .= '<a href="/socialmediafeeds/'.strtolower($service).'_confirm/'.$account->uid.'/'.$admin.'"><div class="button-'.strtolower($service).'"></div></a>';
          break;

      }
    }
    $step1_value = '<div class="title">Step 1: Select Account</div>'.  $account_output;
    $step2_value = '<div class="title">Step 2: Settings</div>';
    $step3_value = '<div class="title">Step 3: Add</div>';
    $hideContainer = 'display:none;';
  }

  $form['newaccount']['step1'] = array(
      '#type'   => 'markup',
      '#prefix' => '<div id="step1" class="steps_container">',
      '#suffix' => '</div>',
      '#value'  => $step1_value,
  );

  $form['newaccount']['step2'] = array(
      '#type'   => 'markup',
      '#prefix' => '<div id="step2" class="steps_container" style="'.$hideContainer.'">',
      '#suffix' => '</div>',
      '#value'  => $step2_value,
  );

  $form['newaccount']['step2']['autopost'] = array(
      '#type' => 'select',
      '#title' => 'Auto update status',
      '#options'=> array('0'=>'Off', '1'=>'On'),
      '#description' => t('Update your Twitter/Facebook status automatically when making new posts'),
  );

  $form['newaccount']['step2']['placement'] = array(
      '#type' => 'select',
      '#title' => 'Status Feed Placement',
      '#description' => t('Location to place twitter/facebook status feed on your profile page'),
      '#options'=> array('none'=>'Off', 'left'=>'Left', 'right'=>'Right'),
  );

  $form['newaccount']['step2']['count'] = array(
      '#type' => 'select',
      '#title' => 'Number of Entries',
      '#description' => t('Selet how many status entries to show'),
      '#options'=> array('1','2','3','4','5','6','7','8','9','10'),
  );

  $form['newaccount']['step3'] = array(
      '#type'   => 'markup',
      '#prefix' => '<div id="step3" class="steps_container" style="'.$hideContainer.'">',
      '#suffix' => '</div>',
      '#value'  => $step3_value,
  );

  $form['newaccount']['step3']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add account'),
  );

  $form['newaccount']['step3']['cancel'] = array(
      '#type' => 'button',
      '#attributes' => array('onClick' => 'location.replace("'. referer_uri() .'"); return false;'),
      '#value' => t('Cancel'),
  );

  $form['newaccount']['fboverlay'] = array(
      '#type'   => 'markup',
      '#prefix' => '<div id="fboverlay" class="" style="display:none;">',
      '#suffix' => '</div>',
      '#value' => ' ',
  );


  return $form;
}



	/*
	*	validate add social media account
	*	
	*/
function socialmediafeeds_add_account_validate($form, &$form_state) {
  
  $verify = FALSE;

  $pass = $form_state['values']['password'];
  $screen_name = $form_state['values']['screen_name'];
  $service = $form_state['values']['service'];


	//Used to authenticate username/password for social network service
  
    $valid = socialmediafeeds_authenticate($screen_name, $pass, $service);
	
	switch($service) {
		
		case "Facebook":
			
			if (!$valid) {
				
				//$form_state['values']['facebook_auth']['#type'] = 'markup';
				
				form_set_error("Facebook-account", t('You need to authorize your Facebook account for this site'));
				
			} else {
				
				$form_state['values']['screen_name'] = $valid['screen_name'];
				$form_state['values']['password'] = $valid['sessionkey'];
			}
			
			break;
		
		case "LinkedIn":
			
			if (!$valid) {
				
				//$form_state['values']['facebook_auth']['#type'] = 'markup';
				
				form_set_error("LinkedIn-account", t('You need to authorize your LinkedIn account for this site'));
				
			} else {
				
				$form_state['values']['screen_name'] = $valid['screen_name'];
				$form_state['values']['password'] = $valid['sessionkey'];
			}
			
			break;
		
		case "Twitter":
			
			if (!$valid) {
			  form_set_error("password", t('Twitter authentication failed. Please check your account name and try again.'));
			} else {
				$form_state['values']['password'] = $valid['sessionkey'];//base64_encode($screen_name .':'. $pass);
			}
			
			break;
		
		

		
	}
		
}

	/*
	*	submit handler for add social media account
	*	
	*/
function socialmediafeeds_add_account_submit($form, &$form_state) {
	$admin = FALSE;	
	if($form_state['values']['globalacct']){
		$admin = TRUE;
	}
	if (!empty($form_state['values']['screen_name'])) {
		socialmediafeeds_user_save($form_state['values'], $admin);
	}
}

	/*
	*	
	*/
function socialmediafeeds_edit_account($form_state, $sid = FALSE) {
  if ($sid) {
    $redirect = (array) path_redirect_load($rid);
    drupal_set_title(check_plain($redirect['path']));
    $output = path_redirect_edit_form($form_state, $redirect);
  }
  else {
    drupal_set_title(t('Add redirect'));
    $output = path_redirect_edit_form($form_state);
  }
  return $output;
}



	/*
	*	Save account changes
	*/
function socialmediafeeds_user_save(&$account = array(), $admin = FALSE) {
	$account += array(
		'screen_name' => '',
	);
	$outputmsg = '';
	//echo $admin;
	
	if($admin){

		$acctExist = db_result(db_query("SELECT 1 FROM {socialmediafeeds_globalaccounts} WHERE screen_name = '%s' AND service = '%s'", $account['screen_name'], $account['service']));

		if ($acctExist) {
			drupal_write_record('socialmediafeeds_globalaccounts', $account, array('screen_name', 'service')); 
		} else {
			drupal_write_record('socialmediafeeds_globalaccounts', $account);
			$outputmsg = t('Account '. $account['screen_name'] .' on '. $account['service'] .' was created.');
		}
		
	} else {

		$exists = db_result(db_query("SELECT 1 FROM {socialmediafeeds_useraccounts} WHERE uid = %d AND screen_name = '%s' AND service = '%s'", $account['uid'], $account['screen_name'], $account['service']));
		if ($exists) {
			drupal_write_record('socialmediafeeds_useraccounts', $account, array('uid', 'screen_name', 'service')); 
		} else {
			drupal_write_record('socialmediafeeds_useraccounts', $account);
			$outputmsg = t('Account '. $account['screen_name'] .' on '. $account['service'] .' was created');
		}
	
	}
	drupal_set_message(t('Success...'). $outputmsg);
	
}


	/*
	*	Delete account
	*/
function socialmediafeeds_user_delete($uid, $service, $screen_name, $admin = FALSE) {
 
 	if($admin) {
		$sql = "DELETE FROM {socialmediafeeds_globalaccounts} WHERE service = '%s' AND screen_name = '%s'";	//Construct SQL Query for Delete
	 	$args = array($service, $screen_name);	//set arguments for SQL Query markers above
	} else {
 
	  $sql = "DELETE FROM {socialmediafeeds_useraccounts} WHERE uid = %d AND service = '%s' AND screen_name = '%s'";	//Construct SQL Query for Delete
	  $args = array($uid, $service, $screen_name);	//set arguments for SQL Query markers above
	}
  
  db_query($sql, $args);
  
  drupal_set_message("Removed ". $screen_name ." from accounts");
}


