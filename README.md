# ADC128S Arduino Library

[![Version](https://img.shields.io/badge/version-1.17.0-blue.svg)](https://github.com/ienai-SPACE/ADC128S_Arduino)
[![Arduino](https://img.shields.io/badge/platform-Arduino-00979D.svg)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/license-Open%20Source-green.svg)](https://github.com/ienai-SPACE/ADC128S_Arduino)

This library provides an easy-to-use interface for controlling the **ADC128S** 8-channel, 12-bit analog-to-digital converter (ADC) over SPI. The library features a clean hardware abstraction layer that makes it easy to port to different platforms while maintaining full compatibility with Arduino.

## Features

- **8-channel ADC support**: Read from all 8 analog input channels
- **12-bit resolution**: High precision analog measurements
- **SPI interface**: Fast serial communication
- **Hardware abstraction**: Platform-independent design for easy porting
- **Simple API**: Easy to use functions for reading single or multiple channels
- **Arduino compatible**: Works with all Arduino-compatible boards

## Hardware Requirements

- **ADC128S** 8-channel ADC chip
- Arduino-compatible microcontroller (Arduino Uno, Nano, Mega, ESP32, etc.)
- SPI bus connection (MOSI, MISO, SCK)
- One GPIO pin for Chip Select (CS)

## Installation

### Arduino Library Manager

1. Open the Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search for "ADC128S" or "IENAI ADC128S"
4. Click **Install**

### Manual Installation

1. Download the latest release from the [GitHub repository](https://github.com/ienai-SPACE/ADC128S_Arduino)
2. Extract the ZIP file
3. In Arduino IDE, go to **Sketch** → **Include Library** → **Add .ZIP Library**
4. Select the extracted folder

### PlatformIO

```ini
lib_deps = 
    ienai-SPACE/ADC128S_Arduino@^1.17.0
```

## Wiring

Connect the ADC128S to your Arduino as follows:

| ADC128S Pin | Arduino Pin | Description |
|-------------|-------------|-------------|
| VDD        | 3.3V or 5V  | Power supply (check ADC128S datasheet) |
| GND        | GND         | Ground |
| CS         | Digital pin | Chip Select (e.g., pin 10) |
| DIN        | MOSI (11)   | SPI Data In |
| DOUT       | MISO (12)   | SPI Data Out |
| CLK        | SCK (13)    | SPI Clock |
| CH0-CH7    | Analog signals | Analog input channels |

**Note**: Pin numbers in parentheses are for Arduino Uno. Check your board's SPI pins.

## Usage

### Basic Example

```cpp
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
    // Read a single channel
    uint16_t value = adc.readChannel(0);
    Serial.print("Channel 0 value: ");
    Serial.println(value);
    
    delay(1000);
}
```

### Reading All Channels

```cpp
void loop() {
    // Read all 8 channels at once
    const size_t numChannels = 8;
    uint16_t buffer[numChannels];
    
    adc.readAll(buffer, numChannels);
    
    Serial.print("All channels: ");
    for (size_t i = 0; i < numChannels; i++) {
        Serial.print("CH");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(buffer[i]);
        Serial.print("  ");
    }
    Serial.println();
    
    delay(1000);
}
```

## API Reference

### Classes

#### `ADC128S`

Main library class for interfacing with the ADC128S chip.

**Constructor:**
```cpp
ADC128S(i_ADC128S& hardware)
```
Creates an ADC128S instance using the provided hardware interface.

**Methods:**

- `void begin()`
  - Initializes the ADC hardware (CS pin setup)
  - Call this once in `setup()`

- `uint16_t readChannel(uint8_t channel)`
  - Reads a single ADC channel
  - **Parameters:**
    - `channel`: Channel number (0-7)
  - **Returns:** 12-bit ADC value (0-4095)
  - **Note:** The library handles the required dummy read cycle internally

- `void readAll(uint16_t* buffer, size_t length)`
  - Reads all channels sequentially
  - **Parameters:**
    - `buffer`: Pointer to array to store results
    - `length`: Number of channels to read (should be 8)
  - **Note:** Results are stored in `buffer[0]` through `buffer[length-1]`

#### `ADC128S_Arduino`

Arduino-specific hardware implementation class.

**Constructor:**
```cpp
ADC128S_Arduino(SPIClass& spiBus, uint8_t cs)
```
Creates an Arduino hardware interface instance.

**Parameters:**
- `spiBus`: SPI bus reference (e.g., `SPI`, `SPI1`)
- `cs`: Chip Select pin number

## How It Works

The ADC128S has a specific read protocol:
1. The ADC remembers the last channel that was read
2. Each SPI transfer returns data from the previously configured channel
3. To read a channel, you must first configure it (dummy read), then read it

This library handles this complexity automatically. When you call `readChannel()`, it:
1. Performs a dummy read to configure the channel
2. Performs the actual read to get the value
3. Returns the 12-bit value with proper masking

## Platform Independence

The library uses a hardware abstraction interface (`i_ADC128S`) that allows easy porting to different platforms. To use this library on a non-Arduino platform:

1. Implement the `i_ADC128S` interface with your platform's SPI functions
2. Pass your implementation to the `ADC128S` constructor
3. Use the library as normal

## Troubleshooting

### No readings or wrong values
- Verify SPI connections (MOSI, MISO, SCK, CS)
- Check that CS pin is correctly configured
- Ensure SPI.begin() is called before adc.begin()
- Verify power supply voltage matches ADC128S requirements

### SPI communication issues
- Some boards require different SPI pins (e.g., ESP32)
- Check your board's SPI pin mapping
- Ensure SPI bus is not shared with other devices without proper CS management

### Incorrect channel readings
- Channel numbers are 0-7 (not 1-8)
- The ADC returns 12-bit values (0-4095), but you may need to convert to voltage based on your reference

## Converting ADC Values to Voltage

The ADC returns raw 12-bit values. To convert to voltage:

```cpp
float referenceVoltage = 3.3; // or 5.0, depending on your setup
uint16_t adcValue = adc.readChannel(0);
float voltage = (adcValue / 4095.0) * referenceVoltage;
```

## Examples

See the `examples` folder for complete working examples:
- `readADC`: Basic example showing single and multi-channel reads

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This library is open source. Please check the LICENSE file for details.

## Credits

- **Author**: Alvaro Torijano
- **Maintainer**: Alvaro Torijano <alvaro.torijano@ienai.space>
- **Organization**: IENAI SPACE

## Support

For issues, questions, or contributions, please visit the [GitHub repository](https://github.com/ienai-SPACE/ADC128S_Arduino).

---

**Version**: 1.17.0
