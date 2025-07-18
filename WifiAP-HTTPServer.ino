//LIB
#include "Arduino.h"
#include "TFT_eSPI.h"
#include "img_logo.h"
#include "pin_config.h"
#include "time.h"
#include "WiFi.h"
#include <WiFiClient.h>
#include <WebServer.h>


//DEF
#define LCD_MODULE_CMD_1
//INIT
TFT_eSPI tft = TFT_eSPI(); // Initialize the display

WebServer server(80);

//VAR
const char *ssid     = "XXXXXXXXXXXXXX";
const char *password = "TheIsTheK3yWPA-A";





#if defined(LCD_MODULE_CMD_1)
typedef struct {
    uint8_t cmd;
    uint8_t data[14];
    uint8_t len;
} lcd_cmd_t;
lcd_cmd_t lcd_st7789v[] = {
    {0x11, {0}, 0 | 0x80},
    {0x3A, {0X05}, 1},
    {0xB2, {0X0B, 0X0B, 0X00, 0X33, 0X33}, 5},
    {0xB7, {0X75}, 1},
    {0xBB, {0X28}, 1},
    {0xC0, {0X2C}, 1},
    {0xC2, {0X01}, 1},
    {0xC3, {0X1F}, 1},
    {0xC6, {0X13}, 1},
    {0xD0, {0XA7}, 1},
    {0xD0, {0XA4, 0XA1}, 2},
    {0xD6, {0XA1}, 1},
    {0xE0, {0XF0, 0X05, 0X0A, 0X06, 0X06, 0X03, 0X2B, 0X32, 0X43, 0X36, 0X11, 0X10, 0X2B, 0X32}, 14},
    {0xE1, {0XF0, 0X08, 0X0C, 0X0B, 0X09, 0X24, 0X2B, 0X22, 0X43, 0X38, 0X15, 0X16, 0X2F, 0X37}, 14},
};
#endif






void setup() {

    Serial.begin(115200);
    Serial.println("ESP32-S3-display-STRINGS1");
    Serial.println("\n[*] Creating AP");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    Serial.print("[+] AP Created with IP Gateway ");
    Serial.println(WiFi.softAPIP());
    
    pinMode(PIN_POWER_ON, OUTPUT);
    digitalWrite(PIN_POWER_ON, HIGH);


    tft.begin();
#if defined(LCD_MODULE_CMD_1)
    for (uint8_t i = 0; i < (sizeof(lcd_st7789v) / sizeof(lcd_cmd_t)); i++) {
        tft.writecommand(lcd_st7789v[i].cmd);
        for (int j = 0; j < (lcd_st7789v[i].len & 0x7f); j++) {
            tft.writedata(lcd_st7789v[i].data[j]);
        }

        if (lcd_st7789v[i].len & 0x80) {
            delay(120);
        }
    }
#endif


    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.pushImage(0, 0, 320, 170, (uint16_t *)img_logo);
    delay(1000);

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0)
    ledcSetup(0, 2000, 8);
    ledcAttachPin(PIN_LCD_BL, 0);
    ledcWrite(0, 255);
#else
    ledcAttach(PIN_LCD_BL, 200, 8);
    ledcWrite(PIN_LCD_BL, 255);
#endif

  tft.init();                      
  tft.setTextSize(3);
  tft.setRotation(3);              
  tft.fillScreen(TFT_BLACK);      
  tft.setTextColor(TFT_GREEN);     


   // Set up the routes for handling HTTP requests
   server.on("/", handleRoot);
   server.on("/sendtext", handleText);
 
   // Start the server
   server.begin();
   Serial.println("Server started");
  

}

void handleRoot() {
  // Send the HTML form to the client
  String html = "<!DOCTYPE html><html><head><title>SEND TEXT TO ESP32-S3!</title></head><body><h1>SEND TEXT TO ESP32-S3!</h1><form method='post' action='/sendtext'><label for='text'>Enter text:</label><input type='text' name='text' id='text' required><br><button type='submit'>Send</button></form></body></html>";
  server.send(200, "text/html", html);
}
void handleText() {
  if (server.method() == HTTP_POST) {
    tft.fillScreen(TFT_BLACK);
    String text = server.arg("text");
    Serial.println("Received text: " + text);
    tft.drawString(text, 12, 80, 4);

    // Send a response back to the client
    server.send(200, "text/html", "<h1>Text received</h1><p>You entered: " + text + "</p>");
  } else {
    server.send(405, "text/html", "<h1>Method Not Allowed</h1>");
  }
}




void loop() {
  server.handleClient();
}























// TFT Pin check
#if PIN_LCD_WR  != TFT_WR || \
    PIN_LCD_RD  != TFT_RD || \
    PIN_LCD_CS    != TFT_CS   || \
    PIN_LCD_DC    != TFT_DC   || \
    PIN_LCD_RES   != TFT_RST  || \
    PIN_LCD_D0   != TFT_D0  || \
    PIN_LCD_D1   != TFT_D1  || \
    PIN_LCD_D2   != TFT_D2  || \
    PIN_LCD_D3   != TFT_D3  || \
    PIN_LCD_D4   != TFT_D4  || \
    PIN_LCD_D5   != TFT_D5  || \
    PIN_LCD_D6   != TFT_D6  || \
    PIN_LCD_D7   != TFT_D7  || \
    PIN_LCD_BL   != TFT_BL  || \
    TFT_BACKLIGHT_ON   != HIGH  || \
    170   != TFT_WIDTH  || \
    320   != TFT_HEIGHT
#error  "Error! Please make sure <User_Setups/Setup206_LilyGo_T_Display_S3.h> is selected in <TFT_eSPI/User_Setup_Select.h>"
#error  "Error! Please make sure <User_Setups/Setup206_LilyGo_T_Display_S3.h> is selected in <TFT_eSPI/User_Setup_Select.h>"
#error  "Error! Please make sure <User_Setups/Setup206_LilyGo_T_Display_S3.h> is selected in <TFT_eSPI/User_Setup_Select.h>"
#error  "Error! Please make sure <User_Setups/Setup206_LilyGo_T_Display_S3.h> is selected in <TFT_eSPI/User_Setup_Select.h>"
#endif

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,0,0)
#error  "The current version is not supported for the time being, please use a version below Arduino ESP32 3.0"
#endif

