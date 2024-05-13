/*

 * ***Libraries To Download***

    ESP32 Board
    Arduino Websockets - Gil Maimon
    ESP32 AnalogWrite - Abdelouahed Errouaguy
    ArduinoJson - Benoit Blanchon

  ESP32 websocket code from Rob Hall
  Bot based on Daniel Shiffman's Twitter Bot
  Servo for ESP32 inspired by Robojax

*/
//--------------------------------------------- LIBRARIES --------------------------------------------

// Include the libraries we installed
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <analogWrite.h>
#include <ArduinoJson.h>
#include <Servo_ESP32.h>

// adafruit
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif


//--------------------------------------------- VARIABLES --------------------------------------------

// adafruit
#define PIXEL_PIN 2     // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 16  // Number of NeoPixels

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ400);
int oldState = 0;
int mode = 0;  // Currently-active animation mode, 0-9

// wifi SSID
const char* ssid = "Superbien";
const char* password = "superbien2017";
const char* websockets_server_host = "192.168.1.154";
const uint16_t websockets_server_port = 3000;
const String boardName = "board1";

using namespace websockets;
WebsocketsClient client;

//const int analogOutPin = 33; // LED for control
const int redPin = 12;
const int greenPin = 14;
const int bluePin = 27;
bool isLEDon = false;  //for control LED

//const int numOfServos = 5;
//static const int motorPins[numOfServos] = {26, 25, 32, 27, 12};  //array of pins
//Servo_ESP32 servos[numOfServos];  // declare servos from the library class
//Servo_ESP32 servos[numOfServos];  // declare servos from the library class

//int rotAngle[] = {0, 0, 0, 0, 0};
//int angleStep[] = {1, 1, 1, 1, 1};
//int minAngle[] = {0, 0, 0, 0, 0 };
//int maxAngle[] = {10, 10, 10, 10, 10};  // relatively small angle as we just want a light shake

int randNum = 0;


//--------------------------------------------- SETUP --------------------------------------------

void setup() {


  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);

  // adafruit
  strip.begin();  // Initialize NeoPixel strip object (REQUIRED)
  strip.show();   // Initialize all pixels to 'off'


  for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");
    delay(1000);
    Serial.println(WiFi.localIP());
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("NO WIFI LOSER");
    return;
    ESP.restart();
  }

  Serial.println("YAY INTERNET");

  //connect to the server
  bool connected = client.connect(websockets_server_host, websockets_server_port, "/");

  if (connected) {
    Serial.println("Connected to server");


    // make a JSON object and send it to the server
    // make space in memory for this document:

    DynamicJsonDocument doc(1024);

    // add some components:
    doc["device"] = boardName;  //same as {device: boardName, action: "INIT"} in java
    doc["action"] = "INIT";


    // create a string for our JSON to go into
    String output;

    //serializeJson turns out object into a string
    serializeJson(doc, output);

    //send the object turned string
    client.send(output);

    // Serial.println(output); //check what it looks like

  } else {
    // in case we didn't connect
    Serial.println("BAD BAD BOY");
  }

  //if we receive a message!
  client.onMessage([&](WebsocketsMessage message) {  //[&] capturing all by reference, [=] capturing all by value
    Serial.println(message.data());

    //make space in memory
    DynamicJsonDocument doc(1024);

    //turn the string into an array we can read
    deserializeJson(doc, message.data());

    //read the array
    String action = doc["action"];

    if (action == "toggleLED") {
      Serial.println("toggling LED");
      isLEDon = true;
      randNum = int(random(9));
      Serial.println(randNum);
      //if (isLEDon == true) {
      //analogWrite(analogOutPin, 1023);
      //} else {
      //analogWrite(analogOutPin, 0);
      //}
    }

  });



  //--------------------------------------------- SETUP SERVOS --------------------------------------------

  // for (int i = 0; i < numOfServos; i++) {
  //   servos[i].attach(motorPins[i]);
  // }
}

//--------------------------------------------- EVENTS AND LOOPS --------------------------------------------

void loop() {
  // put your main code here, to run repeatedly:
  // adafruit
  int newState = randNum;

  if (newState != oldState) {
    delay(20);
    // if (++mode > 8) mode = 0;  // Advance to next mode, wrap around after #8
    // switch (mode) {            // Start the new animation...
    //   case 0:
    //     colorWipe(strip.Color(0, 0, 0), 50);  // Black/off
    //     break;
      //case 1:
        colorWipe(strip.Color(255, 0, 0), 50);  // Red
      //  break;
    //   case 2:
    //     colorWipe(strip.Color(0, 255, 0), 50);  // Green
    //     break;
    //   case 3:
    //     colorWipe(strip.Color(0, 0, 255), 50);  // Blue
    //     break;
    //   case 4:
    //     theaterChase(strip.Color(127, 127, 127), 50);  // White
    //     break;
    //   case 5:
    //     theaterChase(strip.Color(127, 0, 0), 50);  // Red
    //     break;
    //   case 6:
    //     theaterChase(strip.Color(0, 0, 127), 50);  // Blue
    //     break;
    //   case 7:
    //     rainbow(10);
    //     break;
    //   case 8:
    //     theaterChaseRainbow(50);
    //     break;
    // }

    oldState = newState;
  }

  // if (isLEDon == false) {
  //   analogWrite(redPin, 0);
  //   analogWrite(greenPin, 0);
  //   analogWrite(bluePin, 0);

  //    if (client.available()) {
  //      client.poll();  //poll the client to check for any messages
  //
  //      for (int i = 0; i < numOfServos; i++) {
  //        servos[i].detach();   // some of the servos are continuous and need to be manually stopped
  //      }
  //    }
  //}

  // if (isLEDon == true) {

  //   analogWrite(redPin, 50);
  //   analogWrite(greenPin, 210);
  //   analogWrite(bluePin, 150);
  //   delay(700);

  //   analogWrite(redPin, 100);
  //   analogWrite(greenPin, 100);
  //   analogWrite(bluePin, 255);
  //   delay(700);

  //   analogWrite(redPin, 220);
  //   analogWrite(greenPin, 500);
  //   analogWrite(bluePin, 70);
  //   delay(700);


  //
  //    shake();
  //    for (int i = 0; i < numOfServos; i++) {
  //      servos[i].detach(); // some of the servos are continuous and need to be manually stopped
  //      delay(10);
  //      servos[randNum].attach(motorPins[randNum]);
  //      rotAngle[randNum] += angleStep[randNum];
  //      if (rotAngle[randNum] >= maxAngle[randNum] || rotAngle[randNum] <= minAngle[randNum]) {
  //        angleStep[randNum] *= -1;
  //      }
  //      servos[randNum].write(rotAngle[randNum]);
  //    }
  //    delay(5000);
  //    isLEDon = false;
  //    analogWrite(analogOutPin, 0);
  //  }
}

//--------------------------------------------- FUNCTIONS --------------------------------------------

//int shake() {
//
//  randNum  = int(random(numOfServos));
//  Serial.println(randNum);
//  return randNum;


//



//servos[num].detach();
//}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, color);               //  Set pixel's color (in RAM)
    strip.show();                                //  Update strip to match
    delay(wait);                                 //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) {   // Repeat 10 times...
    for (int b = 0; b < 3; b++) {  //  'b' counts from 0 to 2...
      strip.clear();               //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color);  // Set pixel 'c' to value 'color'
      }
      strip.show();  // Update strip with new contents
      delay(wait);   // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 3 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();  // Update strip with new contents
    delay(wait);   // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;           // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++) {   // Repeat 30 times...
    for (int b = 0; b < 3; b++) {  //  'b' counts from 0 to 2...
      strip.clear();               //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int hue = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue));  // hue -> RGB
        strip.setPixelColor(c, color);                        // Set pixel 'c' to value 'color'
      }
      strip.show();                 // Update strip with new contents
      delay(wait);                  // Pause for a moment
      firstPixelHue += 65536 / 90;  // One cycle of color wheel over 90 frames
    }
  }
}
