@api
Feature: As an anonymous user I want to login as administrator and check my orders

  Scenario: No orders
    When I am logged in as a user with the "Administrator" role
    And I click "Zamówienia"
    Then I should not see the text "Nie masz jeszcze zrealizowanych zamówień."