package Pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;

public class youTubeHomePage {
	
	WebDriver driver;
	
	public youTubeHomePage(WebDriver dr) {
		
		driver = dr;
		PageFactory.initElements(driver, this);
		
	}
	
	@FindBy(xpath="/html/body/ytd-app/div[1]/div/ytd-masthead/div[4]/div[2]/ytd-searchbox/form/div[1]/div[1]/input")
	WebElement searchBar;
	
	@FindBy(xpath="//*[@id=\"search-icon-legacy\"]")
	WebElement searchButton;
	
	@FindBy(xpath="//*[@id=\"page-manager\"]/ytd-search")
	WebElement searchResult;
	
	public void input(String input) {
		
		searchBar.sendKeys(input);
	}
	
	public void click() {
		
		searchButton.click();
	}
	
	public boolean verify() {
		
		return searchResult.isDisplayed();
		
	}

}
