UPDATE `variable` SET `value` = 's:26:"changeme.changeme.changeme";' WHERE `variable`.`name` = 'openid_selector_site_name';

UPDATE `variable` SET `value` = 's:21:"changeme@changeme.com";' WHERE `variable`.`name` = 'site_mail';

DELETE FROM variable WHERE `name` LIKE 'piwik_%';