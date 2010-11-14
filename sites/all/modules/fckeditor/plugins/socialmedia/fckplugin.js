// Our method which is called during initialization of the toolbar.



// Register the command.
//FCKCommands.RegisterCommand('socialmedia', new socialmedia());
//FCKCommands.RegisterCommand( 'statusfeed', new FCKDialogCommand( "Status Feed", "Status Feed", FCKConfig.PluginsPath + 'socialmedia/statusfeed.html', 450, 350 ) ) ;
FCKCommands.RegisterCommand( 'badges', new FCKDialogCommand( "Social Networking Badges", "Social Networking Badges", FCKConfig.PluginsPath + 'socialmedia/badges.html', 450, 350 ) ) ;

// Add the button.
/*var item = new FCKToolbarButton('statusfeed', 'Status Feed');
item.IconPath = FCKConfig.PluginsPath + 'socialmedia/statusfeeds.png';
FCKToolbarItems.RegisterItem('twitterfeed', item);
*/
var itemBadges = new FCKToolbarButton('badges', 'Follow Me Badges');
itemBadges.IconPath = FCKConfig.PluginsPath + 'socialmedia/followme_badge.png';
FCKToolbarItems.RegisterItem('badges', itemBadges);