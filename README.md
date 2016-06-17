# SPT Accelerometer Test Rig Firmware

This is the firmware for a simple test rig for the MMA8451
accelerometer. The goal of the test rig is to help SPT
understand movement data.

The logger reads the accelerometer at a rate of roughly 1Hz and
logs both the raw reading and the reading in m/s^2 (which is the SI
unit for acceleration). Note that this means two readings of the
accelerometer and it's possible the logged values with vary slightly.

This code doesn't compensate for jitter in the accelerometer. Given
the low frequency of the readings (around once per second) and the
10bit resolution of the MMA8451, noise in the readings in not likely
to be a significant factor in gaining insight into the general levels
of activity.

## Installing the firmware

First, clone this repo somewhere on your local drive. I recommend cloning it right into your Arduino sketchbook location (e.g. `~/Documents/Arudino` on OS X).

This firmware was written in Arduino IDE 1.6.9, and is assumed to run on the [Adafruit Feather 32u4](https://www.adafruit.com/products/2771) with the [FeatherWing Adalogger](https://www.adafruit.com/products/2922) and a [MMA8451 accelerometer](https://www.adafruit.com/products/2019) attached to the I2C interface.

### Getting dependencies

To install or modify the firmware, you'll need to set up a few dependencies in the Arduion IDE. First, you need to the Adafruit AVR board definitons. To install this, open `Preferences` and add the following line into the `Additional Board Manager URLs`:

```
https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
```

Once you've done that, you'll be able to select the Feather 32u4 as a target board for compiling and uploading.

You will also need to install some libraries. Select the `Sketch` menu and choose `Include Library` &rarr; `Manage Libraries…`. You need to install the following libraries:

* RTClib
* Adafruit MMA8451
* Adafruit Unified Sensor

Once these libraries are installed, close the Library Manager.

### Setting up the RTC
Once you've got the dependencies installed, open the sketch `spt_accel_rtc_config` from the location you cloned this repo into. From the `Tools` menu select "Adafruit Feather 32u4" and set the port to the USB port you've connected Feather to (if you didn't plug the Feather into a USB port, you should do that now!).

Open the serial monitor in the IDE, and set it to 115200 baud. *It's important that you do this before uploading the firmware sketch!* We're going to set the hardware clock in the RTC, and we want ensure that ends up as close to real time as possible. Also, opening the serial monitor causes the Feather to reset, and we don't want that once the sketch is uploaded.

Once you've got the board connected and the serial monitor open, upload the sketch. In a few seconds you should see the board start to report the time as quickly as it can via the serial interface.

### Installing the test rig firmware
Now it's time to install the main firmware for the board. Open the `spt_accel_test_rig` sketch and upload it to the Feather.

Once the upload is complete, the firmware will begin logging raw accelerometer reads and the values in SI units of acceleration (m/s^2) to the file `accel.csv`. Each entry row starts with a timestamp in seconds since the UNIX epoch, that is January 1, 1970 @ 00:00:00 GMT.

## Some notes on the firmware
The test rig firmware logs both raw and SI acceleration units. This should help translate the raw data into something more meaningful, while still preserving the raw values.

## Additional information

The BOM for the test rig hardware is available at https://docs.google.com/spreadsheets/d/15wlsBM79pBig0-j9UxoaQvt3I3dSTPN7S8ViAU0ntvs/edit?usp=sharing
