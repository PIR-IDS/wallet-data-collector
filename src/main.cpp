#include "Arduino.h"
#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

#define STRING_BUFFER_SIZE 6000
#define DELAY_BETWEEN_TRANSMISSIONS 400  // seems to be close to the minimum delay required to get all the data
#define NUMBER_OF_LINES_SEND 2000
//a counter to limit the number of cycles where values are displayed
int counter = 0;
int id = 0;
int dataIndex = 0;


String data[NUMBER_OF_LINES_SEND / 13 + (NUMBER_OF_LINES_SEND % 13 > 0 ? 1 : 0)];  // ble payload can only contain 13 lines of data, so we need to send 15 payload to send the NUMBER_OF_LINES_SEND lines
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
            //   been run less than NUMBER_OF_LINES_SEND times (=~3 seconds displayed)
            if (IMU.accelerationAvailable() && counter < NUMBER_OF_LINES_SEND) {
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

            // When the loop has run NUMBER_OF_LINES_SEND times, reset the counter and delay
            //   3 seconds, then print empty lines and the new datapoint sequence
            } else if (IMU.accelerationAvailable() && counter >= NUMBER_OF_LINES_SEND) {
                id++;
                for (auto & i : data) {
                    testSample.writeValue(i);
                    i = "";
                    delay(DELAY_BETWEEN_TRANSMISSIONS);
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
