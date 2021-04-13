/*  FILE    : main.cpp
    AUTHOR  : Lina Adkins
    LICENSE :
    This file is part of the Open Source Thunk cube timer project at https://github.com/BenjaminHolland/Thunk !
    Copyright © 2021 Lina Adkins, Benjamin Holland

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), 
    to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
    THE SOFTWARE.
*/

#include <Arduino.h>    // For setup/loop and basic arduinolib funcs
#include <SPI.h>        // SPI For primary display
#include <Wire.h>       // I2C For MPU
#include <esp_bt.h>     // For disabling bluetooth
#include <esp_bt_main.h>// For disabling bluetooth MORE
#include <esp_wifi.h>   // For disabling wifi

#include <TM1637Display.h>
// Primary Display
//#define TFT_MISO MISO
//#define TFT_MOSI MOSI
//#define TFT_SCLK SCK
//#define TFT_CS    16 // Chip select control pin
//#define TFT_DC    17  // Data Command control pin
//#define TFT_RST   4  // Reset pin (could connect to RST pin)
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip


#define TFT_GREY 0x5AEB // New colour
TFT_eSPI tft = TFT_eSPI();  // Invoke library
int tft_brightness = 255;
bool tft_dirty = false;

// IMU - MPU6050
#include "IMU.h"
Thunk::IMU g_imu;
bool interruptTriggered = false;
void IRAM_ATTR imu_callback() {	// Our interrupt
	interruptTriggered = true;
    g_imu.DetachInterrupt(); // Detach interrupt so we dont get interrupted over and over
}

// 7 Seg
// We share the clock and use individal pins for DIO
TM1637Display disp_top(25, 14);
TM1637Display disp_left(25, 13);
TM1637Display disp_bot(25, 19);
TM1637Display disp_right(25, 33);

// Speaker
void play_tone(note_t, int, int);
#define BUZZER_PIN 27
#define BUZZER_CHANNEL 1

// Power save/sleep vars
void disable_radio();

#define MAIN_LOOP_TIMESTEP 100 // Run main loop every 100ms when awake

void setup(){

    // Serial
    Serial.begin(115200);
    Serial.println("Booting...");

    // Disable bt and wifi
    disable_radio();

    // Primary Display
    tft.init();
    tft.setRotation(2);

    // Setup pwm pin for powering display
    ledcAttachPin(26 , 0);
    ledcSetup(0, 4000, 8);

    // Lower our cpu frequency
    setCpuFrequencyMhz(40);

    // Play startup beep
    play_tone( note_t::NOTE_C , 5 , 100 );

    // Enable I2C
    Wire.begin();

    // IMU
    g_imu = Thunk::IMU();
    g_imu.Initialize();
    g_imu.AttachInterrupt(imu_callback);

    // 7 Segment Displays
    disp_top.setBrightness(4);
    disp_top.showNumberDec(1234);
    disp_left.setBrightness(4);
    disp_left.showNumberDec(5678);
    disp_bot.setBrightness(4);
    disp_bot.showNumberDec(1337);
    disp_right.setBrightness(4);
    disp_right.showNumberDec(8008);
}

void loop(){
  // Time gate our loop
  static uint32_t timeLast = 0;
  if (millis() - timeLast >= MAIN_LOOP_TIMESTEP)
  {
    timeLast = millis();

    tft.fillScreen(TFT_GREY);
    ledcWrite(0, tft_brightness);
    
    // Set cursor at top with font 2
    tft.setCursor(0, 0, 2);
    
    // Sample Text
    tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
    tft.printf("This is just some test text!");

    // Deal with interrupt being triggered
    if(interruptTriggered){
        play_tone(note_t::NOTE_B , 2 , 250);
        interruptTriggered = false;
        Serial.print(g_imu.GetRegister(0x3A));
        Serial.print(",");
        Serial.print(g_imu.GetRegister(0x37));
        Serial.print("\n");

        // We're done so re-attach interrupt
        g_imu.AttachInterrupt(imu_callback);
    }
  }
}


/**
 Aggressively disables bluetooth and wifi to save power.
*/
void disable_radio(){
    delay(10);

    // Kill BT
    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();

    // Kill WiFi
    esp_wifi_set_mode(WIFI_MODE_NULL);
    esp_wifi_stop();
    esp_wifi_deinit();
}

/**
 Plays a single tone on the buzzer channel.
 */
void play_tone(note_t note , int octave , int duration){

    if(ledcRead(BUZZER_CHANNEL)){
        Serial.printf("Tone channel %d is already in use!\n" , ledcRead(BUZZER_CHANNEL));
        return;
    }

    ledcAttachPin(BUZZER_PIN , BUZZER_CHANNEL);

    ledcWriteNote(BUZZER_CHANNEL , note , octave );
    delay(duration);
    ledcDetachPin(BUZZER_PIN);
    ledcWrite(BUZZER_CHANNEL , 0);
}