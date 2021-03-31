/*  FILE    : MPU6050.h
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

    DESCRIPTION : 
    This file holds defines for accessing the MPU6050 IMUs various registers. You can find a more detailed look at the register map here : https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf .
*/

#ifndef THUNK_MPU6050_H
#define THUNK_MPU6050_H

#define PWR_MGMT_1         0x6B // Power Management 1 - DEVICE_RESET , SLEEP , CYCLE , - , TEMP_DIS , CLKSEL[2:0]
#define PWR_MGMT_2         0x6C // Power Management 2 - LP_WAKE_CTRL[1:0] , STBY_XA , STBY_YA , STBY_ZA , STBY_XG , STBY_YG , STBY_ZG
#define SIGNAL_PATH_RESET  0x68 // Reset signals : - , - , - , - , - , GYRO_RESET , ACCEL_RESET , TEMP_RESET
#define ACCEL_CONFIG       0x1C // Setup high pass filter for IMU readings : XA_ST , YA_ST , ZA_ST , AFS_SEL[1:0]
#define MOT_THR            0x1F // Motion detection threshold : Unsigned 8 bit value.
#define MOT_DUR            0x20 // Motion detection duration
#define ZRMOT_THR          0x21 // Zero motion threshhold
#define ZRMOT_DUR          0x22 // Zero motion duration
#define FF_THR             0x1D // Freefall threshold
#define FF_DUR             0x1E // Freefall duration
#define MOT_DETECT_CTRL    0x69 // Motion detection control : - , - , ACCEL_ON_DELAY[1:0] , FF_COUNT[1:0] , MOT_COUNT[1:0]
#define INT_ENABLE         0x38 // Interrupt Enable : FF_EN , MOT_EN , ZMOT_EN , FIFO_OVERFLOW_EN , I2C_MST_INT_EN , - , - , DATA_RDY_EN
#define INT_STATUS         0x3A // Interrupt Status : FF_INT , MOT_INT , ZMOT_INT , FIFO_OFLOW_INT , I2C_MST_INT , - , - , DATA_RDY_INT
#define INT_PIN_CFG        0x37 // Interrupt Signal Config : INT_LEVEL , INT_OPEN , LATCH_INT_EN , INT_RD_CLEAR , FSYNC_INT_LEVEL , FSYNC_INT_EN , I2C_BYPASS_EN , -

#endif

