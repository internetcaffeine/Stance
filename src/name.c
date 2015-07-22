/*
 * main.c
 * Sets up a Window and BitmapLayer, loads all of the PNGs that make up the animation ad GBitmaps, 
 then advances through the frames when the data from the accelerometer indicates that the user has tilted their watch up
 to view the image.
 
 If the animation doesn't start, keep turning your wrist until the top of the watch is just past perpendicular to the ground.
 */

#include <pebble.h>

//create the types of variables that we will need in to run the program

//create window object  
static Window *window;
//create blank bitmap layer object, that we will later attach to the window
static BitmapLayer *image_layer;

//create blank bitmap objects that we will later attach to the image layer
static GBitmap *seasick_the_cat_bmp_01;
static GBitmap *seasick_the_cat_bmp_02;
static GBitmap *seasick_the_cat_bmp_03;
AppTimer *animation_timer;

//animation frame counter
int animation_frame = 0;
//number of samples to capture per update when grabbing accelerometer data
int num_samples = 3;


//accelerometer handler, this code will loop constantly when the program launches at the rate specified by accel_service_set_sampling_rate()
//function during the initialization phase.
static void apptimer_handler() {

  
  //double z_val = data[0].z;
  
  //if the watch is tipped past ~90 deg advance the animation frame by one 
  if(animation_frame < 4){
    animation_frame = animation_frame + 1;
  }
  
  //big ol' switch statement to animate this mess.
  //note that if the animation reaches the last frame,
  //the animation_frame counter is set to 5 before breaking out of the code block
  //this is so that the animation will continue to run, but will start over about half-way
  //through the loop to avoid the animation running backwards from an unnatural point in the series
  //(e.g. the barf flying back into the cat's mouth, yeesh)
  //it will also allow the animation to keep running from that point so as to crack your friends up.
    switch (animation_frame){
      case 1:
      bitmap_layer_set_bitmap(image_layer, seasick_the_cat_bmp_01);
      break;
      case 2:
      bitmap_layer_set_bitmap(image_layer, seasick_the_cat_bmp_02);
      break;
      case 3:
      bitmap_layer_set_bitmap(image_layer, seasick_the_cat_bmp_03);
      break;
      case 4:
      animation_frame = 0;
      break;
      
    }
  animation_timer = app_timer_register(100, apptimer_handler, NULL);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  //get the boundary of the root window layer
  GRect bounds = layer_get_bounds(window_layer);
  //create the bitmap layer and make it the same size as the window so that it takes up the whole screen
  image_layer = bitmap_layer_create(bounds);
  
  //load up frames of animation into the bitmap objects we established at the top of the code
  //TODO: remember to come back and swap these PNGs out for non-transparent resources, so to 
  //simplify the resource id needed to instantiate them.
  seasick_the_cat_bmp_01 = gbitmap_create_with_resource(RESOURCE_ID_STK_0);
  seasick_the_cat_bmp_02 = gbitmap_create_with_resource(RESOURCE_ID_STK_1);
  seasick_the_cat_bmp_03 = gbitmap_create_with_resource(RESOURCE_ID_STK_2);
  
  //set the initial frame that we want to appear on the screen when the app loads up
  bitmap_layer_set_bitmap(image_layer, seasick_the_cat_bmp_01);

  //add the configured image layer to the window layer.
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
  
  animation_timer = app_timer_register(100, apptimer_handler, NULL);
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(seasick_the_cat_bmp_01);
  gbitmap_destroy(seasick_the_cat_bmp_02);
  gbitmap_destroy(seasick_the_cat_bmp_03);

  bitmap_layer_destroy(image_layer);
  
}

static void init() {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(window, true);
  
   
}

//
static void deinit() {
  accel_data_service_unsubscribe();
  window_destroy(window);
  
}

int main() {
  init();
  app_event_loop();
  deinit();
}