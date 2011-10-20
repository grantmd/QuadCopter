ARDUINO_DIR = /Applications/Arduino.app/Contents/Resources/Java

TARGET       = QuadCopter
BOARD_TAG    = mega2560
ARDUINO_PORT = /dev/cu.usb*

ARDUINO_LIBS = Wire Wire/utility EEPROM 

include ~/Documents/Arduino/Arduino.mk
