<?php
use Drupal\DrupalExtension\Context;

use Behat\Behat\Context\TranslatableContext;
use Behat\Mink\Element\Element;

use Behat\Gherkin\Node\TableNode;
use Drupal\DrupalExtension\Context\DrupalContext;
use Drupal\DrupalExtension\Hook\Scope\UserScope;

class FoodCoopDrupalContext extends DrupalContext {

  /**
   * Enforce  acceptance of legal terms.
   *
   * @beforeUserCreate
   */
  public function alterUserObject(UserScope $scope) {
    $user = $scope->getEntity();
    $user->legal_accept = TRUE;
  }
}
