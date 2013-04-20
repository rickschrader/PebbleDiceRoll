#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "xprintf.h"

#define MY_UUID { 0x0A, 0x27, 0x89, 0xF5, 0xC3, 0x1B, 0x46, 0xA1, 0x90, 0x3C, 0xE3, 0x25, 0x22, 0xEB, 0x22, 0x77 }
PBL_APP_INFO(MY_UUID,
             "Dice Roll", "Rick Schrader",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;

TextLayer textLayer;
int maxNumber = 100;
long randSeed;
#define TIME_ZONE_OFFSET -5

int get_unix_time_from_current_time(PblTm *current_time)
{
	unsigned int unix_time;

	/* Convert time to seconds since epoch. */
    	unix_time = ((0-TIME_ZONE_OFFSET)*3600) + /* time zone offset */ 
              + current_time->tm_sec /* start with seconds */
              + current_time->tm_min*60 /* add minutes */
              + current_time->tm_hour*3600 /* add hours */
              + current_time->tm_yday*86400 /* add days */
              + (current_time->tm_year-70)*31536000 /* add years since 1970 */
              + ((current_time->tm_year-69)/4)*86400 /* add a day after leap years, starting in 1973 */
              - ((current_time->tm_year-1)/100)*86400 /* remove a leap day every 100 years, starting in 2001 */
              + ((current_time->tm_year+299)/400)*86400; /* add a leap day back every 400 years, starting in 2001*/

	return unix_time;
}

int get_unix_time()
{
	PblTm current_time;
	get_time(&current_time);

	return get_unix_time_from_current_time(&current_time);
}

int random(int max)
{
	randSeed = (((randSeed * 214013L + 2531011L) >> 16) & 32767);
	return ((randSeed % max) + 1);
}

void displayRandom()
{
	char* result = "";
	xsprintf(result, "%d", random(maxNumber));
	text_layer_set_text(&textLayer, result);
}

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

}


void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

}


void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	displayRandom();
}


void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

}

void click_config_provider(ClickConfig **config, Window *window) {
	(void)window;

	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

	config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;

	config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
	config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;

	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
	config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
}


void handle_init(AppContextRef ctx) {
	(void)ctx;

	window_init(&window, "Font Viewer");
	window_stack_push(&window, true /* Animated */);

	//resource_init_current_app(&FONT_DEMO_RESOURCES);

	text_layer_init(&textLayer, window.layer.frame);
	
	//Only set the seed once
	randSeed = get_unix_time();	

	displayRandom();
	
	text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
	layer_add_child(&window.layer, &textLayer.layer);

	
	// Attach our desired button functionality
	window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
}


void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init
	};
	app_event_loop(params, &handlers);
}
