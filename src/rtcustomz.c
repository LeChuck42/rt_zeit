#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

#define MAX_RT_LENGTH 6

#define MY_UUID { 0x0C, 0xDB, 0x71, 0x1F, 0x7D, 0x2B, 0x42, 0x80, 0x81, 0x4A, 0xB7, 0xA0, 0x99, 0x9A, 0x28, 0x58 }
PBL_APP_INFO(MY_UUID,
             "RT Zeit", "RT Customz",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
TextLayer label;

static const char rt_digits[42] = {
  '0',
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  'A',
  'B',
  'C',
  'D',
  'E',
  'F',
  'G',
  'H',
  'I',
  'J',
  'K',
  'L',
  'M',
  'N',
  'O',
  'P',
  'Q',
  'R',
  'S',
  'T',
  'U',
  'V',
  'W',
  'X',
  'Y',
  'Z',
  '~',//\xA7',
  'a',//\xC4',
  'o',//\xD6',
  'u',//\xDC',
  's',//\xDF',
  '@'};

static char buffer[MAX_RT_LENGTH+1];

static void convert_to_rt(time_t t, char* buf) {
  unsigned int index = MAX_RT_LENGTH;
  t =  t - 943016400;
  while (index)
  {
    buf[--index] = rt_digits[t%42];
    t = t/42;
  }
}

static void update_label() {
  buffer[MAX_RT_LENGTH] = 0;
  time_t t = time(0);
  convert_to_rt(t, buffer);
  text_layer_set_text(&label, buffer);
}


static void handle_second_tick(AppContextRef app_ctx, PebbleTickEvent* e) {
  update_label();
}

void handle_init(AppContextRef ctx) {

  window_init(&window, "RT Zeit");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&RTZEIT_RESOURCES);
  
  GFont font = \
    fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MINECRAFT_22));

  text_layer_init(&label, GRect(0, 0, window.layer.frame.size.w, window.layer.frame.size.h));
  text_layer_set_background_color(&label, GColorBlack);
  text_layer_set_text_color(&label, GColorWhite);
  text_layer_set_font(&label, font);
  text_layer_set_text_alignment(&label, GTextAlignmentCenter);
  layer_add_child(&window.layer, &label.layer);
  update_label();
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
