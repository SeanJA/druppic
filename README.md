README
====

Install
----
* The dump.sql file in the dump folder is a starting point for now
  1. Change the values that have "changeme" in them
  2. Import the database, change settings.php to reflect your database settings
  3. The default password is "password", since drupal doesn't do anything other than md5, you can safely generate a new one and put it in the password box

Modules enabled, not sure if they are actually all in use at the moment
----

  * Administration theme
  * Content
  * Content Copy
  * FileField
  * ImageField
  * Number
  * Option Widgets
  * Text
  * User Reference
  * Color
  * Comment
  * Database logging
  * Help
  * Menu
  * OpenID
  * Path
  * Taxonomy
  * Update status
  * Upload
  * Devel
  * ImageAPI
  * ImageAPI GD2
  * ImageCache
  * ImageCache UI
  * OpenID Selector
  * OpenID Selector for Drupal login
  * OpenID Selector for Twitter Signin
  * OpenID Selector Inline
    * Modified so that you don't have to put in a number for the number of icons in a row (it was really annoying me...)
  * drippic
    * modified to allow editing of the content_type that it creates when installed
  * Libraries
  * Mailhandler
  * Multiple E-mail Addresses
  * OAuth
  * Pathauto
  * Token
  * Token actions
  * Twitter
  * Twitter actions
  * Twitter Post
  * Twitter Signin
  * Piwik Web analytics
    * This one should probably be removed from the setup
  * Views
  * Views UI