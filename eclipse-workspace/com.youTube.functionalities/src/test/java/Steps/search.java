package Steps;

import org.testng.Assert;

import Common.youTubeBase;
import Pages.youTubeHomePage;
import io.cucumber.java.en.Given;
import io.cucumber.java.en.Then;
import io.cucumber.java.en.When;

public class search extends youTubeBase {
	
	youTubeHomePage yp;
	
	@Given("I am at the youTube Homepage")
	public void i_am_at_the_you_tube_homepage() {
		
		launchBrowser();
	   
	}

	@When("I enter the {string} in the searchbar")
	public void i_enter_the_in_the_searchbar(String string) {
		
		yp = new youTubeHomePage(driver);
		yp.input(string);
	   
	}

	@When("I click the search button")
	public void i_click_the_search_button() {
		
		yp.click();
	   
	}

	@Then("I can see the search result successfully")
	public void i_can_see_the_search_result_successfully() {
		
		Assert.assertTrue(yp.verify());
	   
	}


}
