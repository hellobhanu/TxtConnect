<?php
// $Id: socialmediafeeds.views_default.inc,v 0.5 2010/03/02 14:08:39 emuir Exp $

function socialmediafeeds_views_default_views() {
 // Exported view: social_media_status
  $view = new view;
  $view->name = 'Social_Media_Feeds';
  $view->description = 'Status Messages for Social Media Feeds module';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'users';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'screen_name' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'screen_name',
      'table' => 'socialmediafeeds_useraccounts',
      'field' => 'screen_name',
      'relationship' => 'none',
    ),
    'count' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'count',
      'table' => 'socialmediafeeds_useraccounts',
      'field' => 'count',
      'relationship' => 'none',
    ),
    'service' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 1,
        'text' => '[statusfeed [service]=[screen_name] count=[count] theme=default]',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'service',
      'table' => 'socialmediafeeds_useraccounts',
      'field' => 'service',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'name' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '%1\'s Status',
      'default_argument_type' => 'php',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'glossary' => 0,
      'limit' => '0',
      'case' => 'none',
      'path_case' => 'none',
      'transform_dash' => 1,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '$path = drupal_get_path_alias($_GET[\'q\']); //get alias from URL

$path = explode(\'/\', $path); //break path into an array

if ($path[0] != \'\') {

  return $path[0];

}',
      'validate_argument_node_type' => array(
        'blog' => 0,
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
        '2' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));

    //Create new view as Block for Left Region
  $handler = $view->new_display('block', 'SocialStatusFeeds Status Block:Left', 'socialmediafeeds_block_left');
  $handler->override_option('filters', array(
    'placement' => array(
      'operator' => '=',
      'value' => 'left',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'case' => 0,
      'id' => 'placement',
      'table' => 'socialmediafeeds_useraccounts',
      'field' => 'placement',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('block_description', 'SocialMediaFeeds:Left');
  $handler->override_option('block_caching', TRUE);

    //Create new view as Block for Right Region
  $handler = $view->new_display('block', 'SocialStatusFeeds Status Block:Right', 'socialmediafeeds_block_right');
  $handler->override_option('filters', array(
    'placement' => array(
      'operator' => '=',
      'value' => 'right',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'case' => 0,
      'id' => 'placement',
      'table' => 'socialmediafeeds_useraccounts',
      'field' => 'placement',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('block_description', 'SocialMediaFeeds:Right');
  $handler->override_option('block_caching', TRUE);

  //return array('tweets' => $view);
  $views[$view->name] = $view;
  return $views;
}
