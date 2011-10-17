# A rake file for compiling and uploading this Arduino project
# Created by Myles Grant <myles@mylesgrant.com>
# Adapted from: https://github.com/AndrewVos/arduino-tools

require 'tempfile'
require 'fileutils'

PROJECT          = "Quadcopter"
MCU              = 'atmega328p'
CPU              = '16000000L'
PORT             = Dir.glob('/dev/tty.usbmodem*').first
BITRATE          = '115200'
PROGRAMMER       = 'stk500v1'

CWD              = File.dirname(__FILE__)
BUILD_OUTPUT     = 'build'
ARDUINO_HARDWARE = '/Applications/Arduino.app/Contents/Resources/Java/hardware'
AVRDUDE          = "#{ARDUINO_HARDWARE}/tools/avr/bin/avrdude"
AVRDUDE_CONF     = "#{ARDUINO_HARDWARE}/tools/avr/etc/avrdude.conf"
ARDUINO_CORES    = "#{ARDUINO_HARDWARE}/arduino/cores/arduino"
ARDUINO_WIRE     = "#{ARDUINO_HARDWARE}/../libraries/Wire"
ARDUINO_TWI      = "#{ARDUINO_HARDWARE}/../libraries/Wire/utility"
ARDUINO_EEPROM   = "#{ARDUINO_HARDWARE}/../libraries/EEPROM"
AVR_G_PLUS_PLUS  = "#{ARDUINO_HARDWARE}/tools/avr/bin/avr-g++"
AVR_GCC          = "#{ARDUINO_HARDWARE}/tools/avr/bin/avr-gcc"
AVR_AR           = "#{ARDUINO_HARDWARE}/tools/avr/bin/avr-ar"
AVR_OBJCOPY      = "#{ARDUINO_HARDWARE}/tools/avr/bin/avr-objcopy"

def build_output_path(file)
  Dir.mkdir(BUILD_OUTPUT) if File.exists?(BUILD_OUTPUT) == false
  File.join(BUILD_OUTPUT, file)
end

PDE_FILES        = ["#{PROJECT}.pde", "FlightCommand.pde", "FlightControl.pde", "SerialControl.pde"]
C_FILES          = Dir.glob("#{ARDUINO_CORES}/*.c") + Dir.glob("#{ARDUINO_WIRE}/*.c") + Dir.glob("#{ARDUINO_TWI}/*.c") + Dir.glob("#{ARDUINO_EEPROM}/*.c") + Dir.glob("#{CWD}/*.c")
CPP_FILES        = Dir.glob("#{ARDUINO_CORES}/*.cpp") + Dir.glob("#{ARDUINO_WIRE}/*.cpp") + Dir.glob("#{ARDUINO_EEPROM}/*.cpp") + Dir.glob("#{CWD}/*.cpp") + [build_output_path("#{PROJECT}.cpp")]

desc "Compile and upload"
task :default => [:compile, :upload]

desc "Compile the hex file"
task :compile => [:clean, :preprocess, :c, :cpp, :hex]

desc "Upload compiled hex file to your device"
task :upload do
  hex = build_output_path("#{PROJECT}.hex")
  sh "#{AVRDUDE} -C#{AVRDUDE_CONF} -q -q -p#{MCU} -c#{PROGRAMMER} -P#{PORT} -b#{BITRATE} -D -Uflash:w:#{hex}:i"
end

desc "Delete the build output directory"
task :clean do
  FileUtils.rm_rf(BUILD_OUTPUT)
end

task :preprocess do
  cpp = build_output_path("#{PROJECT}.cpp")

  File.open(cpp, 'w') do |file|
    file.puts '#include "WProgram.h"'
  end

  PDE_FILES.each do |source|
    File.open(cpp, 'a') do |file|
      file.puts File.read(source)
    end
  end
end

task :c do
  C_FILES.each do |source|
    output = build_output_path(File.basename(source, File.extname(source)) + ".o")
    sh "#{AVR_GCC} -c -g -Os -w -ffunction-sections -fdata-sections -mmcu=#{MCU} -DF_CPU=#{CPU} -DARDUINO=22 -I#{CWD} -I#{ARDUINO_CORES} -I#{ARDUINO_WIRE} -I#{ARDUINO_TWI} -I#{ARDUINO_EEPROM} #{source} -o#{output}"
  end
end

task :cpp do
  CPP_FILES.each do |source|
    output = build_output_path(File.basename(source, File.extname(source)) + ".o")
    sh "#{AVR_G_PLUS_PLUS} -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=#{MCU} -DF_CPU=#{CPU} -DARDUINO=22 -I#{CWD} -I#{ARDUINO_CORES} -I#{ARDUINO_WIRE} -I#{ARDUINO_TWI} -I#{ARDUINO_EEPROM} #{source} -o#{output}"
  end
end

task :hex do
  o       = build_output_path("#{PROJECT}.o")
  elf     = build_output_path("#{PROJECT}.elf")
  archive = build_output_path('core.a')
  eep     = build_output_path("#{PROJECT}.eep")
  hex     = build_output_path("#{PROJECT}.hex")

  (C_FILES + CPP_FILES).each do |file|
    file = build_output_path(File.basename(file, File.extname(file)) + ".o")
    sh "#{AVR_AR} rcs #{archive} #{file}"
  end

  sh "#{AVR_GCC} -Os -Wl,--gc-sections -mmcu=#{MCU} -o #{elf} #{o} #{archive} -L#{BUILD_OUTPUT} -lm"
  sh "#{AVR_OBJCOPY} -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 #{elf} #{eep}"
  sh "#{AVR_OBJCOPY} -O ihex -R .eeprom #{elf} #{hex}"
end
