<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
	<title><?php print $head_title ?></title>
	<?php print $head ?>
	<?php print $styles ?>
	<?php print $scripts ?>
	<!--[if lt IE 7]>
	  <?php print phptemplate_get_ie_styles(); ?>
	<![endif]-->
</head>
 
<body<?php print phptemplate_body_class($left, $right); ?>>
  <div id="page">
	<div id="header">
		<div id="header-region" class="clear-block">
          
          <?php if ($site_name): ?>
			<div id="site_name"><a href="<?php print check_url($front_page) ?>"><?php print $site_name ?></a></div>
          <?php endif; ?>
          <?php if ($site_slogan): ?>
			<div class="slogan"><?php print $site_slogan ?></div>
          <?php endif; ?>

          <?php
			/*if($login_area) {
				print $login_area;
			}*/
			print $header;
          ?>
		</div>
			
		<div id="header-bottom" class="clear-block">
        <?php print $header_bottom; ?>
		</div>
	</div> <!-- /header -->
		
    <div id="content-container" class="clear-block">
			
		<div id="column-container">
		  <div id="top-column">
		  	<?php print $breadcrumb; ?>
		  </div>
		  
		  <?php if ($left): ?>
			<div id="sidebar-left" class="sidebar"><?php print $left ?></div>
		  <?php endif; ?>

		  <div id="center">
			  
		  	<?php
              if($node->type != 'project') {
                if ($title): print '<h1'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h1>'; endif;
              }
            ?>
			<?php if ($mission): print '<div id="mission">'. $mission .'</div>'; endif; ?>
			<?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?> 
			
			<?php if ($tabs): print '<ul class="tabs primary">'. $tabs .'</ul></div>'; endif; ?>
			<?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
			<?php if ($show_messages && $messages): print $messages; endif; ?>
			<?php print $help; ?>
			  <div class="clear-block">
				<?php print $content ?>
			  </div>
			  
		  </div> <!-- /#squeeze, /#center -->

		  <?php if ($right): ?>
			<div id="sidebar-right" class="sidebar">
			<?php print $right ?>
			</div>
		  <?php endif; ?>
		</div><!-- /column container-->
			
  </div><!-- /page -->
  
	<!-- Footer -->
  	<div id="footer"><?php print $footer_message . $footer ?></div>
	<!-- /Footer-->
    </div>
  <?php print $closure ?>

  </body>
</html>
