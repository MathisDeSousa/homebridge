#include <FastLED.h>
#define NUM_LEDS 20
 
#define LED_PIN 5

CRGB leds[NUM_LEDS];
int val = 255;
uint8_t paletteIndex = 0;
uint8_t BRIGHTNESS = 255;

DEFINE_GRADIENT_PALETTE (palette) {
  0, 255,   0,   0,
 75, 255,   0,   0,
128, 255, 255, 255,
120, 255,   0,   0,
255, 255,   0,   0,
};
CRGBPalette16 myPal = palette;

 void R_B(){
    FastLED.setBrightness(BRIGHTNESS);
    fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);
  
    EVERY_N_MILLISECONDS(10){
      paletteIndex++;
    } 
     
    FastLED.show();   
 }

#include <ESP8266WiFi.h>

const char* ssid = "Bbox-5FD3FD4C"; //WIFI Name, WeMo will only connect to a 2.4GHz network.
const char* password = "L7VFn7NE33ttkCEcRk"; //WIFI Password

WiFiServer server(80);
IPAddress ip(192, 168, 1, 40); // where xx is the desired IP Address
IPAddress gateway(10, 0, 0, 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

void setup() {
  Serial.begin(115200);
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) { //wait for wifi
      delay(500);
    }
  
  server.begin(); //initialize the server
    Serial.print(WiFi.localIP());
}

void loop() {
 
 WiFiClient client = server.available(); // Check if a client has connected
  if (!client) { //if no client then
    R_B();
    return;
  }

  String request = client.readStringUntil('\r'); // Read the first line of the request
  client.flush(); //erase pending data
  
  //Match the request, checking to see what the currect state is
  if (request.indexOf("/ON") != -1) {
   BRIGHTNESS = 255; 
  } 
  
  if (request.indexOf("/OFF") != -1){
  BRIGHTNESS = 0;
  }
  if (request.indexOf("/val") != -1){
  val = 0;
  }
 
R_B();

}
