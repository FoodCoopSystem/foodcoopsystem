@javascript

Feature: As an anonymous user I want to check if can see the system
  Scenario: Go to login page
    Given I am on the homepage
    When I click "Konto"
    Then I should see the link "Zaloguj się" in the "tabs" region
