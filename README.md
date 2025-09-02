# VDU_ESP32 - Vehicle Display Unit

A professional embedded C application for ESP32 v1.17.1 with a 0.96 inch OLED display (SSD1306). This project demonstrates MISRA C compliant code structure with modular architecture, featuring OLED text display, graphics, and I2C communication.

## 🏗️ Project Architecture

This project follows **MISRA C guidelines** with a clean modular structure:

```
VDU_ESP32/
├── inc/                    # Header files
│   ├── app_config.h       # Application configuration
│   ├── app_main.h         # Main application interface
│   ├── font_data.h        # Font data definitions
│   ├── oled_driver.h      # OLED display driver
│   └── pin_config.h       # ESP32 pin configuration
├── src/                    # Source files
│   ├── app_config.c       # Application configuration implementation
│   ├── app_main.c         # Main application implementation
│   ├── font_data.c        # Font data implementation
│   ├── oled_driver.c      # OLED display driver implementation
│   └── pin_config.c       # ESP32 pin configuration implementation
├── docs/                   # Documentation
│   ├── OLED_Orientation_Guide.md
│   ├── OLED_Display_Technical_Guide.md
│   └── README.md
└── build_and_flash.bat    # Build automation script
```

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

## ✨ Features

### 🎯 Core Functionality
- **Text Display**: Shows ASCII characters using optimized 8x8 font
- **Graphics**: Pixel, line, and rectangle drawing functions
- **Hello World Demo**: Continuous display of "Hello World! ESP32 OLED"
- **I2C Communication**: Direct I2C communication without external libraries

### 🏛️ Code Quality
- **MISRA C Compliant**: Follows automotive industry coding standards
- **Modular Architecture**: Clean separation of concerns
- **Professional Documentation**: Comprehensive header documentation
- **Error Handling**: Robust error checking and logging
- **Memory Efficient**: Minimal memory footprint using ESP-IDF functions

### 🔧 Technical Features
- **Pin Configuration Management**: Centralized pin definitions
- **Font System**: Extensible character font support
- **Display Orientation**: Configurable display orientation
- **FreeRTOS Integration**: Task-based application structure

## 🚀 Application Flow

The application follows a structured initialization and execution sequence:

### Initialization Phase
1. **Application Configuration**: Initialize app settings and constants
2. **Pin Configuration**: Configure ESP32 GPIO pins for I2C and general use
3. **I2C Master Setup**: Initialize I2C communication at 400kHz
4. **OLED Display Init**: Initialize SSD1306 display with proper orientation
5. **Task Creation**: Create main application task on Core 0

### Runtime Phase
1. **Hello World Display**: Shows "Hello", "World!", and "ESP32 OLED" on separate lines
2. **Continuous Loop**: Clears and redisplays message every 1 second
3. **Error Handling**: Comprehensive error logging and recovery

## 🔨 Building and Flashing

### Using PlatformIO (Recommended)

1. **Install PlatformIO**: Install PlatformIO IDE or CLI
2. **Open Project**: Navigate to project directory
3. **Build Project**:
   ```bash
   pio run
   ```
4. **Flash to ESP32**:
   ```bash
   pio run --target upload
   ```
5. **Monitor Serial Output**:
   ```bash
   pio device monitor
   ```

### Using ESP-IDF

1. **Install ESP-IDF**: Install ESP-IDF v4.4 or later
2. **Set Environment**:
   ```bash
   . $HOME/esp/esp-idf/export.sh
   ```
3. **Build Project**:
   ```bash
   idf.py build
   ```
4. **Flash to ESP32**:
   ```bash
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

### Quick Build Script

Use the provided batch script for Windows:
```bash
build_and_flash.bat
```

## ⚙️ Configuration

### Pin Configuration (`inc/pin_config.h`)
- **I2C SDA**: GPIO 21
- **I2C SCL**: GPIO 22
- **Built-in LED**: GPIO 2
- **Button 1**: GPIO 0
- **Button 2**: GPIO 35

### I2C Settings
- **Clock Speed**: 400 kHz
- **OLED Address**: 0x3C (default for most 0.96" OLEDs)
- **Timeout**: 1000ms

### Display Settings
- **Resolution**: 128x64 pixels
- **Pages**: 8 (8 pixels per page)
- **Contrast**: 0xCF (adjustable)
- **Orientation**: Configured for proper text display

### Application Settings (`inc/app_config.h`)
- **Task Stack Size**: 4096 bytes
- **Task Priority**: 5
- **Task Core**: 0
- **Display Update Delay**: 1000ms

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

## 📁 Code Structure

### Header Files (`inc/`)
- `app_config.h`: Application configuration and constants
- `app_main.h`: Main application interface and function declarations
- `font_data.h`: Font data definitions and character support
- `oled_driver.h`: OLED display driver interface and commands
- `pin_config.h`: ESP32 pin configuration and GPIO definitions

### Source Files (`src/`)
- `app_config.c`: Application configuration implementation
- `app_main.c`: Main application logic and task management
- `font_data.c`: Font data implementation with character rendering
- `oled_driver.c`: OLED display driver with I2C communication
- `pin_config.c`: ESP32 pin configuration and GPIO setup

### Build Configuration
- `CMakeLists.txt`: Root ESP-IDF build configuration
- `src/CMakeLists.txt`: Source component build configuration
- `platformio.ini`: PlatformIO project configuration
- `build_and_flash.bat`: Windows build automation script

## 📚 Documentation

Comprehensive documentation is available in the `docs/` folder:

- **[OLED Orientation Guide](docs/OLED_Orientation_Guide.md)**: Detailed guide for display orientation and troubleshooting
- **[Technical Guide](docs/OLED_Display_Technical_Guide.md)**: Complete technical reference for the OLED system
- **[Documentation README](docs/README.md)**: Documentation structure and navigation

## 🔮 Future Enhancements

### Planned Features
- **Extended Font Support**: Additional character sets and font sizes
- **Graphics Library**: Advanced drawing functions and shapes
- **Menu System**: Interactive menu navigation
- **Animation Support**: Smooth transitions and effects
- **Touch Integration**: Support for touch-enabled displays
- **Data Logging**: Display system metrics and debug information

### Production Considerations
- **Error Recovery**: Enhanced fault tolerance and recovery mechanisms
- **Power Management**: Low-power modes and sleep functionality
- **Calibration**: Display calibration and adjustment tools
- **Testing Framework**: Automated testing and validation suite

## 🏆 MISRA C Compliance

This project follows MISRA C guidelines for automotive and safety-critical applications:

- ✅ **Block Comments**: Uses `/* */` instead of `//` comments
- ✅ **Header Documentation**: Comprehensive file and function documentation
- ✅ **Modular Design**: Clear separation of concerns
- ✅ **Error Handling**: Robust error checking and logging
- ✅ **Type Safety**: Proper type definitions and casting
- ✅ **Memory Management**: Safe memory allocation and usage

## 📄 License

This project is provided as-is for educational and testing purposes. See individual source files for detailed licensing information.

## 👨‍💻 Author

**Shantanu Kumar**  
*Vehicle Display Unit (VDU) ESP32 Project*  
*Date: 2025-02-09*