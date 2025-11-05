#ifndef ADC128S_H
#define ADC128S_H

#include <Arduino.h>
#include <SPI.h>

#define ADC128S_NUM_CHANNELS 8

/**
 * @brief Abstract interface for ADC128S hardware access.
 * 
 * Defines the low-level operations required by the ADC128S,
 * independent of the platform. Any platform-specific implementation
 * must inherit from this interface.
 */
class i_ADC128S {
public:
    virtual void initCS() = 0;             /**< Initialize CS pin */
    virtual void selectCS() = 0;           /**< Pull CS low */
    virtual void deselectCS() = 0;         /**< Pull CS high */
    virtual uint8_t SPI_transfer(uint8_t data) = 0; /**< Transfer one byte via SPI */
    
    virtual ~i_ADC128S() {}
};

/**
 * @brief ADC128S library class.
 * 
 * Implements the logic of the ADC128S. Uses a hardware interface
 * to perform SPI operations. Supports reading a single channel
 * or all channels at once.
 */
class ADC128S {
private:
    i_ADC128S& hw; /**< Reference to the hardware interface */
    uint8_t _lastChannelReaden;

public:
    /**
     * @brief Construct a new ADC128S object.
     * 
     * @param hardware Reference to an object implementing i_ADC128S
     */
    ADC128S(i_ADC128S& hardware);

    /**
     * @brief Initialize the ADC hardware (CS pin).
     */
    void begin();

    /**
     * @brief Read a single channel from the ADC.
     * 
     * @param channel Channel number to read
     * @return uint16_t Raw value (logic to be implemented by user)
     */
    uint16_t readChannel(uint8_t channel);

    /**
     * @brief Read all channels at once.
     * 
     * @param buffer Pointer to store results
     * @param length Length of buffer (number of channels)
     */
    void readAll(uint16_t* buffer, size_t length);
};

/**
 * @brief Arduino-specific implementation of the hardware interface.
 * 
 * Implements i_ADC128S using Arduino SPI and digital I/O.
 */
class ADC128S_Arduino : public i_ADC128S {
private:
    SPIClass& spi;   /**< SPI bus to use */
    uint8_t csPin;   /**< Chip Select pin */

public:
    /**
     * @brief Construct a new Arduino ADC128S hardware object.
     * 
     * @param spiBus SPI bus reference (e.g., SPI)
     * @param cs CS pin number
     */
    ADC128S_Arduino(SPIClass& spiBus, uint8_t cs);

    void initCS() override;
    void selectCS() override;
    void deselectCS() override;
    uint8_t SPI_transfer(uint8_t data) override;
};

#endif // ADC128S_H
