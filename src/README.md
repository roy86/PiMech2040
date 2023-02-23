# PiMech Robotics C++ Library for the Raspberry Pi RP2040 Pico and Pico W

These libraries are designed for RP2040 based PiMech Hardware and should not be used outside of the specifically designed ecosystem.

## Installing Software on Pico
There are two methods to upload the firmware to your PiMech RP2040 Pico. The provided UF2 binarys which can be drag-drop or compile your own via the Arduino IDE.

### 1. Loading the prebuilt UF2
Put your Pico into FS / bootloader mode by holding down the BOOTSEL button whilst plugging it into your PC - it should now show up as a drive called RPI-RP2. Once you've navigated to your chosen uf2 file, you can just drag it over to the RPI-RP2 drive on the left to copy it to your Pico - simples.

### 2. Compiling and loading via Arduino IDE
[Arduino IDE Setup](https://github.com/earlephilhower/arduino-pico#installing-via-arduino-boards-manager)

[Uploading Sketches](https://github.com/earlephilhower/arduino-pico#uploading-sketches)

# Licence

MIT License

Copyright (c) 2023 Roy86

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
