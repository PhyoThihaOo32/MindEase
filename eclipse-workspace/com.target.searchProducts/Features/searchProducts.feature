Feature: Searching Products 
Scenario Outline: Searching Products on Target
Given I am on Target Homepage
When I enter the "<product_name>" in the searchbar
And I click the search button
Then I can see the products

Examples:
|product_name|
|Vegetables|
|Furnitures|
|Cosmatics|
|Drugs|




