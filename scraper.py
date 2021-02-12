from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.firefox.options import Options
import requests
import time

# Server URL
URL = 'http://localhost/MLSA/record'

# Webdriver options
options = Options()
options.add_argument('--headless')

while True:
    # Start the webdriver
    driver = webdriver.Firefox(options = options)
    driver.get('https://datelazi.ro/')
    print('Webdriver initialized')

    # Iterate through table until it finds the county or it reached the end
    county = 'Olt'
    reachedEnd = False
    ansFound = False

    while not reachedEnd and not ansFound:
        try:
            # Checks if the next page button is hidden. Throws exception if it is not
            button = driver.find_element_by_css_selector('div.button.right.hide')
        except:
            # If it threw an exception, it means that we haven't reached the last page of the table
            tableElements = driver.find_element_by_css_selector('div.perDayTable').find_elements_by_tag_name('td')

            # Gets cases per 1000 people in given county
            for i in range(len(tableElements)):
                if tableElements[i].text == county:
                    answer = tableElements[i+2].text
                    ansFound = True

            button = driver.find_element_by_css_selector('div.button.right')
            button.click()
        else:
            # Reached the last page of the table
            tableElements = driver.find_element_by_css_selector('div.perDayTable').find_elements_by_tag_name('td')

            # Gets cases per 1000 people in given county
            for i in range(len(tableElements)):
                if tableElements[i].text == county:
                    answer = tableElements[i+2].text
                    ansFound = True
            reachedEnd = True

    if not ansFound:
        print('No records found for county \"' + county + '\". \nCheck spelling')
    else:
        print('Record found for county \"' + county + '\": ' + answer)
    print('Reached end')

    driver.close()

    # Save data to a file on the server
    f = open('C:\\xampp\\htdocs\\MLSA\\record', 'w')
    f.write(answer)
    f.close()
    print('Webriver closed')

    # Check that data has been updated to server
    get_record = requests.get(URL).text
    if get_record != answer:
        print('Server data not updated')
    else:
        print('Server data updated')

    time.sleep(7200) # Wait for two hours to prevent server overload