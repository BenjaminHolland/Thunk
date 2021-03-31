/*  FILE    : IMU.cpp
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

#include "Wire.h"

#include "IMU.h"

void Thunk::IMU::Initialize(){

    Wire.begin();

    SetRegister( PWR_MGMT_1 , 0x00); // Reset device
    SetRegister( SIGNAL_PATH_RESET, 0x07); // Set sleep mode to reset gyro, accel, temp
    SetRegister( ACCEL_CONFIG, 0x01); // +- 4g high pass filter set
    SetRegister( MOT_THR, 3);  // Threshhold for enabling motion detection
    SetRegister( MOT_DUR, 40); // Motion detection duration
    SetRegister( MOT_DETECT_CTRL, 0x15); // Enable motion detection and set settings for
    SetRegister( INT_ENABLE, 0x40); // Enable motion detection
    SetRegister( INT_PIN_CFG, 160); // Make sure int pin is active low

    // Setup our interrupt pin
    pinMode(IMU_INTERRUPT_PIN , INPUT_PULLUP);

    // Reset interrupt on MPU6050 if it's still set
    if(digitalRead(IMU_INTERRUPT_PIN) == 0){
        ResetInterruptState();
    }

    IsInitialized = true;
}

void Thunk::IMU::SetRegister(uint8_t address, uint8_t data ){
    Wire.begin();
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

uint8_t Thunk::IMU::GetRegister(uint8_t address){
  uint8_t data;
  Wire.beginTransmission(IMU_ADDRESS);
  Wire.write(address);
  Wire.endTransmission(false); // Request register data
  Wire.requestFrom(IMU_ADDRESS, (uint8_t) 1); // Read one byte
  data = Wire.read();
  return data; 
}

void Thunk::IMU::AttachInterrupt(imu_callback_fp callback){
    attachInterrupt(IMU_INTERRUPT_PIN, callback, FALLING );
}

void Thunk::IMU::DetachInterrupt(){
    detachInterrupt(IMU_INTERRUPT_PIN);
}

void Thunk::IMU::ResetInterruptState(){
    // When these registers are requested then the IMU considers its interrupt state to be fulfilled. We use this instead of latching.
    GetRegister(INT_STATUS);
    GetRegister(INT_PIN_CFG);
}