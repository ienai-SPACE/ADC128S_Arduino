#include <Arduino.h>
#include <SPI.h>
#include <ADC128S.h>


// Define SPI bus and CS pin
#define CS_PIN 10

// Create hardware backend
ADC128S_Arduino adcHardware(SPI, CS_PIN);

// Create ADC library instance
ADC128S adc(adcHardware);

void setup() {
    Serial.begin(115200);

    SPI.begin();
    adc.begin();

    Serial.println("ADC128S example started");
}

void loop() {
    // Example: read single channel
    uint16_t value = adc.readChannel(0);
    Serial.print("Channel 0 value: ");
    Serial.println(value);

    // Example: read all channels
    const size_t numChannels = 8; // change to actual number
    uint16_t buffer[numChannels];
    adc.readAll(buffer, numChannels);
    Serial.print("All channels: ");
    for (size_t i = 0; i < numChannels; i++) {
        Serial.print(buffer[i]);
        Serial.print(" ");
    }
    Serial.println();

    delay(1000);
}