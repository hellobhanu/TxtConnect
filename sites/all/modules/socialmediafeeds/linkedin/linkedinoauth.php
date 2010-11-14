<?php

  /* Load OAuth lib. You can find it at http://oauth.net */
$module_path = drupal_get_path('module', 'socialmediafeeds');
require_once($module_path .'/OAuth.php');

/**
 * LinkedIn OAuth class
 */
class LinkedinOAuth {
 
  /* Contains the last HTTP status code returned */
  public $http_status;
  /* Contains the last API call */
  public $last_api_call;
  /* Set up the base URL for API */
  public $host = "http://api.linkedin.com/";
  /* Set up the base URL for API */
  public $secure_base_url = "https://api.linkedin.com/";
  /* Set timeout default */
  public $timeout = 30;
  /* Set connect timeout */
  public $connecttimeout = 30; 
  /* Verify SSL Cert */
  public $ssl_verifypeer = FALSE;
  /* Respons format */
  public $format = 'xml';
  /* Decode returne json data */
  public $decode_json = TRUE;
  /* store authorization header */
  public $auth_header = array();
  /* Store last request string */
  public $last_request;
  

  /**
   * Set API URLS
   */
  function accessTokenURL()  { return $this->secure_base_url . 'uas/oauth/accessToken'; }
  function authenticateURL() { return $this->secure_base_url . 'uas/oauth/authenticate'; }
  function authorizeURL()    { return $this->secure_base_url . 'uas/oauth/authorize'; }
  function requestTokenURL() { return $this->secure_base_url . 'uas/oauth/requestToken'; }


  /**
   * Debug helpers
   */
  function lastStatusCode() { return $this->http_status; }
  function lastAPICall() { return $this->last_api_call; }

  /**
   * construct LinkedinOAuth object
   */
  function __construct($consumer_key, $consumer_secret, $oauth_token = NULL, $oauth_token_secret = NULL) {
    $this->signature_method = new OAuthSignatureMethod_HMAC_SHA1();
    $this->consumer = new OAuthConsumer($consumer_key, $consumer_secret);
    if (!empty($oauth_token) && !empty($oauth_token_secret)) {
      $this->token = new OAuthConsumer($oauth_token, $oauth_token_secret);
    } else {
      $this->token = NULL;
    }
  }

  /**
   * Get a request_token from Linkedin
   *
   * @returns a key/value array containing oauth_token and oauth_token_secret
   */
  function getRequestToken($oauth_callback = NULL) {
    $parameters = array();
    if (!empty($oauth_callback)) {
      $parameters['oauth_callback'] = $oauth_callback;
    } 
    $request = $this->oAuthRequest($this->requestTokenURL(), 'GET', $parameters);
    $token = OAuthUtil::parse_parameters($request);
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }

  /**
   * Get the authorize URL
   *
   * @returns a string
   */
  function getAuthorizeURL($token) {
    if (is_array($token)) {
      $token = $token['oauth_token'];
    }
    return $this->authorizeURL() . "?oauth_token={$token}";
  }

  /**
   * Exchange the request token and secret for an access token and
   * secret, to sign API calls.
   *
   * @returns array("oauth_token" => the access token,
   *                "oauth_token_secret" => the access secret)
   */
  function getAccessToken($oauth_verifier = FALSE) {
    $parameters = array();
    if (!empty($oauth_verifier)) {
      $parameters['oauth_verifier'] = $oauth_verifier;
    }
    $request = $this->oAuthRequest($this->accessTokenURL(), 'GET', $parameters);
    $token = OAuthUtil::parse_parameters($request);
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }


		/**
		*
		*  Provided by  Taylor Singletary @ http://developer.linkedin.com/thread/1439
		*/
	function getProfile($resource = "~") {
		$profile_url = $this->host . "/v1/people/" . $resource;
		$request = OAuthRequest::from_consumer_and_token($this->consumer, $this->access_token, "GET", $profile_url);
		$request->sign_request($this->signature_method, $this->consumer, $this->access_token);
		$this->auth_header = $request->to_header("https://api.linkedin.com"); # this is the realm
		# This PHP library doesn't generate the header correctly when a realm is not specified.
		# Make sure there is a space and not a comma after OAuth
		// $auth_header = preg_replace("/Authorization\: OAuth\,/", "Authorization: OAuth ", $auth_header);
		// # Make sure there is a space between OAuth attribute
		// $auth_header = preg_replace('/\"\,/', '", ', $auth_header);
		if ($debug) {
			echo $auth_header;
		}
		// $response will now hold the XML document
		$response = $this->get($profile_url, $auth_header);
		return $response;
	}
	
		/**
		*
		*  Provided by  Taylor Singletary @ http://developer.linkedin.com/thread/1439
		*/
	function setStatus1($status) {
   
    if (strrpos($url, 'https://') !== 0 && strrpos($url, 'http://') !== 0) {
      $url = $this->host . "v1/people/~/current-status";//"{$this->host}{$url}";
    }
    $xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"><current-status>" . htmlspecialchars($status, ENT_NOQUOTES, "UTF-8") . "</current-status>";
    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, "PUT", $url, array($status));
    $request->sign_request($this->signature_method, $this->consumer, $this->token);
    //$auth_header = $request->to_header("https://api.linkedin.com", "Content-Type: text/xml;charset=utf-8");
    return $this->http($request->get_normalized_http_url(), "PUT", $request->to_postdata());
  }
  
  /**
	 * Recursive function to transform xml to a workable array
	 * @param $input simplexml_load_string('xml_string')
	 * @return array
	 */
	function XMLtoArray($obj) {
		$obj = is_string($obj) ? simplexml_load_string($obj) : $obj;
		$_arr = is_object($obj) ? get_object_vars($obj) : $obj;
		
		if(!empty($_arr)) {
			foreach ($_arr as $key => $val) {
				$val = (is_array($val) || is_object($val)) ? $this->XMLtoArray($val) : $val;
				$arr[$key] = $val;
			}
		
			return $arr;
		}
	}

  /**
   * GET wrappwer for oAuthRequest.
   */
  function get($url, $parameters = array()) {
    $response = $this->oAuthRequest($url, 'GET', $parameters);
    if ($this->format === 'json' && $this->decode_json) {
      return json_decode($response);
    }
    
    if ($this->format === 'xml') {
      return $this->XMLtoArray($response);
    }
    return $response;
  }
  
  /**
   * POST wreapper for oAuthRequest.
   */
  function post($url, $parameters = array()) {
    $response = $this->oAuthRequest($url, 'POST', $parameters);
    if ($this->format === 'json' && $this->decode_json) {
      return json_decode($response);
    }
    return $response;
  }
  
  /**
   * POST wreapper for oAuthRequest.
   */
  function put($url, $parameters = array(), $body = NULL) {
    $response = $this->oAuthRequest($url, 'PUT', $parameters, $body);
    if ($this->format === 'json' && $this->decode_json) {
      return json_decode($response);
    }
    if ($this->format === 'xml') {
      return $this->XMLtoArray($response);
    }
    return $response;
  }
  

  /**
   * DELTE wrapper for oAuthReqeust.
   */
  function delete($url, $parameters = array()) {
    $response = $this->oAuthRequest($url, 'DELETE', $parameters);
    if ($this->format === 'json' && $this->decode_json) {
      return json_decode($response);
    }
    return $response;
  }

  /**
   * Format and sign an OAuth / API request
   */
  function oAuthRequest($url, $method, $parameters, $body = NULL) {
    if (strrpos($url, 'https://') !== 0 && strrpos($url, 'http://') !== 0) {
      $url = "{$this->host}{$url}";
    } else if($method == 'PUT'){
    	$url = "{$this->secure_base_url}{$url}";
    }
    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, $method, $url, $parameters);
    $request->sign_request($this->signature_method, $this->consumer, $this->token);
    $this->auth_header = $request->to_header();
    switch ($method) {
    case 'GET':
      return $this->http($request->to_url(), 'GET');
    case 'PUT':
    	return $this->http($request->to_url(), $method, $request->to_postdata(), $body);
    default:
      return $this->http($request->get_normalized_http_url(), $method, $request->to_postdata());
    }
  }

  /**
   * Make an HTTP request
   *
   * @return API results
   */
  function http($url, $method, $postfields = NULL, $body = NULL, $auth_header = "") {
   
		$ci = curl_init();
			/* Curl settings */
		//curl_setopt($ci, CURLOPT_CONNECTTIMEOUT, $this->connecttimeout);
		//curl_setopt($ci, CURLOPT_TIMEOUT, $this->timeout);
		curl_setopt($ci, CURLOPT_URL, $url);
		curl_setopt($ci, CURLOPT_HEADER, 0);
		curl_setopt($ci, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ci, CURLOPT_HTTPHEADER, array($auth_header));
        //curl_setopt($ci, CURLOPT_SSL_VERIFYPEER, $this->ssl_verifypeer);
    
		switch ($method) {
			case 'POST':
				curl_setopt($ci, CURLOPT_POST, TRUE);
				if (!empty($postfields)) {
					curl_setopt($ci, CURLOPT_POSTFIELDS, $postfields);
				}
				break;
			case 'DELETE':
				curl_setopt($ci, CURLOPT_CUSTOMREQUEST, 'DELETE');
				if (!empty($postfields)) {
					$url = "{$url}?{$postfields}";
				}
		}
		
		if ($body) {
			//array_push ($this->auth_header, "Content-Type: text/xml;charset=utf-8");
			curl_setopt($ci, CURLOPT_POST, 1);
			curl_setopt($ci, CURLOPT_POSTFIELDS, $body);
			curl_setopt($ci, CURLOPT_CUSTOMREQUEST, $method);
			curl_setopt($ci, CURLOPT_HTTPHEADER, array($auth_header, "Content-Type: text/xml;charset=utf-8")); 
    }	
		$response = curl_exec($ci);
		$this->http_status = curl_getinfo($ci, CURLINFO_HTTP_CODE);
		$this->last_api_call = $url;
		curl_close ($ci);
		return $response;
  }

  
  function setStatus($status) {
    $status_url = $this->secure_base_url . "v1/people/~/current-status";
    //echo "Setting status...\n";
    $xml = "<current-status>" . htmlspecialchars($status, ENT_NOQUOTES, "UTF-8") . "</current-status>";
    //echo $xml . "\n";
    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, "PUT", $status_url);
    $request->sign_request($this->signature_method, $this->consumer, $this->token);
    $auth_header = $request->to_header();
    if ($debug) {
      echo $auth_header . "\n";
    }
    //$response = $this->httpRequest($status_url, $auth_header, "PUT", $xml);
    $response = $this->http($status_url, "PUT", NULL, $xml, $auth_header);
    return $response;
  }
  
  
  function httpRequest($url, $auth_header, $method, $body = NULL) {
    if (!$method) {
      $method = "GET";
    };

    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $url);
    curl_setopt($curl, CURLOPT_HEADER, 0);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($curl, CURLOPT_HTTPHEADER, array($auth_header)); // Set the headers.

    if ($body) {
      curl_setopt($curl, CURLOPT_POST, 1);
      curl_setopt($curl, CURLOPT_POSTFIELDS, $body);
      curl_setopt($curl, CURLOPT_CUSTOMREQUEST, $method);
      curl_setopt($curl, CURLOPT_HTTPHEADER, array($auth_header, "Content-Type: text/xml;charset=utf-8"));   
    }

    $data = curl_exec($curl);
    if ($this->debug) {
      echo $data . "\n";
    }
    //$this->auth_header = curl_getinfo($curl, CURLINFO_HEADER);
    $this->http_status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
    $this->last_api_call = $url;
    curl_close($curl);
    return $data; 
  }

}

