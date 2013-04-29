<?php

/**
 * @file
 * Template file returns HTML for Commerce Reports Visualization.
 *
 * @param $chart
 * @param $chart_attributes
 *
 * @return
 *   The chart div.
 */
?>
<div <?php print $chart_attributes; ?>><?php empty($chart) ? "" : $chart; ?></div>
