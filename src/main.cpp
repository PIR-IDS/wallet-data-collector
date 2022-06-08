#include "Arduino.h"
#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

#define STRING_BUFFER_SIZE 6000
//a counter to limit the number of cycles where values are displayed
int counter = 0;
int id = 0;
int dataIndex = 0;


String data[16];  // ble payload can only contain 13 lines of data, so we need to send 15 payload to send the 200 lines
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

    data[0] = "-,-,-\n";
}

void loop()
{
    float x, y, z;
    int xx, yy, zz;

    BLEDevice central = BLE.central();

    if (central) {
        digitalWrite(LED_BUILTIN, HIGH);
    }

    while (central.connected()) {
        if (central.connected()) {
            // Check if the accelerometer is ready and if the loop has only
            //   been run less than 200 times (=~3 seconds displayed)
            if (IMU.accelerationAvailable() && counter < 200) {
                // Read the accelerometer
                IMU.readAcceleration(x, y, z);
                // Scale up the values to better distinguish movements
                xx = (int) (x * 1000);
                yy = (int) (y * 1000);
                zz = (int) (z * 1000);

                data[dataIndex] = data[dataIndex] + String(xx) + "," + String(yy) + "," + String(zz);

                if (counter != 0 && counter % 13 == 0) {
                    dataIndex++;
                } else {
                    data[dataIndex] = data[dataIndex] + "\n";
                }

            // When the loop has run 200 times, reset the counter and delay
            //   3 seconds, then print empty lines and the new datapoint sequence
            } else if (IMU.accelerationAvailable() && counter >= 200) {
                id++;
                for (auto & i : data) {
                    testSample.writeValue(i);
                    i = "";
                    delay(1000);
                    if (!central.connected()) {
                        break;
                    }
                }
                testSample.writeValue("stop");
                counter = 0;
                data[0] = "-,-,-\n";
                dataIndex = 0;
            }
            // Increment the counter and delay .01 seconds
            counter++;
            delay(10);
        }

        if (!central.connected()) {
            digitalWrite(LED_BUILTIN, LOW);
            break;
        }
    }
}
