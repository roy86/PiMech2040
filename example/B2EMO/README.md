# PiMech2040
RP2040 based Astromech and Groundmech Controller using the Pimoroni Servo2040 board

1. Install support for RP2040 for Arduino CLI
https://github.com/earlephilhower/arduino-pico

1. Install support for RP2040 Servos
https://github.com/RadekVoltr/RP2040PioServos

2. Set Servo2040 to Boot Mode
3. Select board in Arduino CLI (May need to show all ports)
4. Upload Sketch

# Transmitter Channels
Drive Mode:
1. FWD/BKWD
2. STRAFE
3. -
4. Turn
Stationary Mode
1. -
2. Head Turn
3. Head Pitch
4. Head Roll 

All Modes
5. Mode Select (Binary) Low: Stationary, High: Drive