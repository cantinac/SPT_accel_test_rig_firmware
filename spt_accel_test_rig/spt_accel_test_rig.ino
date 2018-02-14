/*
 * SPT Accelerometer Test Rig
 *
 * This is the firmware for a simple test rig for the MMA8451
 * accelerometer. The goal of the test rig is to help SPT
 * understand movement data.
 *
 * The logger reads the accelerometer at a rate of roughly 1Hz and
 * logs both the raw reading and the reading in m/s^2 (which is the SI
 * unit for acceleration). Note that this means two readings of the
 * accelerometer and it's possible the logged values with vary slightly.
 *
 * This code doesn't compensate for jitter in the accelerometer. Given
 * the low frquency of the readings (around once per second) and the
 * 10bit resolution of the MMA8451, noise in the readigs in not likely
 * to be a significant factor in gaining insight into the general levels
 * of activity.
 *
 * RTC setup note: this firmware does not set the current time
 * on the realtime clock chip. Ideally, this should be done before
 * the this skecth is run. The best way to do this is to upload the
 * sketch spt_accel_rtc_config firmware to the test rig before
 * installing this firmware. This will set the RTC to the current time
 * on the installing computer (well, within a few seconds, given the
 * time lag between sketch compliation and the upload onto the board.
 *
 *
 * The BOM for the test rig hardware is available at
 * https://docs.google.com/spreadsheets/d/15wlsBM79pBig0-j9UxoaQvt3I3dSTPN7S8ViAU0ntvs/edit?usp=sharing
 */

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <RTClib.h>
#include "datastructs.h"

// Enable or disable Serial port logging
const bool SERIAL_ENABLE = true;

// chip select pin on the Adalogger FeatherWing with the 32u4
const byte SDCS = 10;

// Set up the MMA8451 with I2C
Adafruit_MMA8451 accelerometer = Adafruit_MMA8451();

// Set up the realtime clock
RTC_DS1307 RTC;

void setup() {
  if (SERIAL_ENABLE) {
    Serial.begin(115200);
  }

  // Attempt to start up the realtime clock
  RTC.begin();
  if (!RTC.isrunning()) {
    writeLog("RTC did not start");
    while(1);
  }

  // Attempt to start up the accelerometer
  if (!accelerometer.begin()) {
    writeLog("Accelerometer did not start");
    while(1);
  }
  accelerometer.setRange(MMA8451_RANGE_8_G); // starting at +-8G, but 2 or 4 also possible

  if (!SD.begin(SDCS)) {
    writeLog("SD card did not start");
    while(1);
  }
}

void loop() {
  RawAccel ra;
  Accel a;

  readAccelerometerRawValues(&accelerometer, &ra);
  readAccelerometerAsGs(&accelerometer, &a);

  String raw = "";
  raw = raw + ra.x + "," + ra.y + "," + ra.z;

  String msquared = "";
  msquared = msquared + a.x + "," + a.y + "," + a.z;

  String output = timestamp() + raw + "," + msquared;

  writeToSD("accel.csv", output);
  Serial.println(output);

  delay(1000); // log at ~1Hz
}

/*
 * Use the Adafuit Sensor Library to get the accelerometer reading in m/s^2
 */
void readAccelerometerAsGs(Adafruit_MMA8451 *accel, Accel *a) {
  sensors_event_t event;
  accel->getEvent(&event);
  a->x = event.acceleration.x;
  a->y = event.acceleration.y;
  a->z = event.acceleration.z;
}

/*
 * Read the raw accelerometer values
 */
void readAccelerometerRawValues(Adafruit_MMA8451 *accel, RawAccel *r) {
  accel->read();
  r->x = accel->x;
  r->y = accel->y;
  r->z = accel->z;
}

/*
 * get a timetamp in seconds since UNIX epoch (Jan 1, 1970 00:00)
 * and push it to the front of the string
 */
long timestamp() {
  DateTime now = RTC.now();
  return now.unixtime();
}

void writeLog(String s) {
  String ts = timestamp() + s;
  if (SERIAL_ENABLE) {
    Serial.println(ts);
  }
  writeToSD("log.txt", ts);
}

void writeToSD(String fn, String s) {
  File file = SD.open(fn, FILE_WRITE);
  if (file) {
    file.println(s);
    file.close();
  }
}
