
MAYO - original theme for Drupal 7 from PIXTURE STUDIO

The name "MAYO" is taken from a phrase "MAke Your Own" and "MAYOnnaise" which I love :-). As the name describes itself, MAYO is developed to be a theme that provides users easy ways to customize and create a theme they like.

MAYO is developed based on the experience of developing PIXTURE theme. However, MAYO fully takes advantage of Drupal 7's color module and advaced theme settings.  MAYO is simple but flexible. You can customize the look of the MAYO easily from the theme setting page. It does not require you to have the knowledge of CSS, HTML and PHP.


What can be customized from the theme settings page ?
=====================================================

MAYO's theme settings page provides many options you can select/customize.  Below are the summary of what you can customize.

1. Colors of most of the theme elements (base, page, header, footer, sidebar, node, text and link color of most of the elements), or users can choose a color set from the large number of predefined color sets.

2. Base font and heading font type (either Serif or Sans-serif) and base font size.

3. Page layout options such as width of the site (fixed width layout or liquid layout), margins, sidebar layout and width, etc.

4. Styles such as menubar styles and round corner for node/sidebar block.

5. Detail layout of header area contents (logo, site name, slogan, search engine box).

6. You can easily upload and add your own image as the background of the header area. (You can use both background image and logo at the same time).

7. Watermark of the header area which is added over the header gradation or header background images.

8. Other miscellaneous options such as toggle to display/not display breadcrumbs.

9 You can add font resizing control to the header (default is OFF)

The template files it uses are page.tpl.php and comment.tpl.php only (+ maintenance_page.tpl.php for maintenance mode). For all other elements, the standard template files that comes with Drupal Core will be used.

In addition to the customization features listed above, it supports following drop down menu modules.

Superfish module <http://drupal.org/project/superfish>
Nice Menus module <http://drupal.org/project/nice_menus>


Regions
=========

MAYO supports the following regions.

+-----------------------------------------+
| +-------------------------------------+ |
| |              Header                 | |
| +-------------------------------------+ |
| Menu                                    |
| Submenu                                 |
| +-------------------------------------+ |
| |              Banner Top             | |
| +-------------------------------------+ |
| +-------------------------------------+ |
| | Top      Top      Top      Top      | |
| | Column1  Column2  Column3  Column4  | |
| +-------------------------------------+ |
| +---------+ +-------------------------+ |
| | Sidebar | |       Highlighted       | |
| |         | |                         | |
| |         | +-------------------------+ |
| |         | +-------------------------+ |
| |         | |         Content         | |
| |         | |                         | |
| |         | |                         | |
| |         | |                         | |
| |         | |                         | |
| +---------+ |                         | |
|             |                         | |
|             +-------------------------+ |
| +-------------------------------------+ |
| | Bottom   Bottom   Bottom   Bottom   | |
| | Column1  Column2  Column3  Column4  | |
| +-------------------------------------+ |
| +-------------------------------------+ |
| |            Banner Bottom            | |
| +-------------------------------------+ |
| +-------------------------------------+ |
| |                                     | |
| | Footer   Footer   Footer   Footer   | |
| | Column1  Column2  Column3  Column4  | |
| |                                     | |
| | ----------------------------------- | |
| |               Footer                | |
| +-------------------------------------+ |
+-----------------------------------------+

As you can see, it supports multiple columns in the top, bottom and footer regions. You can add up to four columns. The width of a column is automatically calculated based on the number of columns to be used. For example, if you add 3 blocks to the footer, then the width of each column will be 33% of the footer width.
Each block in the top and bottom columns regions has its own box style just like sidebar blocks. Height of these blocks are equalized to the tallest blocks within the same region so that they looks nice and neat.

Menu and Submenu regions are for those who uses superfish, nice_menus and other drop down menus (see the next section for more details).  Position of the sidebar first and the second can be configured from the theme settings page.


Mininum width
==============
Currently, the theme uses 700px as the minimum-width. If you want to change it, please manually edit the css/layout.css and change the min-width of the body.


How to use superfish/nice_menus module?
=======================================
The primary menu and secondary menu does not support superfish and nice_menus. If you want to use one of them, you need to turn off the primary and secondary menu from the theme settings page first. Then, create and configure menu blocks and then assign them to either the 'menubar' or 'sub-menubar' region of the MAYO theme at admin/structure/block page.
When you select menubar type 2 (gloss black image background), the type style is applied only to the menubar, and the sub-menubar still uses original menubar style. This is simply because that having two menubar rows of gloss black background image does not look nice from the design stand point.


Dark color theme
================
One of the characteristics of the MAYO theme is that it supports both light color and dark color sets. We developed MAYO so that it would look nice either cases. However, the message (status, warning, error) colors are defined in the Drupal Core and it would not match the dark color sets. So we have created own CSS for the messages for the dark color sets. You need to check the option of 'Use dark message colors' in the theme settings page if you use a dark color set.


Header backgroud image
======================
If you use liquid theme, you should be aware that the header area width expands as the browser's window size is widen.
If the header background image you choose is not wide enough, the image repeats, which may make the header area a bit ugly. We recommend that you prepare and use the header image at least 1600px wide for the liquid layout.  Of course, this is not an issue for fixed layout.


Round corners for sidebar block and node
========================================
By using CSS3 and browser specific stylesheet properties, round corners for sidebar blocks and node are supported by major browsers such as Fireforx, Safari, Google Chrome and Opera except for IE. We have decided not to support round corners for IE with Javascript since may slows down the page display. So if you are IE users, please wait for the future version of IE that supports CSS3's border-radius property.


Changing default color set
===========================
Please do not change the default color set (MAYO) in the color/color.inc file.  You can change other entries but not the default one. Otherwise, it will mess up the entire color changing scheme.



Author
=======
Hideki Ito
PIXTURE STUDIO <http://www.pixture.com>

