<?php

use Behat\Behat\Hook\Scope\AfterStepScope;
use Behat\Mink\Driver\Selenium2Driver;
use Behat\Mink\Driver\GoutteDriver;
use Drupal\DrupalExtension\Context\RawDrupalContext;
use Behat\Behat\Context\SnippetAcceptingContext;
use Behat\Gherkin\Node\PyStringNode;
use Behat\Gherkin\Node\TableNode;

/**
 * Defines application features from the specific context.
 */
class FeatureContext extends RawDrupalContext implements SnippetAcceptingContext {

  /**
   * Initializes context.
   *
   * Every scenario gets its own context instance.
   * You can also pass arbitrary arguments to the
   * context constructor through behat.yml.
   */
  public function __construct() {
  }

    /**
     * @AfterStep
     */
    public function takeScreenShotAfterFailedStep(afterStepScope $scope)
    {
/*        if (99 === $scope->getTestResult()->getResultCode()) {
            $driver = $this->getSession()->getDriver();
            if (!($driver instanceof Selenium2Driver)) {
               // return;
            }
            file_put_contents('test_.png', $this->getSession()->getDriver()->getScreenshot());
        }*/
    }

    /**
     * @AfterStep
     */
    public function save_step_as_html(afterStepScope $scope) {

        $html_data = $this->getSession()->getDriver()->getContent();
        $file_and_path = dirname(__FILE__) . '/../../errors/fcs_' . $scope->getStep()->getText() . '.html';
        file_put_contents($file_and_path, $html_data);
    }

}
