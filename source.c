/*
CNIT 315
Final Project
Group Members:
-Carter Slivka, cslivka@purdue.edu
-Kangyeon Lee, lee3245@purdue.edu
-Dominic Rossi, rossi6@purdue.edu
-Adnane Sentoussi, 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <string.h>

//struct
struct weather
{
	double temp;
	double windchill;
	double windspeed;
	double humidity;
	double chanceOfPrecipitation;
	double AQI;
};

//fucntion prototype
void advise(double windchill, double chanceOfPrecipitation);

int main()
{
	printf("* * * * * * * * * * * * * * * * * * * *\n");
	printf("*           W E A T H E R             *\n");
	printf("*          F E T C H I N G            *\n");
	printf("*           S E R V I C E             *\n");
	printf("*            with advice              *\n");
	printf("* * * * * * * * * * * * * * * * * * * *\n");

	/*********VARIABLE DECLARATION*********/
	char zipcode[5];
	int choice; //Used for switch statement in menu
	bool cont = true; //used to check if the user is going to continue the program

	//Site information
	int portno = 80; //Port to use to access accuweather APIs
	char* host = "http://dataservice.accuweather.com";
	char* hostLocation = "http://dataservice.accuweather.com/locations/v1/search/";//Accuweather location fetch URL
	char* hostLocationPad = "search?q=%c&apikey=kLo88jeGZoEuKcaXkLkfSKzkAQDBmeoH HTTP/1.0\r\n"; //Will be appended to above. Will have the entered ZIP code inserted
	char hostSend[4096], hostResponse[4096]; //URL to be sent once everything is gathered and store API Responses
	char* hostForecast = "http://dataservice.accuweather.com/forecasts/v1/daily/1day/";
	char* hostForecastPad; //Will later be declared equal to the location pad
	char* argv; //Arguments placholder for the process of modifying the URLs

	/*********PRIMARY MENU*********/
	do {
		printf("\nPress 1 to search for current weather by ZIP code\n");
		printf("Press 2 to search for 1 day forecast by ZIP code\n");
		printf("Press 3 to search for current weather by geo location\n");
		printf("Press 4 to search for 1 day forecast by geo location\n");
		printf("Press any other key to exit\n");
		printf("Please enter your choice: ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			do
			{
				//ZIP Code Input
				printf("\nPlease enter the five-digit ZIP code: ");
				scanf("%s", &zipcode[5]);
			} while (atoi(zipcode) < 0 || atoi(zipcode) > 99999);

			if (atoi(zipcode) < 0 || atoi(zipcode) > 99999)
				printf("Error: Invalid ZIP Code! Try again.");

			sprintf(hostSend, hostLocationPad, argv[1], argv[2]); //Add the zipcode to the Location Pad
			printf("Request to AccuWeather: \n%c\n", hostSend); //Verify 

			//Create socket
			int sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (sockfd < 0) {
				printf("Error encountered");
				scanf("%c");
			}

			//Get AccuWeather IP
			char AccuWeatherIP = gethostbyname(host);

			//Connect to AccuWeather and send
			char total = strlen(hostSend);
			int sent = 0;
			do {
				int bytes = write(sockfd, hostSend + sent, total - sent);
				if (bytes == 0)
					break;
			} while (sent < total);

			//TODO: Write the receive code and close the socket
			// print weather according to the zip code
			break;
		case 2:
			//1 day forecast by zipcode
			break;
		case 3:
			// current weather by geo location
			break;
		case 4:
			// 1 day forecast by geo location
			break;
		default:
			cont = false;
		}
	} while (cont == true);

	/*TODO
	Ask user for input that will be used & store the input
	For now just use ZIP Codes and keep the search local to the United States for EZ points
	The character string will have to be passed through a libcurl() function that will be send to the Accuweather API
	*/

	//TODO: Send user input to the locationID API - Needs to use libcurl
	//Add to "http://dataservice.accuweather.com/locations/v1/search?q=16801&apikey{apiKey}" where the last "apiKey" is user input


	//TODO: Obtain locationKey from API response.

	//TODO: Append to Forecast API URL
	//May give user the option of multiple forcast day options
	//Example link for 1 day: "http://dataservice.accuweather.com/forecasts/v1/daily/1day/{locationKey}" - locationKey = API's earlier response.

	//TODO: Retrieve information from the API

	//TODO: Scan through the API forecast for key words
	//Some expirementing required to figure out what we are able to read.
	//Examples to check for, rain vs sunny, temperature is below freezing


	return 0;
}


void advise(double windchill, double chanceOfPrecipitation)
{
	//advise on temperature
	if (windchill < 30)
		printf("The weather is freezing! You need a thick sweater and a winter coat. Heavy coats, gloves, hats and boots are recommended.\n");
	else if (windchill < 50)
		printf("The weather is cold. Although you don't need a heavy coat, layers are recommended. Long-sleeved tees, sweaters, light jacket or coat, closed-toed shoes are recommended.\n");
	else if (windchill < 70)
		printf("The weather is warm. Long sleeved tees, jeans are recommended. You might want to bring a windbreaker to resist windchill. \n");
	else if (windchill < 100)
		printf("The weather is hot! Wear short-sleeved tees, short pants. Cool Fabrics will reduce sweating.\n");

	//advise on precipitation
	if (chanceOfPrecipitation < 30)
		printf("There is a low chance of precipitation. You don't have to bring an umbrella.\n");
	else if (chanceOfPrecipitation < 50)
		printf("Bring your umbrella with you when you go outside. Although the percentage is low, there is a chance of raining.\n");
	else if (chanceOfPrecipitation < 70)
		printf("Bring your umbrella with you when you go outside. There is a slightly high chance of raining.\n");
	else if (chanceOfPrecipitation < 100)
		printf("You must bring your umbrella with you when you go outside. There is a very high chance of raining.\n");

}