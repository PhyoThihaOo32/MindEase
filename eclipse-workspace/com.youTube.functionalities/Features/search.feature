Feature: Searching on youTube
Scenario Outline: Searching on youTube
Given I am at the youTube Homepage
When I enter the "<input>" in the searchbar
And I click the search button
Then I can see the search result successfully

Examples:
|input|
|coldplay|
|bring me the horizons|
|this is the new shit|
|Lofi|
