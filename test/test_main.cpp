#include <unity.h>

void setUp() {
}

void tearDown() {
}

void test_init() {
    TEST_ASSERT_TRUE(true);
}

#ifdef ARDUINO

#include <Arduino.h>
#include <Wire.h>

void setup() {
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_init);

    UNITY_END();
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_init);

    UNITY_END();

    return 0;
}

#endif // ARDUINO