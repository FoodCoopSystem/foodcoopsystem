<?php

/**
 * @file
 * Template file returns HTML for a single block on the Commerce Reports dashboard.
 *
 * @param $block
 *
 * @return
 *   The block div.
 */

if (!empty($block['#children'])) {
  $attributes = array(
    'class' => array('commerce-reports-dashboard-block', 'commerce-reports-dashboard-' . $block['#name'] . '-block'),
  );
?>
<div<?php print drupal_attributes($attributes); ?>>
  <div class='header'>
    <h1><?php print $block['#title']; ?></h1>
    <?php if (!empty($block['#report'])) { ?>
    <span>(from <?php print $block['#report']; ?>)</span>
    <?php } ?>
    <?php if (!empty($block['#operations'])) { ?>
    <div class='operations'>
      <?php print $block['#operations']; ?>
    </div>
    <?php } ?>
  </div>
  <?php
    $sectionWidth =  (1. / (count($block['#visible']))) * 100;

    $i = 0;
    foreach ($block['sections'] as $name => $render) {
      if (($name != '#children') && ($name != '#printed')) {
        $attributes = array(
          'class' => array('commerce-reports-dashboard-section', 'commerce-reports-dashboard-section-' . $name),
          'data-section' => $name,
          'style' => '',
        );

        if (in_array($name, array_values($block['#visible']), TRUE)) {
          $attributes['class'][] = 'visible';
        }

        if (!empty($render['#width'])) {
          $attributes['style'] .= 'width: ' . $render['#width'] . '%';
        } else {
          $attributes['style'] .= 'width: ' . floor($sectionWidth) . '%';
        }
  ?>
        <div<?php print drupal_attributes($attributes) ?>><?php print $render['#children']; ?></div>
  <?php
        $i ++;
      }
    }
  ?>
</div>
<?php } ?>
