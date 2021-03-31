/*  FILE    : IMU.h
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

#ifndef THUNK_IMU_H
#define THUNK_IMU_H

// BEGIN IMU CONFIG

#define IMU_ADDRESS 0x68
#define IMU_INTERRUPT_PIN 32

// END IMU CONFIG

#include "MPU6050.h"

#include <inttypes.h>

typedef void (*imu_callback_fp)(); // Function pointer type for callbacks.

namespace Thunk{

    /**
     The IMU class handles the Accelerometer/Gyro of choice in a semi-high level, abstract way. With this we can initialize, set registers, read registers and keep track of the state of the IMU.
     Currently we are making use of the MPU6050 IMU.
    */
    class IMU{
        public:

        IMU(){ /* empty */ }

        /**
         Initializes ore re-initializes the IMU and resets the interrupt state.

        @param address The address for the register you'd like to write to.
        @param data The data you'd like to write to the address.
        */
        void Initialize();

        /**
         Writes a byte of data to the specified register. This is generally how we will configure the IMU.

        @param address The address for the register you'd like to write to.
        @param data The data you'd like to write to the address.
        */
        void SetRegister(uint8_t address , uint8_t data);

        /**
         Reads a byte from the specified register on the IMU.

        @param address The address for the register you'd like to read.
        @return The value contained in the register. 
        */
        uint8_t GetRegister(uint8_t address);

        /**
         Attach an interrupt callback to handle when the IMU is moved or another interrupt is called.

        @param callback A callback of type (void IRAM_ATTR*) is required. Please also make sure that this function is in global scope.
        */
        void AttachInterrupt(imu_callback_fp callback);

        /**
         Detach an interrupt to preven it from being called. Generally we will do this immediately after the interrupt is called to avoid another ISR being triggered.

        */
        void DetachInterrupt();

        /**
         Reset the interrupt state for the IMU, which should bring it to high for inactive and low for active.
         
         */
        void ResetInterruptState();

        bool IsInitialized = false;

        private:

    };
}

#endif