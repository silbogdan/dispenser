## About the project


This is a disinfectant dispenser that uses a webscraper to get data about COVID-19 form the internet. It serves as an academic project for the local Microsoft Learn Student Ambassadors organization.

The webscraper is created with Python, using Selenium. It connects to a website providing local data and scrapes it from a table. The data is then provided to the local server.

The dispenser's circuit can be found in the Fritzing file. It uses a water pump and a HC_SR04 distance sensor to supply the user with disinfectant. A LoLin V3 NodeMCU is used to connect to the local server and get the updated COVID-19 data.
