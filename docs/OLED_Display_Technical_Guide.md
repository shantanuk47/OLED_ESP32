# OLED Display Technical Guide

## Table of Contents
1. [Overview](#overview)
2. [Hardware Specifications](#hardware-specifications)
3. [Communication Protocol](#communication-protocol)
4. [Display Memory Structure](#display-memory-structure)
5. [Code Architecture](#code-architecture)
6. [Function Reference](#function-reference)
7. [Data Flow](#data-flow)
8. [Font System](#font-system)
9. [Performance Considerations](#performance-considerations)
10. [Troubleshooting](#troubleshooting)

## Overview

This project implements a complete OLED display driver for the SSD1306 controller using ESP32. The system provides text display capabilities with custom font rendering and orientation control.

### Key Features
- I2C communication with SSD1306 OLED controller
- Custom 8x8 pixel font system
- Character rotation and mirroring
- Display orientation control
- Memory-efficient implementation

## Hardware Specifications

### SSD1306 OLED Controller
- **Display Size**: 128x64 pixels
- **Interface**: I2C (also supports SPI)
- **Voltage**: 3.3V - 5V
- **Current**: ~20mA active, ~10μA sleep
- **Address**: 0x3C (default) or 0x3D

### ESP32 Configuration
- **I2C Pins**: GPIO21 (SDA), GPIO22 (SCL)
- **Pull-up Resistors**: 4.7kΩ (external)
- **Clock Speed**: 400kHz (fast mode)
- **Voltage Level**: 3.3V

## Communication Protocol

### I2C Frame Structure
```
Start | Address | R/W | ACK | Data | ACK | ... | Stop
```

### Command vs Data
```c
// Command (Control Byte: 0x00)
i2c_cmd_handle_t cmd = i2c_cmd_link_create();
i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
i2c_master_write_byte(cmd, 0x00, true);  // Control byte for command
i2c_master_write_byte(cmd, command, true);

// Data (Control Byte: 0x40)
i2c_cmd_handle_t cmd = i2c_cmd_link_create();
i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
i2c_master_write_byte(cmd, 0x40, true);  // Control byte for data
i2c_master_write_byte(cmd, data, true);
```

### Key SSD1306 Commands
| Command | Value | Description |
|---------|-------|-------------|
| Display ON/OFF | 0xAE/0xAF | Turn display off/on |
| Set Memory Addressing Mode | 0x20 | Set addressing mode |
| Set Column Address | 0x21 | Set column start/end |
| Set Page Address | 0x22 | Set page start/end |
| Set Start Line | 0x40-0x7F | Set display start line |
| Set Contrast | 0x81 | Set contrast level |
| Segment Remap | 0xA0/0xA1 | Horizontal flip |
| COM Scan Direction | 0xC0/0xC8 | Vertical flip |
| Set COM Pins | 0xDA | COM pin configuration |
| Set Display Clock | 0xD5 | Set display clock divide |
| Set Precharge | 0xD9 | Set precharge period |
| Set VCOMH | 0xDB | Set VCOMH deselect level |

## Display Memory Structure

### Memory Layout
```
SSD1306 Memory (128x64 pixels):
- 8 pages (0-7), each 8 pixels tall
- 128 columns (0-127)
- Each page contains 128 bytes
- Total memory: 1024 bytes (128x8)

Page 0: Pixels 0-7
Page 1: Pixels 8-15
...
Page 7: Pixels 56-63
```

### Pixel Addressing
```c
// For 8x8 font at position (x,y):
// x: column (0-15 for 8-pixel wide characters)
// y: row (0-7 for 8-pixel tall characters)

page = y;           // Page number
column = x * 8;     // Starting column
```

## Code Architecture

### File Structure
```
src/
├── main.c              # Main application and OLED driver
├── CMakeLists.txt      # Build configuration
platformio.ini          # PlatformIO configuration
docs/                   # Documentation
├── OLED_Orientation_Guide.md
└── OLED_Display_Technical_Guide.md
```

### Main Components

#### 1. Initialization Layer
```c
void app_main()                    // ESP-IDF entry point
├── i2c_master_init()             // Initialize I2C bus
├── oled_init()                   // Initialize OLED display
└── xTaskCreate(app_main_task)    // Create main application task
```

#### 2. Communication Layer
```c
oled_write_cmd(uint8_t cmd)       // Send command to OLED
oled_write_data(uint8_t data)     // Send data to OLED
```

#### 3. Display Control Layer
```c
oled_clear()                      // Clear entire display
oled_set_cursor(uint8_t x, uint8_t y)  // Set text cursor position
```

#### 4. Text Rendering Layer
```c
oled_write_char(char c)           // Write single character
oled_write_string(const char* str) // Write string
```

#### 5. Graphics Layer
```c
oled_draw_pixel(uint8_t x, uint8_t y)     // Draw single pixel
oled_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)  // Draw line
oled_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)      // Draw rectangle
```

## Function Reference

### Core Functions

#### `oled_init()`
Initializes the SSD1306 OLED display with proper configuration.
```c
esp_err_t oled_init(void)
```
**Process:**
1. Power on display
2. Set fundamental commands (oscillator, multiplex, offset)
3. Configure orientation (segment remap, COM scan)
4. Set display parameters (contrast, precharge, VCOMH)
5. Enable display

#### `oled_write_char(char c)`
Renders a single character with rotation and mirroring.
```c
esp_err_t oled_write_char(char c)
```
**Process:**
1. Look up character in font data
2. Apply horizontal mirroring
3. Rotate 90° counter-clockwise
4. Write 8 bytes to display memory

#### `oled_write_string(const char* str)`
Writes a null-terminated string to the display.
```c
esp_err_t oled_write_string(const char* str)
```
**Process:**
1. Iterate through each character
2. Call `oled_write_char()` for each character
3. Advance cursor position

### Graphics Functions

#### `oled_draw_pixel(uint8_t x, uint8_t y)`
Sets a single pixel on the display.
```c
esp_err_t oled_draw_pixel(uint8_t x, uint8_t y)
```

#### `oled_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)`
Draws a line between two points using Bresenham's algorithm.
```c
esp_err_t oled_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
```

#### `oled_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)`
Draws a rectangle outline.
```c
esp_err_t oled_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
```

## Data Flow

### Text Rendering Pipeline
```
String Input
    ↓
Character Lookup (switch statement)
    ↓
Font Data (8x8 bytes)
    ↓
Horizontal Mirroring (font_data[j] & (1 << i))
    ↓
90° Counter-clockwise Rotation
    ↓
8 Bytes Output
    ↓
I2C Data Transmission
    ↓
SSD1306 Memory Update
    ↓
Display Update
```

### Memory Update Process
```
1. Set cursor position (page, column)
2. Send data bytes via I2C
3. SSD1306 updates internal memory
4. Display refreshes automatically
```

## Font System

### Font Data Structure
Each character is defined as 8 bytes (8x8 pixels):
```c
case 'A': 
    font_data[0] = 0x0C;  // Row 0: ░░██░░░░
    font_data[1] = 0x1E;  // Row 1: ░███░░░░
    font_data[2] = 0x33;  // Row 2: ██░░██░░
    font_data[3] = 0x33;  // Row 3: ██░░██░░
    font_data[4] = 0x3F;  // Row 4: ██████░░
    font_data[5] = 0x33;  // Row 5: ██░░██░░
    font_data[6] = 0x33;  // Row 6: ██░░██░░
    font_data[7] = 0x00;  // Row 7: ░░░░░░░░
```

### Character Transformation
```c
// Original font data (8x8)
// Apply horizontal mirroring: (1 << i) instead of (1 << (7-i))
// Apply 90° counter-clockwise rotation
// Result: Properly oriented character for display
```

### Supported Characters
- **Uppercase**: A-Z
- **Lowercase**: a-z (limited set)
- **Numbers**: 0-9
- **Symbols**: Space, !, and others as needed

## Performance Considerations

### Memory Usage
- **RAM**: ~11KB (3.4% of 327KB)
- **Flash**: ~200KB (19.2% of 1MB)
- **Font Data**: ~2KB for complete character set

### I2C Performance
- **Clock Speed**: 400kHz (fast mode)
- **Data Rate**: ~40KB/s theoretical
- **Character Update**: ~1ms per character
- **Full Screen Update**: ~25ms

### Optimization Strategies
1. **Batch Updates**: Group multiple characters
2. **Partial Updates**: Update only changed areas
3. **Font Caching**: Cache frequently used characters
4. **DMA**: Use DMA for large data transfers

## Troubleshooting

### Common Issues

#### 1. Display Not Responding
**Symptoms**: No display output, I2C errors
**Solutions**:
- Check I2C connections (SDA, SCL, VCC, GND)
- Verify I2C address (0x3C or 0x3D)
- Check pull-up resistors (4.7kΩ)
- Verify power supply (3.3V)

#### 2. Garbled Text
**Symptoms**: Random characters or patterns
**Solutions**:
- Check font data integrity
- Verify character transformation algorithm
- Ensure proper cursor positioning
- Check I2C data transmission

#### 3. Wrong Orientation
**Symptoms**: Text appears rotated or mirrored
**Solutions**:
- Adjust OLED initialization commands
- Modify font transformation algorithm
- See OLED_Orientation_Guide.md for details

#### 4. Performance Issues
**Symptoms**: Slow updates, flickering
**Solutions**:
- Increase I2C clock speed
- Implement double buffering
- Use DMA for data transfer
- Optimize font rendering

### Debug Tools

#### Serial Output
```c
ESP_LOGI(TAG, "Displaying Hello World...");
ESP_LOGE(TAG, "I2C write failed: %s", esp_err_to_name(ret));
```

#### I2C Scanner
```c
// Scan for I2C devices
for (uint8_t addr = 0x08; addr < 0x78; addr++) {
    if (i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true) == ESP_OK) {
        printf("Found device at address 0x%02X\n", addr);
    }
}
```

## Best Practices

### Code Organization
1. **Modular Design**: Separate initialization, communication, and rendering
2. **Error Handling**: Check return values and handle errors gracefully
3. **Documentation**: Comment complex algorithms and transformations
4. **Testing**: Test with simple patterns before complex graphics

### Performance
1. **Minimize I2C Calls**: Batch operations when possible
2. **Efficient Fonts**: Use compact font representations
3. **Memory Management**: Avoid dynamic allocation
4. **Power Management**: Turn off display when not needed

### Maintenance
1. **Version Control**: Track changes to orientation settings
2. **Backup Configurations**: Save working parameter sets
3. **Documentation**: Keep technical notes updated
4. **Testing**: Regular validation of display functionality

## Conclusion

This OLED display system provides a solid foundation for embedded display applications. The modular architecture allows for easy extension and modification. The comprehensive orientation control and font system enable flexible text and graphics rendering.

For specific orientation issues, refer to the OLED_Orientation_Guide.md. For implementation details and advanced features, consult the source code and this technical guide.
