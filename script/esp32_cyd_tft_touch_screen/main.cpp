#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>
#include <XPT2046_Touchscreen.h>

/*Touch screen config*/
#define XPT2046_IRQ 36 //GPIO driver cảm ứng 
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
SPIClass tsSpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240,touchScreenMaximumY = 3800; //Chạy Calibration để lấy giá trị mỗi màn hình mỗi khác

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    if(ts.touched())
    {
        TS_Point p = ts.getPoint();
        //Some very basic auto calibration so it doesn't go out of range
        if(p.x < touchScreenMinimumX) touchScreenMinimumX = p.x;
        if(p.x > touchScreenMaximumX) touchScreenMaximumX = p.x;
        if(p.y < touchScreenMinimumY) touchScreenMinimumY = p.y;
        if(p.y > touchScreenMaximumY) touchScreenMaximumY = p.y;

        // Serial.print( "Touch x " );
        // Serial.print( p.x );
        // Serial.print( " y " );
        // Serial.println( p.y );

        //Map this to the pixel position
        data->point.x = map(p.x,touchScreenMinimumX,touchScreenMaximumX,1,screenWidth); /* Touchscreen X calibration */
        data->point.y = map(p.y,touchScreenMinimumY,touchScreenMaximumY,1,screenHeight); /* Touchscreen Y calibration */
        data->state = LV_INDEV_STATE_PR;

        // Serial.print( "Mapped Touch x " );
        // Serial.print( data->point.x );
        // Serial.print( " y " );
        // Serial.println( data->point.y );
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

void my_switch_event_cb(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    bool state = lv_obj_has_state(obj, LV_STATE_CHECKED);
    printf("Switch state changed: %s\n", state ? "ON" : "OFF");
}

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    //Initialise the touchscreen
    tsSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
    ts.begin(tsSpi);      /* Touchscreen init */
    ts.setRotation(3);   /* Inverted landscape orientation to match screen */

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

    tft.begin();          /* TFT init */
    tft.setRotation( 3 ); /* Landscape orientation, flipped */

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    //     // Register the event callback after creating the objects:
    
    // lv_obj_add_event_cb(ui_Arc1, my_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    // lv_obj_add_event_cb(ui_Slider1, my_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    // lv_obj_add_event_cb(ui_Switch1, my_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    ui_init();
        
    // Register the callback after creating ui_Switch1:
    lv_obj_add_event_cb(ui_Switch1, my_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    Serial.println( "Setup done" );

    Serial.println("TFT_CS: " + String(TFT_CS));
    Serial.println("TFT_DC: " + String(TFT_DC));
    Serial.println("TFT_RST: " + String(TFT_RST));
    Serial.println("TFT_MOSI: " + String(TFT_MOSI));
    Serial.println("TFT_MISO: " + String(TFT_MISO));
    Serial.println("TFT_SCLK: " + String(TFT_SCLK));
    Serial.println("TFT_BL: " + String(TFT_BL));
    Serial.println("XPT2046_CS: " + String(XPT2046_CS));
    Serial.println("XPT2046_IRQ: " + String(XPT2046_IRQ));
    Serial.println("XPT2046_MOSI: " + String(XPT2046_MOSI));
    Serial.println("XPT2046_MISO: " + String(XPT2046_MISO));
    Serial.println("XPT2046_CLK: " + String(XPT2046_CLK));
    Serial.println("XPT2046 Minimum X: " + String(touchScreenMinimumX));
    Serial.println("XPT2046 Maximum X: " + String(touchScreenMaximumX));
    Serial.println("XPT2046 Minimum Y: " + String(touchScreenMinimumY));
    Serial.println("XPT2046 Maximum Y: " + String(touchScreenMaximumY));
    Serial.println("Screen Width: " + String(screenWidth));
    Serial.println("Screen Height: " + String(screenHeight));
    Serial.println("RGB LED GPIO 4: " + String(4));
    Serial.println("RGB LED GPIO 16: " + String(16));
    Serial.println("RGB LED GPIO 17: " + String(17));
    Serial.println("RGB LED PWM Frequency: 5000 Hz");
    Serial.println("RGB LED PWM Resolution: 8 bits");
    Serial.println("RGB LED Setup Complete");
    Serial.println("LVGL Version: " + String(LVGL_VERSION_MAJOR) + "." + String(LVGL_VERSION_MINOR) + "." + String(LVGL_VERSION_PATCH));
    Serial.println("UI Initialized");
    Serial.println("ESP32 Setup Complete");
}

void loop()
{
    lv_tick_inc(5); /* tell LVGL that 5 milliseconds were elapsed */
    lv_timer_handler(); /* let the GUI do its work */
    delay(5);

    static uint8_t x = 0;
    static uint8_t state_change = 0;
    x++;
    if (x > 200) {
        x = 0;
        Serial.println("Looping...");

        // Get value from Arc
        int arc_value = lv_arc_get_value(ui_Arc1);

        // Get value from Slider
        int slider_value = lv_slider_get_value(ui_Slider1);

        // Get state from Switch (returns true if ON, false if OFF)
        bool switch_state = lv_obj_has_state(ui_Switch1, LV_STATE_CHECKED);

        Serial.print("ui_Arc1...");
        Serial.println(arc_value);
        Serial.print("ui_Slider1...");
        Serial.println(slider_value);
        Serial.print("ui_Switch1...");
        Serial.println(switch_state);

        state_change = !state_change;
        lv_arc_set_value(ui_Arc1, 100*state_change); // Increment arc value
        lv_arc_set_value(ui_Slider1, 100*state_change);
    }

    // if(ts.touched())
    // {
    //     TS_Point p = ts.getPoint();
    //     //Some very basic auto calibration so it doesn't go out of range
    //     if(p.x < touchScreenMinimumX) touchScreenMinimumX = p.x;
    //     if(p.x > touchScreenMaximumX) touchScreenMaximumX = p.x;
    //     if(p.y < touchScreenMinimumY) touchScreenMinimumY = p.y;
    //     if(p.y > touchScreenMaximumY) touchScreenMaximumY = p.y;

    //     Serial.print( "Touch x " );
    //     Serial.print( p.x );
    //     Serial.print( " y " );
    //     Serial.println( p.y );

    //     // Map this to the pixel position
    //     // data->point.x = map(p.x,touchScreenMinimumX,touchScreenMaximumX,1,screenWidth); /* Touchscreen X calibration */
    //     // data->point.y = map(p.y,touchScreenMinimumY,touchScreenMaximumY,1,screenHeight); /* Touchscreen Y calibration */
    //     // data->state = LV_INDEV_STATE_PR;

    //     // Serial.print( "Mapped Touch x " );
    //     // Serial.print( data->point.x );
    //     // Serial.print( " y " );
    //     // Serial.println( data->point.y );
    // }
    // else
    // {
    //     // data->state = LV_INDEV_STATE_REL;
    // }
}
