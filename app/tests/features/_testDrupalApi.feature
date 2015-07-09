@api
Feature: As an anonymous user I want to login as administrator and check my orders

  Scenario: No orders
    Given I am logged in as a user with the "administrator" role
    When I click "Zamówienia"
    Then I should not see the text "Nie masz jeszcze zrealizowanych zamówień."