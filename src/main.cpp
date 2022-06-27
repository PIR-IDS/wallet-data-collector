#include "Arduino.h"
#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

#define STRING_BUFFER_SIZE 2000
#define DELAY_BETWEEN_TRANSMISSIONS 2000 // en ms
#define NUMBER_OF_LINES_SEND 96 // 256 pour une sortie de portefeuille, 2000 pour du negatif
#define SAMPLE_RATE_TARGET 25 // the rate expected to run the model training

//a counter to limit the number of cycles where values are displayed

int counter = 0;
int sample_skip_counter;

float sample_rate_IMU = IMU.accelerationSampleRate(); // the arduino nano 33 BLE sample rate is supposed to be119Hz
int sample_every_n = static_cast<int>(roundf(sample_rate_IMU / SAMPLE_RATE_TARGET));

String data;  // ble payload can only contain 13 lines of data, so we need to send 15 payload to send the NUMBER_OF_LINES_SEND lines
BLEService userData("181C");
BLEStringCharacteristic testSample("2A3D",
                                   BLERead | BLENotify,    // remote clients will be able to get notifications if this characteristic changes
                                   STRING_BUFFER_SIZE);
BLEBooleanCharacteristic idCharacteristic("2AE2",
                                   BLERead | BLENotify);    // remote clients will be able to get notifications if this characteristic changes


void setup()
{
    /* ------ INITIALIZE BLE ------ */

    if(!BLE.begin()) {
    }

    BLE.setDeviceName("PIR-IDS Card");
    BLE.setLocalName("PIR-IDS");

    // Generic Access Control (the closest one to an IDS)
    // see: https://specificationrefs.bluetooth.com/assigned-values/Appearance%20Values.pdf
    BLE.setAppearance(0x0700);
    userData.addCharacteristic(testSample); // string
    userData.addCharacteristic(idCharacteristic); // bool

    // add the String service
    BLE.addService(userData);

    /* ------ ADVERTISE ------ */

    // start advertising
    BLE.advertise();

    // Accelerometer setup
    if (!IMU.begin()) {
    }

    data = String(counter%10) + "-,-,-,-,-,-\n";
}

void loop()
{
    float ax, ay, az, gx, gy, gz;
    int xx, yy, zz, gxx, gyy, gzz;

    BLEDevice central = BLE.central();

    if (central) {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    if (central.connected())
    {
        testSample.writeValue("\n");
    }

    while (central.connected()) {
        if (central.connected()) {
            // Check if the accelerometer is ready and if the loop has only
            //   been run less than NUMBER_OF_LINES_SEND times (=~3 seconds displayed)
            if (counter < NUMBER_OF_LINES_SEND) {
                // Read the accelerometer
                while (!IMU.accelerationAvailable()) {}
                if (!IMU.readAcceleration(ax, ay, az)) {
                    break;
                }
                if (IMU.gyroscopeAvailable()) {
                    IMU.readGyroscope(gx, gy, gz);
                }
                // Scale up the values to better distinguish movements
                xx = (int) (ax * 1000);
                yy = (int) (ay * 1000);
                zz = (int) (az * 1000);
                
                gxx = (int) (gx * 5);
                gyy = (int) (gy * 5);
                gzz = (int) (gz * 5);

                data = data + String(xx) + "," + String(yy) + "," + String(zz) + "," + String(gxx) + "," + String(gyy) + "," + String(gzz);
                //data = data + String(counter) + "," + String(sample_rate_IMU) + "," + String(zz);

                // Skip the next values to record at the proper sample rate
                sample_skip_counter = 0;
                for (int index = 1; index < sample_every_n; index++) {
                    while (!IMU.accelerationAvailable()) {}
                    if (!IMU.readAcceleration(ax, ay, az)) {
                        break;
                    }
                }

                if (counter != 0 && counter % 5 == 0) {
                    if (!central.connected()) {
                        break;
                    }
                    testSample.writeValue(data);
                    data = String(counter%10);
                } else {
                    data = data + "\n";
                }
                // Increment the counter
                counter++;

            // When the loop has run NUMBER_OF_LINES_SEND times, reset the counter and delay
            //   3 seconds, then print empty lines and the new datapoint sequence
            } else {
                delay(DELAY_BETWEEN_TRANSMISSIONS);
                if (!central.connected()) {
                    break;
                }
                testSample.writeValue("stop");
                delay(500);
                if (!central.connected()) {
                    break;
                }
                // print a count to warn the user on when to execute the movements
                for (int i = 3; i>0;i--) {
                    if (!central.connected()) {
                        break;
                    }
                    testSample.writeValue(String(i) + "\n");
                    delay(1000);
                }
                counter = 0;
                data = String(7) + "-,-,-,-,-,-\n";

            }
        }

        if (!central.connected()) {
            digitalWrite(LED_BUILTIN, LOW);
            break;
        }
    }
}
