# ESP32 OLED Test Project

A simple test application for ESP32 v1.17.1 with a 0.96 inch OLED display (SSD1306). This project demonstrates basic OLED functionality including text display, graphics, and I2C communication.

## Hardware Requirements

- ESP32 Development Board
- 0.96 inch OLED Display (SSD1306, 128x64 pixels)
- Jumper wires
- Breadboard (optional)

## Pin Connections

| ESP32 Pin | OLED Pin | Description |
|-----------|----------|-------------|
| GPIO 21   | SDA      | I2C Data Line |
| GPIO 22   | SCL      | I2C Clock Line |
| 3.3V      | VCC      | Power Supply |
| GND       | GND      | Ground |

## Features

- **Text Display**: Shows ASCII characters using a simple 8x8 font
- **Graphics**: Basic pixel, line, and rectangle drawing functions
- **Test Patterns**: Automated test sequences to verify display functionality
- **I2C Communication**: Direct I2C communication without external libraries
- **Minimal Memory Usage**: Uses standard ESP-IDF functions, no external libraries

## Test Sequence

The application runs a continuous test sequence that includes:

1. **Text Display Test**: Shows "ESP32 OLED Test", "Hello World!", numbers, and "Test Complete"
2. **Pattern Test**: Draws rectangles, lines, and pixel patterns
3. **Simple Message**: Displays "ESP32 Working!" message

## Building and Flashing

### Using ESP-IDF

1. Install ESP-IDF v4.4 or later
2. Set up the environment:
   ```bash
   . $HOME/esp/esp-idf/export.sh
   ```
3. Build the project:
   ```bash
   idf.py build
   ```
4. Flash to ESP32:
   ```bash
   idf.py -p /dev/ttyUSB0 flash monitor
   ```
   (Replace `/dev/ttyUSB0` with your actual port)

### Using PlatformIO

1. Install PlatformIO
2. Open the project in PlatformIO
3. Build and upload:
   ```bash
   pio run --target upload
   ```
4. Monitor serial output:
   ```bash
   pio device monitor
   ```

## Configuration

### I2C Settings
- **Clock Speed**: 400 kHz
- **SDA Pin**: GPIO 21
- **SCL Pin**: GPIO 22
- **OLED Address**: 0x3C (default for most 0.96" OLEDs)

### Display Settings
- **Resolution**: 128x64 pixels
- **Pages**: 8 (8 pixels per page)
- **Contrast**: 0xCF (adjustable)

## Troubleshooting

### Display Not Working
1. Check pin connections (SDA to GPIO 21, SCL to GPIO 22)
2. Verify power supply (3.3V to VCC, GND to GND)
3. Check I2C address (try 0x3D if 0x3C doesn't work)
4. Ensure proper pull-up resistors (usually built into ESP32)

### Compilation Errors
1. Make sure ESP-IDF is properly installed and sourced
2. Check that you're using ESP-IDF v4.4 or later
3. Verify all files are in the correct directory structure

### Upload Issues
1. Check USB cable connection
2. Verify correct COM port (Windows) or device path (Linux/Mac)
3. Try holding BOOT button while uploading
4. Check if drivers are installed for your ESP32 board

## Code Structure

- `main/main.c`: Main application code with OLED functions
- `CMakeLists.txt`: ESP-IDF build configuration
- `main/CMakeLists.txt`: Component build configuration
- `platformio.ini`: PlatformIO configuration

## Future Enhancements

This is a basic test implementation. For production use, consider:

- Custom font support
- Image display capabilities
- Animation functions
- Menu system
- Touch input (if using touch-enabled display)
- External library integration (Adafruit SSD1306, etc.)

## License

This project is provided as-is for educational and testing purposes.