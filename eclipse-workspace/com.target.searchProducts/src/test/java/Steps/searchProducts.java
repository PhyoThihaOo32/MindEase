package Steps;


import Commons.TargetBase;
import Pages.TargetHomePage;
import io.cucumber.java.en.Given;
import io.cucumber.java.en.Then;
import io.cucumber.java.en.When;

public class searchProducts extends TargetBase {
	
	TargetHomePage tp;
	
	@Given("I am on Target Homepage")
	public void i_am_on_target_homepage() {
		
		launchBrowser();
	   
	}

	@When("I enter the {string} in the searchbar")
	public void i_enter_the_in_the_searchbar(String string) {
		
		tp = new TargetHomePage(driver);
		tp.searchProduct(string);
	
	}

	@When("I click the search button")
	public void i_click_the_search_button() {
		
		tp.click();

	}

	@Then("I can see the products")
	public void i_can_see_the_products() {
		
		tp.verifySearchResult();
		closeAll();
	
	}



}
