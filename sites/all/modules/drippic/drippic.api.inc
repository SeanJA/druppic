<?php

/**
 * Page callback for drippic/upload to use Twitter basic auth
 *
 * @param $function
 *   Other functions to call within the page callback.
 *   tweet - Sends the post to Twitter.
 *
 * @return
 *   XML response of successful or failed upload.
 */


/**
 * Page callback for drippic2/upload to use Twitter oauth echo
 * The most common method to upload images,
 * supported by 'Twitter for iPhone', and Twitteriffic.
 * Used on Drippic.com by Spaz WebOS app, and Drippic uploader iPhone app.
 *
 * @param $function
 *    tweet - Sends the post to Twitter.
 *    null  - Doesn't post to Twitter, allowing third party to do it.
 *
 * @param $format
 *   xml or json.
 *   xml is the default, json was a request for the drippic uploader iphone app.
 *
 * @return
 *   response of successful or failed upload.
 */
function drippic_two_upload($function = null, $format = 'xml'){
  // Verify user using oauth echo http://dev.twitter.com/pages/oauth_echo
  // Currently uses CURL although could/should be replaced by drupal_http_request
  $sp = $_SERVER['HTTP_X_AUTH_SERVICE_PROVIDER'];
  $vc_auth = $_SERVER['HTTP_X_VERIFY_CREDENTIALS_AUTHORIZATION'];
  
  if(!isset($sp) && !isset($vc_auth)){
    header('HTTP/1.1 401 Unauthorized');
    return 'HTTP_X_AUTH_SERVICE_PROVIDER & HTTP_X_VERIFY_CREDENTIALS_AUTHORIZATION not set';
  }
  
  $curl = curl_init();
  curl_setopt($curl, CURLOPT_URL, $sp);
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($curl, CURLOPT_FAILONERROR, false);
  curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, FALSE);

  // Set our OAuth Echo headers
  curl_setopt($curl, CURLOPT_HTTPHEADER, array(
          'Authorization: ' . $vc_auth,
          'X-Auth-Service-Provider: ' . $sp
          ));

  $response = curl_exec($curl);
  if (!$response) {
    $response = curl_error($curl);
  }
  curl_close($curl);
  $response_obj = json_decode($response);
  watchdog('drippic','Twitter response: </pre>'.print_r($response_obj,true).'</pre>');
  
  if(isset($response_obj->error)){
    header('HTTP/1.1 401 Unauthorized');
    return 'Twitter error: ' . $response_obj->error;
  }
  else{
  	$twitter_user = $response_obj;
  }
  
	$tweet = $_POST['message'];
	$media = $_FILES['media'];
	if($media['error'] != 0){
    header('HTTP/1.1 401 Unauthorized');
    return 'File error code: ' . $media['error'];
  }
	watchdog('drippic','media: </pre>'.print_r($media,true).'</pre>');

  // Check user rules
	if(drupal_is_denied('user',$twitter_user->screen_name)){
    header('HTTP/1.1 401 Unauthorized');
    return 'Access denied';
  }
  
	$output = '<?xml version="1.0" encoding="UTF-8"?>';
	$response = array(
		'key' => 'rsp',
	);

	if(isset($twitter_user->screen_name) && isset($media)){
 	  $account = user_load(array('name' => $twitter_user->screen_name));
 	  // Check is user exists
    if($account->name == $twitter_user->screen_name){
      // Check is user is blocked
      if($account->status == 0){
        header('HTTP/1.1 401 Unauthorized');
        return 'User blocked';
      }
      // Update drupal user details to stay in sync with Twitter.
      $edit = array(
        'name' => $twitter_user->screen_name,
        'pass' => user_password(),
        'init' => $twitter_user->screen_name,
        "authname_twitter" => $twitter_user->id,
        'access' => time(),
        'twitter' => $twitter_user,
      );
      user_save($account,$edit);
      // Log the user into drupal
      $form_values = array();
      $form_values["name"] = $edit['name'];
      $form_values["pass"] = $edit['pass'];
      $form_values["op"] = "Log in";
      $account = user_authenticate($form_values);
    }
    else{
      //The user did not exist and needs to be created
      $userinfo = array(
        'name' => $twitter_user->screen_name,
        'pass' => user_password(),
        'init' => $twitter_user->screen_name,
        'status' => 1,
        'access' => time(),
        'twitter' => $twitter_user,
      );
      $account = user_save('', $userinfo);
      // The newly created user can now be logged into drupal
      $form_values = array();
      $form_values["name"] = $name;
      $form_values["pass"] = $pass;
      $form_values["op"] = "Log in";
      $account = user_authenticate($form_values);
    }
    // Create the node
		$nid = drippic_create_node($tweet ,$media, $account);
		// Check the node was given an nid / was created properly
		if(isset($nid)){
		  // Check is Drippic should tweet or not
			if($function == 'tweet'){
			  // Create a short URL
			  $url = drippic_shorten('node/' . $nid);
			  // Make sure the post + url is short enough for Twitter, and shorten if needed
				$post = $tweet . ' ' . $url;
				while(strlen($post) > 140){
					$tweet = substr($tweet, 0, -4);
					$post = $tweet . '... ' . $url;
				}
				// Post to twitter using the twitter module
				module_load_include('inc', 'twitter');
      	$values = db_fetch_array(db_query("SELECT twitter_uid FROM {twitter_account} WHERE uid = %d", $account->uid)); 
        $twitter_account = twitter_account_load($values['twitter_uid']);
         try {
         	$result = twitter_set_status($twitter_account, $post);
         	watchdog('drippic',t('Successfully posted to Twitter'));
         	}
         	catch (TwitterException $e) {
         	watchdog('drippic',t('An error occurred when posting to twitter: %code %error',
         	array('%code' => $result->code, '%error' => $result->error)), 'warning');
         	}
			}

			$response['attributes'] = array(
				'stat' => 'ok',
			);
			// return the node id and the short url
			$response['value'] = array(
				'mediaid' => $nid,
				'mediaurl' => drippic_shorten('node/' . $nid),
			);
		}
		else{
		  // return an error is no nid was set
			$response['attributes'] = array(
				'stat' => 'fail',
			);
			$response['value'] = array(
				'error' => 'No nid returned',
			);
		}
	}
	else{
	  // return an error if $twitter_user->screen_name and $media were not set
		$response['attributes'] = array(
			'stat' => 'fail',
		);
		$response['value'] = array(
			'error' => 'Make sure username and media vars are sent.',
		);
	}
	$output .= format_xml_elements(array($response));
	
	// return as xml or json
	if($format == 'xml'){
	  header ("Content-Type:text/xml");  
	  print $output;
  }
  elseif($format =='json'){
    header('Content-type: application/json');
    $json = array();
    $json['url'] = $response['value']['mediaurl'];
    print json_encode($json);
  }
 
}

/**
 * Page callback for drippic/show/%/%
 *
 * @param $size
 *   an imagecache size.
 *
 * @param $code
 *   the shorturl code for an image.
 *
 */
function drippic_show($size,$code){
  $indx = shorturl_decode_url($code);
  $orig_url = db_result(db_query('SELECT orig_url FROM {shorturl_link} WHERE lid=%d ', $indx));

    $re1='.*?';	# Non-greedy match on filler
    $re2='(\\d+)';	# Integer Number 1

    if ($c=preg_match_all ("/".$re1.$re2."/is", $orig_url, $matches))
    {
        $int1=$matches[1][0];
    }


  $node = node_load($int1);
  $image = imagecache_create_url($size, $node->field_pic[0]['filepath']);
  drupal_goto($image);
}

/**
 * Page callback for photos/%/%
 *
 * @param $username
 *   the drupal / twitter username for a user.
 *
 * @param $format
 *   the format to return image list.
 *   json currently the only option.
 *
 * @return
 *   list of user's nodes in json.
 *
 */
function drippic_user_photos($username,$format){
  
  $account = user_load(array('name'=>$username));
  $nodes = array();
  $query = db_query('SELECT nid FROM {node} WHERE uid = %d',$account->uid);
  while ($node = db_fetch_object($query)) {
    $nodes[] = node_load($node->nid);
  }
  
  if($format == 'json'){
    header("Content-type: application/json; charset=utf-8");
    print json_encode($nodes);
  }
}
