/*
CNIT 315
Final Project
Group Members:
-Carter Slivka, cslivka@purdue.edu
-Kangyeon Lee, lee3245@purdue.edu
-Dominic Rossi, rossi6@purdue.edu
-Adnane Sentoussi asentous@purdue.edu
Brief Summary of this code: Fetch weather information from ACCUWEATHER by using ACCUWEATHER API and give an advice on clothing, AQI, precipitation, etc.
*/

//include libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>

//struct

// curl struct
struct MemoryStruct {
	char* memory;
	size_t size;
};

//current weather struct
struct c_weather
{
	char weather_summary[100];
	char temp[5];
	char windchill[5];
	char windspeed[5];
	char humidity[5];
	char hasPrecipitation[5];
};

//forecast weather struct
struct f_weather
{
	char weather_summary[100];
	char min_temp[5];
	char max_temp[5];
	char min_windchill[5];
	char max_windchill[5];
	char AQI[5];
	char AQI_category[30];
	char day_hasPrecipitation[5];
	char day_chanceOfPrecipitation[5];
	char day_windspeed[5];
	char night_hasPrecipitation[5];
	char night_chanceOfPrecipitation[5];
	char night_windspeed[5];
};


//define API_KEY variable
#define API_KEY "4P3b0PRSQAu1CAWR6o7AcO5nvDAw0Wtc"

//fucntion prototypes
char *CopyTextFromWebData(char *ap_dest_str, char *ap_src_str, const char *ap_start_word, const char *ap_end_word);
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);
char* output_zip(char* zipcode_input);
char* output_geo(char* latitude_input, char* longitude_input);
char* current_weather_location_code(char* location_code);
char* forecast_location_code(char* location_code);
void current_weather_advice(double windchill, char* hasPrecipitation);
void forecast_advice(double min_windchill, double max_windchill, int AQI, int day_chanceOfPrecipitation, int night_chanceOfPrecipitation);

int main()
{
	//main menu graphics
	printf("\n* * * * * * * * * * * * * * * * * * * *\n");
	printf("*           W E A T H E R             *\n");
	printf("*          F E T C H I N G            *\n");
	printf("*           S E R V I C E             *\n");
	printf("*       with a \"kindly\" advice        *\n");
	printf("* * * * * * * * * * * * * * * * * * * *\n");

	/*********VARIABLE DECLARATION*********/
	int choice; //stores option that user choose
	bool cont = true; //checks if the user is going to continue the program
	char zipcode[5];
	char latitude[30];
	char longitude[30];
	char * output;
	char * p_pos;
	char location_name[50];
	char location_code[10];
	char * weather_output;
	char * p_pos2;
	
	//create struct nodes to store weather information
	struct c_weather* current_weather = (struct c_weather*)malloc(sizeof(struct c_weather));
	struct f_weather* forecast_weather = (struct f_weather*)malloc(sizeof(struct f_weather));
	
	/*********PRIMARY MENU*********/
	do {
		printf("\nPress 1 to search for current weather by ZIP code\n");
		printf("Press 2 to search for 1 day forecast by ZIP code\n");
		printf("Press 3 to search for current weather by geo location\n");
		printf("Press 4 to search for 1 day forecast by geo location\n");
		printf("Press any other key to exit\n\n");
		
		printf("Please enter your choice: ");
		scanf("%1d", &choice);

		switch (choice)
		{
		case 1:
			//current weather by ZIP code
			do
			{ 
				//ZIP Code Input
				printf("Please enter the five-digit ZIP code: ");
				scanf("%s", zipcode);
				
				//Verify ZIP code
				if (atoi(zipcode) < 0 || atoi(zipcode) > 99999)
					printf("Error: Invalid ZIP Code! Try again.\n\n");
			} while (atoi(zipcode) < 0 || atoi(zipcode) > 99999);
			
			output = output_zip(zipcode); //store website text
			p_pos = strstr(output, "\"Type\":\"PostalCode\"");
			p_pos = CopyTextFromWebData(location_name, p_pos, "\"LocalizedName\":\"", "\",\"EnglishName\""); //store location name on location_name by using website text
			p_pos = CopyTextFromWebData(location_code, p_pos, "\"ParentCity\":{\"Key\":\"", "\",\"LocalizedName\""); //store location code on location_code by using website text
			
			weather_output = current_weather_location_code(location_code); //stores website text
			p_pos2 = strstr(weather_output, "\"LocalObservationDateTime\"");
			p_pos2 = CopyTextFromWebData(current_weather->weather_summary, p_pos2, "\"WeatherText\":\"", "\",\"WeatherIcon\""); //store weather summary by using website text
			p_pos2 = CopyTextFromWebData(current_weather->hasPrecipitation, p_pos2, "\"HasPrecipitation\":", ",\"PrecipitationType\""); //store hasPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(current_weather->temp, p_pos2, "\"Imperial\":{\"Value\":", ",\"Unit\":\"F\""); //store temp by using website text
			p_pos2 = CopyTextFromWebData(current_weather->windchill, p_pos2, ":17},\"Imperial\":{\"Value\":", ",\"Unit\":\"F\""); //store windchill by using website text
			p_pos2 = CopyTextFromWebData(current_weather->humidity, p_pos2, "\"RelativeHumidity\":", ",\"IndoorRelativeHumidity\""); //store humidity by using website text 
			p_pos2 = CopyTextFromWebData(current_weather->windspeed, p_pos2, "\"Speed\":{\"Metric\":{\"Value\":", ",\"Unit\":\"km/h\""); //store windspeed by using website text
			
			//print weather information
			printf("\n\n\n\n* * * * * * * * * * * * * * * * * * * * * * * *\n");
			printf("%s Weather Information\n", location_name);
			printf("* * * * * * * * * * * * * * * * * * * * * * * *\n\n");
			printf("Weather Summary = %s\n", current_weather->weather_summary);
			printf("Current Temperature = %s F\n", current_weather->temp);
			printf("Current WindChill Temperature = %s F\n", current_weather->windchill);
			printf("Current Humidity = %s percent \n", current_weather->humidity);
			printf("Current Wind Speed = %s km/s\n", current_weather->windspeed);
			printf("Raining? = %s\n", current_weather->hasPrecipitation);
			printf("\n* * * * * * * * * * * * * * * * * * * * * * * *\n\n");
			
			//print advice
			current_weather_advice(atof(current_weather->windchill), current_weather->hasPrecipitation);
			printf("\n\n\n\n");
			break;
		case 2:
			//1 day forecast by zipcode
			do
			{ 
				//ZIP Code Input
				printf("Please enter the five-digit ZIP code: ");
				scanf("%s", zipcode);
				
				//Verify ZIP code
				if (atoi(zipcode) < 0 || atoi(zipcode) > 99999)
					printf("Error: Invalid ZIP Code! Try again.\n\n");
			} while (atoi(zipcode) < 0 || atoi(zipcode) > 99999);
			
			output = output_zip(zipcode); //store website text
			p_pos = strstr(output, "\"Type\":\"PostalCode\"");
			p_pos = CopyTextFromWebData(location_name, p_pos, "\"LocalizedName\":\"", "\",\"EnglishName\""); //store location name on location_name by using website text
			p_pos = CopyTextFromWebData(location_code, p_pos, "\"ParentCity\":{\"Key\":\"", "\",\"LocalizedName\""); //store location code on location_code by using website text
			
			weather_output = forecast_location_code(location_code); //stores website text
			p_pos2 = strstr(weather_output, "\"Severity\":");
			p_pos2 = CopyTextFromWebData(forecast_weather->weather_summary, p_pos2, "\"Text\":\"", "\",\"Category\":\""); //store weather summary by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->min_temp, p_pos2, "\"Temperature\":{\"Minimum\":{\"Value\":", ",\"Unit\":\"F\""); //store min_temp by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->max_temp, p_pos2, "\"Maximum\":{\"Value\":", ",\"Unit\":\"F\""); //store max_temp by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->min_windchill, p_pos2, "\"RealFeelTemperature\":{\"Minimum\":{\"Value\":", ",\"Unit\":\"F\""); //store min_windchill by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->max_windchill, p_pos2, "\"Maximum\":{\"Value\":", ",\"Unit\":\"F\""); //store max_windchill by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->AQI, p_pos2, "\"Name\":\"AirQuality\",\"Value\":", ",\"Cat"); //store AQI by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->AQI_category, p_pos2, "gory\":\"", "\",\"CategoryValue\":"); //store AQI_category by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->day_hasPrecipitation, p_pos2, "\"HasPrecipitation\":", ",\"ShortPhrase\":"); //store day_hasPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->day_chanceOfPrecipitation, p_pos2, "\"PrecipitationProbability\":", ",\"ThunderstormProbability\""); //store day_chanceOfPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->day_windspeed, p_pos2, "\"Wind\":{\"Speed\":{\"Value\":", ",\"Unit\":\"mi/h\""); //store day_windspeed by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->night_hasPrecipitation, p_pos2, "\"HasPrecipitation\":", ",\"ShortPhrase\":"); //store night_hasPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->night_chanceOfPrecipitation, p_pos2, "\"PrecipitationProbability\":", ",\"ThunderstormProbability\":"); //store night_chanceOfPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->night_windspeed, p_pos2, "\"Wind\":{\"Speed\":{\"Value\":", ",\"Unit\":"); //store night_windspeed by using website text
			
			//print weather information
			printf("\n\n\n\n* * * * * * * * * * * * * * * * * * * * * * * *\n");
			printf("%s Weather 1 Day Forecast\n", location_name);
			printf("* * * * * * * * * * * * * * * * * * * * * * * *\n\n");
			printf("Weather Summary = %s\n", forecast_weather->weather_summary);
			printf("Minimum Temperature = %s F\n", forecast_weather->min_temp);
			printf("Maximum Temperature = %s F\n", forecast_weather->max_temp);
			printf("Minimum WindChill Temperature = %s F\n", forecast_weather->min_windchill);
			printf("Maximum WindChill Temperature = %s F\n", forecast_weather->max_windchill);
			printf("AQI = %s / %s\n\n", forecast_weather->AQI, forecast_weather->AQI_category);
			printf("DAY\n");
			printf("Raining? = %.5s\n", forecast_weather->day_hasPrecipitation);
			printf("Precipitation Probability = %s%% \n", forecast_weather->day_chanceOfPrecipitation);
			printf("Wind Speed = %s mi/h\n\n", forecast_weather->day_windspeed);
			printf("NIGHT\n");
			printf("Raining? = %.5s\n", forecast_weather->night_hasPrecipitation);
			printf("Precipitation Probability = %s%% \n", forecast_weather->night_chanceOfPrecipitation);
			printf("Wind Speed = %s mi/h\n", forecast_weather->night_windspeed);
			printf("\n* * * * * * * * * * * * * * * * * * * * * * * *\n\n");
			
			//print advice
			forecast_advice(atof(forecast_weather->min_windchill), atof(forecast_weather->max_windchill), atoi(forecast_weather->AQI), atoi(forecast_weather->day_chanceOfPrecipitation), atoi(forecast_weather->night_chanceOfPrecipitation));
			printf("\n\n\n\n");
			break;
		case 3:
			// current weather by geo location
			do
			{ 
				//latitude input
				printf("Please enter latitude value (-90 ~ 90): ");
				scanf("%s", latitude);

				//verify latitude
				if (atoi(latitude) < -90 || atoi(latitude) > 90)
					printf("Error: Invalid latitude value!\n\n");
			} while (atoi(latitude) < -90 || atoi(latitude) > 90);
			
			do
			{
				//longitude input
				printf("Please enter longitude value (-180 ~ 180): ");
				scanf("%s", longitude);
				
				//verify longitude
				if (atoi(longitude) < -180 || atoi(longitude) > 180)
					printf("Error: Invalid longitude value!\n\n");
			} while (atoi(longitude) < -180 || atoi(longitude) > 180);
			
			output = output_geo(latitude, longitude); //store website text
			p_pos = strstr(output, "\"Version\":");
			p_pos = CopyTextFromWebData(location_code, p_pos, "\"Key\":\"", "\",\"Type\""); //store location code on location_code by using website text
			p_pos = CopyTextFromWebData(location_name, p_pos, "\"LocalizedName\":\"", "\",\"EnglishName\""); //store location name on location_name by using website text
			
			weather_output = current_weather_location_code(location_code);
			p_pos2 = strstr(weather_output, "\"LocalObservationDateTime\"");
			p_pos2 = CopyTextFromWebData(current_weather->weather_summary, p_pos2, "\"WeatherText\":\"", "\",\"WeatherIcon\""); //store weather summary by using website text
			p_pos2 = CopyTextFromWebData(current_weather->hasPrecipitation, p_pos2, "\"HasPrecipitation\":", ",\"PrecipitationType\""); //store hasPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(current_weather->temp, p_pos2, "\"Imperial\":{\"Value\":", ",\"Unit\":\"F\""); //store temp by using website text
			p_pos2 = CopyTextFromWebData(current_weather->windchill, p_pos2, ":17},\"Imperial\":{\"Value\":", ",\"Unit\":\"F\""); //store windchill by using website text
			p_pos2 = CopyTextFromWebData(current_weather->humidity, p_pos2, "\"RelativeHumidity\":", ",\"IndoorRelativeHumidity\""); //store humidity by using website text 
			p_pos2 = CopyTextFromWebData(current_weather->windspeed, p_pos2, "\"Speed\":{\"Metric\":{\"Value\":", ",\"Unit\":\"km/h\""); //store windspeed by using website text
			
			//print weather information
			printf("\n\n\n\n* * * * * * * * * * * * * * * * * * * * * * * *\n");
			printf("%s Weather Information\n", location_name);
			printf("* * * * * * * * * * * * * * * * * * * * * * * *\n\n");
			printf("Weather Summary = %s\n", current_weather->weather_summary);
			printf("Current Temperature = %s F\n", current_weather->temp);
			printf("Current WindChill Temperature = %s F\n", current_weather->windchill);
			printf("Current Humidity = %s percent \n", current_weather->humidity);
			printf("Current Wind Speed = %s km/s\n", current_weather->windspeed);
			printf("Raining? = %s\n", current_weather->hasPrecipitation);
			printf("\n* * * * * * * * * * * * * * * * * * * * * * * *\n\n");
			
			//print advice
			current_weather_advice(atof(current_weather->windchill), current_weather->hasPrecipitation);
			printf("\n\n\n\n");
			break;
		case 4:
			// 1 day forecast by geo location
			do
			{ 
				//latitude input
				printf("Please enter latitude value (-90 ~ 90): ");
				scanf("%s", latitude);
				
				//Verify latitude
				if (atoi(latitude) < -90 || atoi(latitude) > 90)
					printf("Error: Invalid latitude value!\n\n");
			} while (atoi(latitude) < -90 || atoi(latitude) > 90);
			
			do
			{
				//longitude input
				printf("Please enter longitude value (-180 ~ 180): ");
				scanf("%s", longitude);
				
				//Verify longitude
				if (atoi(longitude) < -180 || atoi(longitude) > 180)
					printf("Error: Invalid longitude value!\n\n");
			} while (atoi(longitude) < -180 || atoi(longitude) > 180);
			
			output = output_geo(latitude, longitude); //store website text
			p_pos = strstr(output, "\"Version\":");
			p_pos = CopyTextFromWebData(location_code, p_pos, "\"Key\":\"", "\",\"Type\""); //store location code on location_code by using website text
			p_pos = CopyTextFromWebData(location_name, p_pos, "\"LocalizedName\":\"", "\",\"EnglishName\""); //store location name on location_name by using website text
			
			weather_output = forecast_location_code(location_code);
			p_pos2 = strstr(weather_output, "\"Severity\":");
			p_pos2 = CopyTextFromWebData(forecast_weather->weather_summary, p_pos2, "\"Text\":\"", "\",\"Category\":\""); //store weather summary by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->min_temp, p_pos2, "\"Temperature\":{\"Minimum\":{\"Value\":", ",\"Unit\":\"F\""); //store min_temp by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->max_temp, p_pos2, "\"Maximum\":{\"Value\":", ",\"Unit\":\"F\""); //store max_temp by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->min_windchill, p_pos2, "\"RealFeelTemperature\":{\"Minimum\":{\"Value\":", ",\"Unit\":\"F\""); //store min_windchill by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->max_windchill, p_pos2, "\"Maximum\":{\"Value\":", ",\"Unit\":\"F\""); //store max_windchill by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->AQI, p_pos2, "\"Name\":\"AirQuality\",\"Value\":", ",\"Cat"); //store AQI by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->AQI_category, p_pos2, "gory\":\"", "\",\"CategoryValue\":"); //store AQI_category by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->day_hasPrecipitation, p_pos2, "\"HasPrecipitation\":", ",\"ShortPhrase\":"); //store day_hasPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->day_chanceOfPrecipitation, p_pos2, "\"PrecipitationProbability\":", ",\"ThunderstormProbability\""); //store day_chanceOfPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->day_windspeed, p_pos2, "\"Wind\":{\"Speed\":{\"Value\":", ",\"Unit\":\"mi/h\""); //store day_windspeed by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->night_hasPrecipitation, p_pos2, "\"HasPrecipitation\":", ",\"ShortPhrase\":"); //store night_hasPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->night_chanceOfPrecipitation, p_pos2, "\"PrecipitationProbability\":", ",\"ThunderstormProbability\":"); //store night_chanceOfPrecipitation by using website text
			p_pos2 = CopyTextFromWebData(forecast_weather->night_windspeed, p_pos2, "\"Wind\":{\"Speed\":{\"Value\":", ",\"Unit\":"); //store night_windspeed by using website text
			
			//print weather information
			printf("\n\n\n\n* * * * * * * * * * * * * * * * * * * * * * * *\n");
			printf("%s Weather 1 Day Forecast\n", location_name);
			printf("* * * * * * * * * * * * * * * * * * * * * * * *\n\n");
			printf("Weather Summary = %s\n", forecast_weather->weather_summary);
			printf("Minimum Temperature = %s F\n", forecast_weather->min_temp);
			printf("Maximum Temperature = %s F\n", forecast_weather->max_temp);
			printf("Minimum WindChill Temperature = %s F\n", forecast_weather->min_windchill);
			printf("Maximum WindChill Temperature = %s F\n", forecast_weather->max_windchill);
			printf("AQI = %s / %s\n\n", forecast_weather->AQI, forecast_weather->AQI_category);
			printf("DAY\n");
			printf("Raining? = %.5s\n", forecast_weather->day_hasPrecipitation);
			printf("Precipitation Probability = %s%% \n", forecast_weather->day_chanceOfPrecipitation);
			printf("Wind Speed = %s mi/h\n\n", forecast_weather->day_windspeed);
			printf("NIGHT\n");
			printf("Raining? = %.5s\n", forecast_weather->night_hasPrecipitation);
			printf("Precipitation Probability = %s%% \n", forecast_weather->night_chanceOfPrecipitation);
			printf("Wind Speed = %s mi/h\n", forecast_weather->night_windspeed);
			printf("\n* * * * * * * * * * * * * * * * * * * * * * * *\n\n");
			
			//print advice
			forecast_advice(atof(forecast_weather->min_windchill), atof(forecast_weather->max_windchill), atoi(forecast_weather->AQI), atoi(forecast_weather->day_chanceOfPrecipitation), atoi(forecast_weather->night_chanceOfPrecipitation));
			printf("\n\n\n\n");
			break;
		default:
			cont = false;
		}
	} while (cont == true);

	return 0;
}


char *CopyTextFromWebData(char *ap_dest_str, char *ap_src_str, const char *ap_start_word, const char *ap_end_word)
{
	char *p_pos = strstr(ap_src_str, ap_start_word); 
	if (p_pos != NULL) {
		char *p_limit_pos = strstr(p_pos, ap_end_word); 
		if (p_limit_pos != NULL) {
			p_pos += strlen(ap_start_word); 
			char open_flag = 0;  
			while (p_pos < p_limit_pos) { 
				if (*p_pos == '<') open_flag = 1;  
				else if (*p_pos == '>') open_flag = 0; 
				else {
					if (!open_flag) { 
						if (*p_pos == '|') {  
							*ap_dest_str++  = ',';  *ap_dest_str++  = ' ';
						} else { 
							if (*(ap_dest_str - 1) != ' ' || *p_pos != ' ') *ap_dest_str++  = *p_pos;
						}
					}
				}
				p_pos++;
			}
			*ap_dest_str = 0; 
			return p_pos; 
		}
	}
	return ap_src_str; 
}

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	char* ptr = realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

char* output_zip(char* zipcode)
{
	char URL_COMBINED[256] = "http://dataservice.accuweather.com/locations/v1/postalcodes/US/search?apikey=";
	strcat(URL_COMBINED, API_KEY);
	strcat(URL_COMBINED, "&q=");
	strcat(URL_COMBINED, zipcode);
	CURL *curl_handle;
 	CURLcode res;
 
  	struct MemoryStruct chunk;
 
  	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
  	chunk.size = 0;    /* no data at this point */ 
 	
  	curl_global_init(CURL_GLOBAL_ALL);
 	
  	/* init the curl session */ 
  	curl_handle = curl_easy_init();
 
  	/* specify URL to get */ 
  	curl_easy_setopt(curl_handle, CURLOPT_URL, URL_COMBINED);
 
  	/* send all data to this function  */ 
  	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  	/* we pass our 'chunk' struct to the callback function */ 
  	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  	/* some servers don't like requests that are made without a user-agent
  	   field, so we provide one */ 
  	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 	
  	/* get it! */ 
  	res = curl_easy_perform(curl_handle);
 	
  	/* check for errors */ 
  	if(res != CURLE_OK)
  	{
  		fprintf(stderr, "curl_easy_perform() failed: %s\n",
            	curl_easy_strerror(res));
  	}
  	else {
  		return chunk.memory;
  	}
}

char* output_geo(char* latitude_input, char* longitude_input)
{
	char URL_COMBINED[256] = "http://dataservice.accuweather.com/locations/v1/cities/geoposition/search?apikey=";
	strcat(URL_COMBINED, API_KEY);
	strcat(URL_COMBINED, "&q=");
	strcat(URL_COMBINED, latitude_input);
	strcat(URL_COMBINED, ",");
	strcat(URL_COMBINED, longitude_input);
	CURL *curl_handle;
 	CURLcode res;
 
  	struct MemoryStruct chunk;
 
  	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
  	chunk.size = 0;    /* no data at this point */ 
 	
  	curl_global_init(CURL_GLOBAL_ALL);
 	
  	/* init the curl session */ 
  	curl_handle = curl_easy_init();
 
  	/* specify URL to get */ 
  	curl_easy_setopt(curl_handle, CURLOPT_URL, URL_COMBINED);
 
  	/* send all data to this function  */ 
  	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  	/* we pass our 'chunk' struct to the callback function */ 
  	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  	/* some servers don't like requests that are made without a user-agent
  	   field, so we provide one */ 
  	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 	
  	/* get it! */ 
  	res = curl_easy_perform(curl_handle);
 	
  	/* check for errors */ 
  	if(res != CURLE_OK)
  	{
  		fprintf(stderr, "curl_easy_perform() failed: %s\n",
            	curl_easy_strerror(res));
  	}
  	else {
  		return chunk.memory;
  	}
}

char* current_weather_location_code(char* location_code)
{
	char URL_COMBINED[256] = "http://dataservice.accuweather.com/currentconditions/v1/";
	strcat(URL_COMBINED, location_code);
	strcat(URL_COMBINED, "?apikey=");
	strcat(URL_COMBINED, API_KEY);
	strcat(URL_COMBINED, "&details=true");
	CURL *curl_handle;
 	CURLcode res;
 
  	struct MemoryStruct chunk;
 
  	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
  	chunk.size = 0;    /* no data at this point */ 
 	
  	curl_global_init(CURL_GLOBAL_ALL);
 	
  	/* init the curl session */ 
  	curl_handle = curl_easy_init();
 
  	/* specify URL to get */ 
  	curl_easy_setopt(curl_handle, CURLOPT_URL, URL_COMBINED);
 
  	/* send all data to this function  */ 
  	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  	/* we pass our 'chunk' struct to the callback function */ 
  	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  	/* some servers don't like requests that are made without a user-agent
  	   field, so we provide one */ 
  	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 	
  	/* get it! */ 
  	res = curl_easy_perform(curl_handle);
 	
  	/* check for errors */ 
  	if(res != CURLE_OK)
  	{
  		fprintf(stderr, "curl_easy_perform() failed: %s\n",
            	curl_easy_strerror(res));
  	}
  	else {
  		return chunk.memory;
  	}

}

char* forecast_location_code(char* location_code)
{
	char URL_COMBINED[256] = "http://dataservice.accuweather.com/forecasts/v1/daily/1day/";
	strcat(URL_COMBINED, location_code);
	strcat(URL_COMBINED, "?apikey=");
	strcat(URL_COMBINED, API_KEY);
	strcat(URL_COMBINED, "&details=true");
	CURL *curl_handle;
 	CURLcode res;
 
  	struct MemoryStruct chunk;
 
  	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
  	chunk.size = 0;    /* no data at this point */ 
 	
  	curl_global_init(CURL_GLOBAL_ALL);
 	
  	/* init the curl session */ 
  	curl_handle = curl_easy_init();
 
  	/* specify URL to get */ 
  	curl_easy_setopt(curl_handle, CURLOPT_URL, URL_COMBINED);
 
  	/* send all data to this function  */ 
  	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  	/* we pass our 'chunk' struct to the callback function */ 
  	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  	/* some servers don't like requests that are made without a user-agent
  	   field, so we provide one */ 
  	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 	
  	/* get it! */ 
  	res = curl_easy_perform(curl_handle);
 	
  	/* check for errors */ 
  	if(res != CURLE_OK)
  	{
  		fprintf(stderr, "curl_easy_perform() failed: %s\n",
            	curl_easy_strerror(res));
  	}
  	else {
  		return chunk.memory;
  	}

}

void current_weather_advice(double windchill, char* hasPrecipitation)
{
	printf("\n* * * * * * * * * * * * * * * * * * * *\n");
	printf("*        Advice on Weather            *\n");
	printf("* * * * * * * * * * * * * * * * * * * *\n\n");
	
	//advise on temperature
	if (windchill < 30)
	{
		printf("The weather is freezing!\n");
		printf("Recommendation on clothing:\n");
		printf("     Outerwear: Double-layered hooded down jackets (Preferably waterproof)\n");
		printf("     Tops: Sweaters, Jumpers, Turtlenecks\n");
		printf("     Bottoms: Pants, Leggings (Preferably waterproof) Thermal underwear\n");
		printf("     Accessories: Beanies, Ear Muffs, Scarves, Thick Socks (bring multiple pairs to layer, if need be), Heat pads (for extra warmth!)\n");
		printf("     Tops: Sweaters, Jumpers, Turtlenecks\n");
	}
	else if (windchill < 60)
	{
		printf("The weather is cold.\n");
		printf("Recommendation on clothing:\n");
		printf("     Tops (for Layering): Shirts, Hoodies, Dresses\n");
		printf("     Lightweight Outerwear: Leather jackets, Biker jackets, Parkas, Pea Coats\n");
		printf("     Bottoms: Jeans, Trousers, Skirts Shoes: Sneakers, Boots\n");
		printf("     Accessories: A light scarf *optional\n");
	}
	else if (windchill < 80)
	{
		printf("The weather is warm. Long sleeved tees, jeans are recommended. You might want to bring a windbreaker to resist windchill. \n");
		printf("Recommendation on clothing:\n");
		printf("     Tops: Long sleeved tee\n");
		printf("     Bottoms: Jeans, trousers, closed toe shoes\n");
		printf("     Tops (for Layering): Shirts, Hoodies, Dresses\n");
		printf("     light jackets: windbreak *optional\n");
	}
	else
		printf("The weather is hot! Wear short-sleeved tees, short pants. Cool Fabrics will reduce sweating.\n");

	//advice on precipitation
	if (strcmp(hasPrecipitation, "true") == 0)
		printf("\nIt is raining outside. Bring an Umbrella with you.\n");

}

void forecast_advice(double min_windchill, double max_windchill, int AQI, int day_chanceOfPrecipitation, int night_chanceOfPrecipitation)
{
	printf("\n* * * * * * * * * * * * * * * * * * * *\n");
	printf("*        Advice on Weather            *\n");
	printf("* * * * * * * * * * * * * * * * * * * *\n\n");
	
	//advise on temperature
	if (((min_windchill + max_windchill)/2) < 30)
	{
		printf("The weather is freezing!\n");
		printf("     Recommendation on clothing:\n");
		printf("     Outerwear: Double-layered hooded down jackets (Preferably waterproof)\n");
		printf("     Tops: Sweaters, Jumpers, Turtlenecks\n");
		printf("     Bottoms: Pants, Leggings (Preferably waterproof) Thermal underwear\n");
		printf("     Accessories: Beanies, Ear Muffs, Scarves, Thick Socks (bring multiple pairs to layer, if need be), Heat pads (for extra warmth!)\n");
		printf("     Tops: Sweaters, Jumpers, Turtlenecks\n");
	}
	else if (((min_windchill + max_windchill)/2) < 60)
	{
		printf("The weather is cold.\n");
		printf("     Recommendation on clothing:\n");
		printf("     Tops (for Layering): Shirts, Hoodies, Dresses\n");
		printf("     Lightweight Outerwear: Leather jackets, Biker jackets, Parkas, Pea Coats\n");
		printf("     Bottoms: Jeans, Trousers, Skirts Shoes: Sneakers, Boots\n");
		printf("     Accessories: A light scarf *optional\n");
	}
	else if (((min_windchill + max_windchill)/2)  < 80)
	{
		printf("The weather is warm. Long sleeved tees, jeans are recommended. You might want to bring a windbreaker to resist windchill.\n");
		printf("     Recommendation on clothing:\n");
		printf("     Tops: Long sleeved tee\n");
		printf("     Bottoms: Jeans, trousers, closed toe shoes\n");
		printf("     Tops (for Layering): Shirts, Hoodies, Dresses\n");
		printf("     light jackets: windbreak *optional\n");
	}
	else
		printf("The weather is hot! Wear short-sleeved tees, short pants. Cool Fabrics will reduce sweating.\n");
	
	//advice on AQI
	if (AQI > 100 && AQI < 150)
		printf("\nUnhealthy air quality for sensitive groups.\n Sensitive groups include people with lung disease, older adults andchildren at a greater risk frome xposure to ozone. \nWe recommend to wear a respirator mask for sensitive people.\n");
	else if (AQI > 150 && AQI < 200)
		printf("\nAir quality is bad. We highly do not recommend going outside without a respiratory mask.\n");
	else if (AQI >= 200)
		printf("\nAir quality is extremely hazardous. Please do not go outside.\n");
		
	//advice on day precipitation
	if (day_chanceOfPrecipitation >= 20 && day_chanceOfPrecipitation < 40)
		printf("\nThere is a low chance of raining today in the day time. You might want to bring your umbrella if your luck is bad today.\n");
	else if (day_chanceOfPrecipitation >= 40 && day_chanceOfPrecipitation < 60)
		printf("\nThere is a moderate chance of raining today in the day time. Bring an umbrella with you in case it rains.\n");
	else if (day_chanceOfPrecipitation >= 60)
		printf("\nThere is a high chance of raining today in the day time. You must bring an umbrella.\n");
		
	//advice on day precipitation
	if (night_chanceOfPrecipitation >= 20 && night_chanceOfPrecipitation < 40)
		printf("\nThere is a low chance of raining today in the night. You might want to bring your umbrella if your luck is bad today.\n");
	else if (night_chanceOfPrecipitation >= 40 && night_chanceOfPrecipitation < 60)
		printf("\nThere is a moderate chance of raining today in the night. Bring an umbrella with you in case it rains.\n");
	else if (night_chanceOfPrecipitation >= 60)
		printf("\nThere is a high chance of raining today in the night. You must bring an umbrella.\n");

}
