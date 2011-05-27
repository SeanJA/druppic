<?php
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="description" content=""/>
<meta name="keywords" content=""/>

<?php global $base_url;?>
<title><?php print $head_title ?></title>
<?php print $head ?>
<?php print $styles ?>
<link href="/sites/all/themes/journalcrunch_im/css/jquery.fancybox-1.3.4.css" media="all" rel="stylesheet" type="text/css">
<?php print $scripts ?>
</head>
<body>

<div id="mainWrapper">

    <!-- Header. -->
    <div id="wrapper">

    <!-- Header. -->
    <div id="header">
    
        <div id="logo">
			<?php
            // Prepare header
            $site_fields = array();
            if ($site_name) {
            $site_fields[] = check_plain($site_name);
            }
            if ($site_slogan) {
            $site_fields[] = check_plain($site_slogan);
            }
            $site_title = implode(' ', $site_fields);
            if ($site_fields) {
            $site_fields[0] = '<div>'. $site_fields[0] .'</div>';
            }
            $site_html = implode(' ', $site_fields);
            
            if ($logo || $site_title) {
            print '<a href="'. check_url($front_page) .'" title="'. $site_title .'">';
            if ($logo) {
            print '<img src="'. check_url($logo) .'" alt="'. $site_title .'" height="80" width="108" />';
            }
            print $site_html .'</a>';
            }
            ?>
        </div>
        
        <div id="topMenu">
           
        <?php if (isset($primary_links)) : ?>
          <?php print theme('links', $primary_links, array('class' => 'links primary-links')) ?>
        <?php endif; ?>
            
        </div>
        
        <div id="topSearch">
		<?php print $search_box; ?>
        </div>
        
        <div id="topSocial">
            
            <ul>									
            <li><a class="twitter tip" href="http://twitter.com/SeanJA" title="Follow Me on Twitter!"></a></li>
            <li><a href="/rss.xml" class="rss" title="Subcribe to the RSS Feed"></a></li>
            </ul>
        
        </div>
		
    
    </div><!-- EOF: #header -->
    
	<!-- Content. -->
    <div id="content">
    
		<?php /*if ($is_front) {
            print $messages;
            print $tabs;
            print $content;
         } else { */?>
            <div id="colLeft">
            
	<?php 
		print $messages;
                print $tabs;  
		print $content;
	?> 
                
            </div><!-- EOF: #main -->
            
            <div id="colRight">
                 
                <?php print $sidebar;?>
    
            </div><!-- EOF: #sidebar -->
         <?php /*}*/ ?>

    </div><!-- EOF: #content -->
    
</div><!-- EOF: #wrapper -->
    
<!-- Footer -->    
<div id="footer">
        
    <div id="footerInner">
    
        <div class="blockFooter">
            <?php print $footer_first; ?>
        </div>
        
        <div class="blockFooter">
            <?php print $footer_second; ?>
        </div>
        
        <div class="blockFooter">
            <?php print $footer_third; ?>
        </div>
        
        <div class="blockFooter">
            <?php print $footer_fourth; ?>
        </div>
        
    <div id="secondary-links">
        <?php if (isset($secondary_links)) { ?><?php print theme('links', $secondary_links, array('class' =>'secondary-links links')) ?><?php } ?>
    </div>
        
    <div id="footer-message">
        <?php print $footer_message ?>
    </div>
    
    </div>
    
    </div>

</div><!-- EOF: #footer -->

</div>
<?php print $closure ?>
</body>
</html>