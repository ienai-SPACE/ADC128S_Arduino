#include "ADC128S.h"

/******************************
 * ADC128S class implementation
 ******************************/

ADC128S::ADC128S(i_ADC128S& hardware) : hw(hardware) {}

void ADC128S::begin() {
    hw.initCS();
}

/*
* Implementation note *
*-----------------------
* This ADC behaves the following way:
*    - Every time an SPI transfer is initiated sensor returns 
*      last configured channel or channel 0 as default.
*    - Sensor remembers the last channel readen
*
* Steps for reading:
*    - Set the channel and preform a dummy reading
*    - Set the same channel and perform the actual reading
*/

uint16_t ADC128S::readChannel(uint8_t channel) {

    uint8_t hi = 0;
    uint8_t lo = 0;
    uint16_t result = 0;

    hw.selectCS();

    //Prepare the channel to be readen
    hw.SPI_transfer(channel << 3);
    hw.SPI_transfer(0x00);

    //reade the channel
    hi = hw.SPI_transfer(channel << 3);
    lo = hw.SPI_transfer(0x00);

    hw.deselectCS();

    result = ((hi << 8) | lo) & 0x0FFF; // Combine into 12-bit result (mask off any garbage bits)
    result += 1; // Lab offset correction

    return result; 
}

void ADC128S::readAll(uint16_t* buffer, size_t length) {
    // User logic to read all channels

    uint8_t hi = 0;
    uint8_t lo = 0;

    hw.selectCS();

    //Prepare the channel to be readen
    hw.SPI_transfer(0 << 3);
    hw.SPI_transfer(0x00);

    if (length >= ADC128S_NUM_CHANNELS) 
    {
        for (uint8_t i = 0; i < ADC128S_NUM_CHANNELS; i++)
        {
            hi = hw.SPI_transfer((i + 1) % ADC128S_NUM_CHANNELS << 3); // ends up with channel 0 set
            lo = hw.SPI_transfer(0x00);
            buffer [i] = (((hi << 8) | lo) & 0x0FFF) + 1; // combine and make the lab correction
        }
    }

    hw.deselectCS();
}

/***************************************
 * ADC128S_Arduino class implementation
 ***************************************/

ADC128S_Arduino::ADC128S_Arduino(SPIClass& spiBus, uint8_t cs)
    : spi(spiBus), csPin(cs) {}

void ADC128S_Arduino::initCS() {
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH); // deselect CS
}

void ADC128S_Arduino::selectCS() {
    digitalWrite(csPin, LOW);
}

void ADC128S_Arduino::deselectCS() {
    digitalWrite(csPin, HIGH);
}

uint8_t ADC128S_Arduino::SPI_transfer(uint8_t data) {
    return spi.transfer(data);
}
