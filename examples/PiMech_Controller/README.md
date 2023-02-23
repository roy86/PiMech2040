# PiMech Robotics Controller (RP2040 Pico W)

This is designed for the PiMech Controller Shield with an RP2040 Pico W (Wireless)

It's primary function is to read inputs from the Operator(s) and augment their inputs via a Fly-By-Wire process.
It uses an AI logic to identify the operator intents and adjust the droids functions to improve the "life like" movements beyond primary operator commands.

An example of this is the B2EMO groundmech droid from the Star Wars ANDOR series, a highly expressive droid with 3 operators puppetting it.

In the case of a single operators' intent to express "Thinking", the operator will tilt the head forward and to the right. Based on the default configuration, the AI will identify this pattern and shorten the body, slow any drum rotation and lean the droid forward a little while maintaing the head clear of the collision with the neck. This will allow more movement with less control and demonstrate more "life like" movements the operator cannot manage by themselves.

### Software Features
* Operator Augmentation for certain input patterns and idle movements
* Receiver Inputs via PPM, SBUS/iBUS
* When using SBUS/iBUS, PPM can be used as a seconadry operator input for up to 8 channels
* Configurable mapping for inputs, outputs and automation routines supporting single, single + AI, or dual operator functions
* SD storage for Configuration files.
* (Pending) WiFi Connection to specified network/hotspot to allow interaction of certain operations and configuration via Web Browsers
* More to come...

# Setup

Check wiring diagram for board connections: [RP2040 Pico W ControlShield](https://github.com/roy86/PiMech2040-Hardware/tree/main/RP2040-PicoW-ControlShield)

## SD Card Configuration

The PiMech Controller configures the entire connected system based on this configuration file.

It is important to at least have an SD card installed and if a configuration file is missing, the PiMech will generate a default one on start. This will allow you on the first bootup to create the groundmech defaults automatically and tweak the config files on your computer or via the WiFi Browser Interface (Pending)

## Installing Software on Pico W
There are two methods supplied here to upload the firmware to your PiMech Controller. The provided UF2 binarys which can be drag-drop or compile your own via the Arduino IDE.

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
