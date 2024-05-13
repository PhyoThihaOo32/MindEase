package Pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.testng.Assert;

public class TargetHomePage {
	
	 WebDriver driver;
	
	public TargetHomePage(WebDriver dr) {
		
		driver = dr;
		PageFactory.initElements(driver, this);
		
	}
	
	@FindBy(id="search")
	WebElement searchbar;
	
	@FindBy(xpath="//*[@id=\"headerPrimary\"]/div[6]/form/button[2]")
	WebElement searchButton;
	
	@FindBy(className="h-display-flex")
	WebElement searchResult;
	
	public void searchProduct(String products) {
		
		searchbar.sendKeys(products);
	}
	
	public void click() {
		
		searchButton.click();
	}
	
	public boolean verifySearchResult() {
		
		return searchResult.isDisplayed();
		
	}
	
	

}
