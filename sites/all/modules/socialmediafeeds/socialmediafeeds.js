 // JavaScript Document

	/*
		Pull in twitterfeed for account
	
		Reference: 
		- http://www.sitepoint.com/blogs/2009/04/21/make-your-own-web-site-badges-with-jquery-and-json/
		- http://ralphwhitbeck.com/2007/11/20/PullingTwitterUpdatesWithJSONAndJQuery.aspx
	*/

$(document).ready(function() {
	//$("#edit-facebookauth").hide();	
	
	/*
	checkNewAccountDropdown();
	$("#edit-service").change(function() {
		checkNewAccountDropdown();
	});
	*/
	
	hideUserPass();	//Hide username and password fields
	
	
	$('#socialmediafeeds-useredit-account-list .form-item').each(function(){
		$(this).bind("change", function(){
			showWarning();
		});
	});
	
});


function showWarning(){
	if(!$('#form-warning').length) {
		$('#edit-buttons-submit').before('<div id="form-warning" class="warning">Changes made to the form will <b>NOT</b> be saved until the "Save Changes" button is clicked.</div>');
	}
}

function hideUserPass() {
	$('#edit-screen-name-wrapper').hide();
	$('#edit-password-wrapper').hide();
	$('#edit-service-wrapper').hide();
	//$("#edit-submit").attr('disabled', 'disabled');
}

function checkNewAccountDropdown(){
	if($("#edit-service option:selected").text() == "Facebook"){
			$("#edit-submit").attr('disabled', 'disabled');
			$('#edit-screen-name-wrapper').hide();
			$('#edit-password-wrapper').hide();
			$("#edit-twitterauth").hide();
			$("#edit-facebookauth").show();
			initFacebook();
	} else if ($("#edit-service option:selected").text() == "Twitter") {
			$("#edit-submit").attr('disabled', 'disabled');
			$('#edit-screen-name-wrapper').hide();
			$('#edit-password-wrapper').hide();
			$("#edit-facebookauth").hide();
			$("#edit-twitterauth").show();
	} else if ($("#edit-service option:selected").text() == "LinkedIn") {
			$("#edit-submit").attr('disabled', 'disabled');
			$('#edit-screen-name-wrapper').hide();
			$('#edit-password-wrapper').hide();
			$("#edit-facebookauth").hide();
			$("#edit-twitterauth").hide();
	} else {
			$("#edit-facebookauth").hide();
			$('#edit-screen-name-wrapper').show();
			$('#edit-password-wrapper').show();
			$("#edit-screen-name").val('');
	  	$("#edit-password").val('');
			//$('#edit-screen-name').removeAttr('disabled');
			//$('#edit-password').removeAttr('disabled');
			$("#edit-submit").removeAttr('disabled');
			$("#edit-submit").show();
	}
}

	/*
	* Fills in hidden form fields for socialmedia account
	* 
	* @param username
	* 	username to save
	*
	* @param authocde 
	* 	Code provided by OAUTH authorzation 
	*/
function replaceAccount(service, username, authcode) {
	$('#edit-service').val(service);						//Change selection of service dropdown
	$("#edit-screen-name").val(username);				// Set username field
	$("#edit-password").val(authcode);					// Set password field using authcodes
	$("#edit-submit").show();										// Show submit button
	$("#edit-submit").focus();									// Set Submit button focus
	$("#edit-submit").removeAttr('disabled');		// Re-enable submit button
}

	/*
	* Runs if user is already connected to Facebook
	*/
function userConnected() {
	
	$('#edit-service').val('Facebook');	//Change selection of service dropdown
	$('#fboverlay')
		.html('Facebook Loading')
		.css("margin-left", "-"+($('#fboverlay').width()/2)+ "px")
		.show();
	var usr = new Array(1);
	var usrInfoArray = new Array("name");
	var perms;
	var resulthtml;
	var resulthtml1;
	
	usr[0] = FB.Facebook.apiClient.get_session().uid;	//Get facebook session key for logged in user
	skey = FB.Facebook.apiClient.get_session().session_key;	//get session key
			//Get user information passing uid and session key
	FB.Facebook.apiClient.users_getInfo(usr, usrInfoArray, function(info){
			//var screename = document.getElementById("edit-screen-name");
			//screename.value = info[0]["name"];																
		$("#edit-screen-name").val(info[0]["name"]);
		$("#edit-password").val(skey);
			//$("#edit-facebookauth").append('<img src="'+ info[0]["profile-pic"] +'"><div class="">'+ info[0]["name"] +'</div>');
	});
		
		
	FB.ensureInit(function() {
    checkPermissions();
  });
		
}

					  
function onNotConnected(){
	  $("#fb_account").html('');
	  $("#fb_perms").html('');
	  $("#step1a").remove();
	  $("#step2").hide();
	  $("#step3").hide();
	  $("#edit-submit").attr('disabled', 'disabled');
	  $("#edit-screen-name").val('');
	  $("#edit-password").val('');
	  //var user[0] = FB.Facebook.apiClient.get_session().uid;
	  //var authApp = FB.Facebook.apiClient.users_hasAppPermission("offline_access,status_update",user);
}


function afterAppAuth() {
	 /*if(result.session) {
		$("#edit-password").val(result.session["session_key"]);
	 }*/
	 $('#edit-service').val('Facebook');	//Change selection of service dropdown
	 
	var permstatus = false;
	var permsg = "";
	
	$('#fboverlay')
		.html('Facebook Loading')
		.css("margin-left", "-"+($('#fboverlay').width()/2)+ "px")
		.show();
	
	
	FB.ensureInit(function() {
    checkPermissions();
  });
	
}
  
  
function checkPermissions(){
	//$("#step1").after("<div>test</div>");
	var permsg = "";
	var perms_output = "";
	var validPerms = false;
	//var result;
	//var result1;
	
	
		FB.Facebook.apiClient.users_hasAppPermission("offline_access", function(result) {
			FB.Facebook.apiClient.users_hasAppPermission("publish_stream", function(result1) {
		
				if (result == 0) {
					permsg += " Offline Access";
				}
		
				if (result1 == 0) {
					if(permsg != "") {
						permsg += " and Publish Stream";
					} else {
						permsg = " Publish Stream";
					}
				}
		
		
		
				// prompt offline permission
				if (result == 0 || result1 == 0) {
					if(jQuery('#step1a').length) {
						jQuery("#step1a").html('<div class="title">Step 2: Permissions</div><div id="grant-permissions"><fb:prompt-permission perms="offline_access, publish_stream" next_fbjs="afterAppAuth()" class="btn">Grant permissions for '+permsg+'</fb:prompt-permission></div>');
					} else {
						jQuery("#step1").after('<div id="step1a" class="steps_container"><div class="title">Step 2: Permissions</div><div id="grant-permissions"><fb:prompt-permission perms="offline_access, publish_stream" next_fbjs="afterAppAuth()" class="btn">Grant permissions for '+permsg+'</fb:prompt-permission></div></div>');
					}
					jQuery("#step2 .title").html('Step 3: Settings');
					jQuery("#step3 .title").html('Step 4: Add');
					
					FB.XFBML.Host.parseDomTree();
					jQuery('#fboverlay').html('').hide();
				} else {
					if(jQuery('#step1a').length) {
						jQuery("#step1a").html('<div class="title">Step 2: Permissions</div><div id="grant-permissions">Permissions granted</div>');
					} else {
						jQuery("#step1").after('<div id="step1a" class="steps_container"><div class="title">Step 2: Permissions</div><div id="grant-permissions">Permissions granted</div></div>');
					}
					jQuery("#step2 .title").html('Step 3: Settings');
					jQuery("#step3 .title").html('Step 4: Add');
					jQuery("#step2").show();
					jQuery("#step3").show();
					jQuery("#edit-submit").removeAttr('disabled');
					FB.XFBML.Host.parseDomTree();
					jQuery('#fboverlay').html('').hide();
				}
			});
		});
		
		facebookprofile = '<div class="title">Step 1: Account</div><fb:profile-pic uid="loggedinuser" ></fb:profile-pic> <fb:name uid="loggedinuser" useyou="false"></fb:name><fb:login-button length="long" size="medium" background="light" onlogin="userConnected();" autologoutlink="true"></fb:login-button><br/>';
		jQuery("#step1").html(facebookprofile);
		
		
		/*FB.Facebook.apiClient.users_hasAppPermission("offline_access", function(result) {
			
				//First check if permission granted, otherwise assume permission not granted
			if (result) {
				perms_output += 'Offline Access Granted <br/>';
				validPerms = true;
				//"Offline Access";
					//permsg += " Offline Access";
			} else {
				
				perms_output += '<fb:prompt-permission perms="offline_access" next_fbjs="afterAppAuth()" class="btn">Grant permissions for Offline Access</fb:prompt-permission>';
				
			}
			
		});
		
		FB.Facebook.apiClient.users_hasAppPermission("publish_stream", function(result1) {
		
				if (result1) {
					perms_output += 'Publish Status Granted';
					validPerms = true;
				} else {
					perms_output += '<fb:prompt-permission perms="publish_stream" next_fbjs="afterAppAuth()" class="btn">Grant permissions for Publish Stream</fb:prompt-permission>';
				}
		
		});
		
		//jQuery("#step1").after("<div>test1</div>");
		
		// prompt offline permission
		if (!validPerms) {
					
					jQuery("#step2").before('<div id="step1a" class="steps_container"><div class="title">Step 2: Permissions</div><div id="grant-permissions">'+perms_output+'</div></div>');
					jQuery("#step2 .title").html('Step 3: Settings');
					jQuery("#step3 .title").html('Step 4: Add');
					//jQuery("#step2").hide();
					FB.XFBML.Host.parseDomTree();
					jQuery('#fboverlay').html('').hide();
		} else {
					jQuery("#step2").before('<div id="step1a" class="steps_container"><div class="title">Step 2: Permissions</div><div id="grant-permissions">Permissions granted</div></div>');
					
					jQuery("#step2 .title").html('Step 3: Settings');
					jQuery("#step3 .title").html('Step 4: Add');
					jQuery("#step2").show();
					jQuery("#step3").show();
					jQuery("#edit-submit").removeAttr('disabled');
					FB.XFBML.Host.parseDomTree();
					jQuery('#fboverlay').html('').hide();
		}
		
		
		facebookprofile = '<div class="title">Step 1: Account</div><fb:profile-pic uid="loggedinuser" ></fb:profile-pic> <fb:name uid="loggedinuser" useyou="false"></fb:name><fb:login-button length="long" size="medium" background="light" onlogin="userConnected();" autologoutlink="true"></fb:login-button><br/>';
		jQuery("#step1").html(facebookprofile);
		
		//resulthtml = '<div id="grant-permissions"><fb:prompt-permission perms="offline_access, publish_stream" next_fbjs="afterAppAuth()">Grant permission for status updates </fb:prompt-permission></div></div><br/>'
		
		//jQuery("#fb_perms").html(resulthtml);
		/*jQuery("#step1").after('<div id="step1a" class="steps_container"><div class="title">Step 2: Permissions</div><div id="grant-permissions">'+perms_output+'</div></div></div>');
		jQuery("#step2 .title").html('Step 3: Settings');
		jQuery("#step3 .title").html('Step 4: Add');
		
		if (result || result1) {
			jQuery("#step2").show();
			jQuery("#step3").show();
			jQuery("#edit-submit").removeAttr('disabled');
			jQuery('#fboverlay').html('').hide();
		}
		*/
		FB.XFBML.Host.parseDomTree();
	
} 

function twitterfeed(name, cnt, skin){
	if(!cnt){cnt = 5};
	if(!name){return "no username provided"};
	if(!skin) { skin="default" };
	
	//var tweeturl = feedurl+"?count="+cnt+"&callback=?";
	jQuery.getJSON('http://twitter.com/statuses/user_timeline.json',{screen_name:name, count:cnt}, function(data){
		
		
		jQuery('#twitterholder').attr("class", skin);
			//Display twitter profile image
		jQuery('<img class="userpic">')
			.prependTo('#twitterholder');
		
			//Cycle through most recent twitter feeds.
		jQuery.each(data, function(i, item) {
		var txt = item.text.replace(
		  /(https?:\/\/[-a-z0-9._~:\/?#@!jQuery&\'()*+,;=%]+)/ig,
		  '<a href="jQuery1">jQuery1</a>'
		).replace(
		  /@+([_A-Za-z0-9-]+)/ig, 
		  '<a href="http://twitter.com/jQuery1">@jQuery1</a>'
		).replace(
		  /#+([_A-Za-z0-9-]+)/ig,
		  '<a href="http://search.twitter.com/search?q=jQuery1">#jQuery1</a>'
		);
		jQuery('.userpic').attr("src", item.user["profile_image_url"]);
		jQuery('<p></p>')
			.addClass(i%2 ? 'even' : 'odd')
			.html(txt 
				  + '<span class="created">'+ relative_time(item.created_at) 
                  + ' via '  
                  + item.source +"</span")
			.appendTo('#twitterholder');
		});
	});
}

	/*
		Copyright Source: 
		- http://ralphwhitbeck.com/2007/11/20/PullingTwitterUpdatesWithJSONAndJQuery.aspx
	*/
function relative_time(time_value) {
	var values = time_value.split(" ");
	time_value = values[1] + " " + values[2] + ", " + values[5] + " " + values[3];
	var parsed_date = Date.parse(time_value);
	var relative_to = (arguments.length > 1) ? arguments[1] : new Date();
	var delta = parseInt((relative_to.getTime() - parsed_date) / 1000);
	delta = delta + (relative_to.getTimezoneOffset() * 60);
	
	var r = '';
	
	if (delta < 60) {
		r = 'a minute ago';
	} else if(delta < 120) {
		r = 'couple of minutes ago';
	} else if(delta < (45*60)) {
		r = (parseInt(delta / 60)).toString() + ' minutes ago';
	} else if(delta < (90*60)) {
		r = 'an hour ago';
	} else if(delta < (24*60*60)) {
		r = '' + (parseInt(delta / 3600)).toString() + ' hours ago';
	} else if(delta < (48*60*60)) {
	r = '1 day ago';
	} else {
	r = (parseInt(delta / 86400)).toString() + ' days ago';
	}
	
	return r;
} 
