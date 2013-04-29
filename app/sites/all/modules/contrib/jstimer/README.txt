
-------------- OVERVIEW -------------------------------------------
The Javascript Timer module provides a timer api that can hook html elements to
javascript widget objects. This is useful if you want a moving timer/clock or a
widget that updates every second. It comes with widgets for a countdown timer, a
countup timer, and a clock.


-------------- INSTALLING -----------------------------------------
Simply by activating the module and a widget.
There are no module dependencies.
There are no table components.


-------------- GENERAL USAGE -----------------------------------------
You can either build-up your own nested <span tags within
a full html (or filtered with <spans allowed) input format, or
use php directly.  There are now theme functions to help with
the php formatted strings.

NOTE: the date format is an ISO8601 subset, so use the
      formats as you see them below.


-------------- Inline HTML Examples --------------

Countdown timer:
<span class="jst_timer">
 <span style="display:none" class="datetime">2015-05-02T08:11:00-08:00</span>
</span>

Countdown timer with output format number setting:
<span class="jst_timer">
<span class="datetime" style="display: none;">2015-11-27T10:15:00-07:00</span>
<span class="format_num" style="display:none;">2</span>
</span>


Count up timer:
<span class="jst_timer">
  <span style="display:none" class="datetime">2010-09-20T08:11:00Z</span>
  <span style="display:none" class="dir">up</span>
</span>

NASA style down/up timer:
<span class="jst_timer">
  <span class="datetime" style="display: none;">2012-10-26T10:28:00-07:00</span>
  <span class="dir" style="display: none;">up</span>
  <span class="format_txt" style="display:none;">%sign%%hours%::%mins%::%secs%</span>
</span>


Clock:
<span class="jst_clock">
 <span style="display:none" class="clock_type">2</span>
 <span style="display:none" class="size">200</span>
</span>



-------------- PHP Input Format Examples --------------

Countdown timer:
<?php
print theme('jstimer', array(
  'widget_name' => 'jst_timer',
  'widget_args' => array(
    'datetime' => '2015-05-02T08:11:00-08:00'
  )
));
?>

Countdown timer with output format number setting:
<?php
print theme('jstimer', array(
  'widget_name' => 'jst_timer',
  'widget_args' => array(
  'datetime' => '2015-05-02T08:11:00-08:00',
    'format_num' => 2,
  )
));
?>


Count up timer:
<?php
print theme('jstimer', array(
  'widget_name' => 'jst_timer',
  'widget_args' => array(
    'datetime' => '2010-05-02T08:11:00+02:00',
    'dir'=>'up'
  )
));
?>

NASA style down/up timer:
<?php
print theme('jstimer', array(
  'widget_name' => 'jst_timer',
  'widget_args' => array(
    'datetime' => '2015-05-02T08:11:00+02:00',
    'dir'=>'up',
    'format_txt' => '%sign%%hours%::%mins%::%secs%'
  )
));
?>

Clock Widget:
<?php
print theme('jstimer', array(
  'widget_name' => 'jst_clock',
  'widget_args' => array(
    'clock_type' => 2,
    'size' => 200
  )
));
?>



-------------- Timer widget OUTPUT FORMAT ---------------------------------------
The display of the actual timer is configurable in the Site configuration
admin menu: countdowntimer.

IMPORTANT: If you have a format_num and a format_txt in a timer, the format_txt
value will trump the format_num value.

Currently supported replacement values are:
%day%   - Day number of target date (0-31)
%month% - Month number of target date (1-12)
%year%  - Year number of target date (4 digit number)
%dow%   - Day-Of-Week (Mon-Sun)
%moy%   - Month-Of-Year (Jan-Dec)

%years% - Years from set date(integer number)
%ydays% - (Days - Years) from set date(integer number)

%days%  - Total Days from set date (integer number)

%hours% - (Hours - Days) from set date (integer number, zero padded)
%mins%  - (Minutes - Hours) from set date (intger number, zero padded)
%secs%  - (Seconds - Minutes) from set date (integer number, zero padded)

%hours_nopad% - (Hours - Days) from set date (integer number, no padding)
%mins_nopad%  - (Minutes - Hours) from set date (intger number, no padding)
%secs_nopad%  - (Seconds - Minutes) from set date (integer number, no padding)

%months%       - (Months - Years) from set date, will be 11 or less (integer number)
%tot_months%   - Months from set date, can be larger than 11 (integer number)
%sign%         - Is used for NASA style countdown, will be '-' before set date and '' after set date (blank or '-')



-------------- CAVEATS ---------------------------------------------
If a daylight saving time shift should occur in either the client's tz or
the target's tz between the current date/time and your target datetime,
you could be off by one hour until you pass the point of conversion.

If you use the PHP input format beware.  If you have a syntax or other PHP error
and you've put your code in a block visible on all pages your entire site
may go down and you'll need to edit your database directly to delete that block!
