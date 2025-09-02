# OLED Display Orientation Guide

## Overview
This document explains the OLED display orientation configuration and troubleshooting for the ESP32 OLED Display System project. The system uses a modular, MISRA C compliant architecture with dedicated OLED driver and pin configuration modules.

## Hardware Configuration
- **Display**: SSD1306 0.96" OLED (128x64 pixels)
- **Interface**: I2C (SDA, SCL, VCC, GND)
- **Controller**: ESP32 v1.17.1

## I2C Connection
```
ESP32    →    OLED
GPIO21   →    SDA
GPIO22   →    SCL
3.3V     →    VCC
GND      →    GND
```

**Note**: Pin configuration is managed in `inc/pin_config.h` and `src/pin_config.c` for centralized pin management.

## OLED Initialization Commands

### Key Commands for Orientation Control

#### 1. Segment Remap (Horizontal Flip)
```c
OLED_CMD_SEG_REMAP | 0x0  // Normal horizontal orientation
OLED_CMD_SEG_REMAP | 0x1  // Horizontally flipped
```

#### 2. COM Scan Direction (Vertical Flip)
```c
OLED_CMD_COM_SCAN_INC  // Normal vertical scan (top to bottom)
OLED_CMD_COM_SCAN_DEC  // Reversed vertical scan (bottom to top)
```

#### 3. COM Pin Configuration
```c
OLED_CMD_SET_COM_PINS
0x02  // Alternative COM pin configuration
0x12  // Standard COM pin configuration
```

## Current Working Configuration

The project uses the following configuration for correct display orientation:

```c
// In src/oled_driver.c - oled_init() function
ret |= oled_write_cmd(OLED_CMD_SEG_REMAP | 0x1);  // Horizontal flip
ret |= oled_write_cmd(OLED_CMD_COM_SCAN_DEC);     // Vertical scan decrement
ret |= oled_write_cmd(OLED_CMD_SET_COM_PINS);
ret |= oled_write_cmd(0x12);                      // Standard COM pin config
```

## Font Rendering and Character Orientation

### Font Data Transformation
The project uses a custom font rendering approach to ensure characters display correctly:

```c
// In src/oled_driver.c - oled_write_char() function
// Mirror the font data horizontally first, then rotate 90 degrees counter-clockwise
for (int i = 0; i < 8; i++) {
    uint8_t rotated_byte = 0;
    for (int j = 0; j < 8; j++) {
        if (font_data[j] & (1 << i)) { // Use i instead of (7-i) to mirror horizontally
            rotated_byte |= (1 << j);
        }
    }
    ret |= oled_write_data(rotated_byte);
}
```

### Key Points:
- **Horizontal Mirroring**: `(1 << i)` instead of `(1 << (7-i))` mirrors characters left-to-right
- **90° Rotation**: The algorithm rotates each character 90 degrees counter-clockwise
- **Font Data**: Each character is defined as 8x8 pixel pattern in `font_data[]` array

## Troubleshooting Orientation Issues

### Common Problems and Solutions

#### 1. Text Appears Mirrored
**Problem**: Characters look like "T32T" instead of "TEST"
**Solution**: Adjust font rendering algorithm
```c
// Wrong: if (font_data[j] & (1 << (7-i)))
// Correct: if (font_data[j] & (1 << i))
```

#### 2. Text Appears Upside Down
**Problem**: Text is readable but inverted vertically
**Solution**: Change COM scan direction
```c
// Try: OLED_CMD_COM_SCAN_INC instead of OLED_CMD_COM_SCAN_DEC
```

#### 3. Text Placement is Wrong
**Problem**: Characters are in correct orientation but positioned incorrectly
**Solution**: Adjust segment remap
```c
// Try: OLED_CMD_SEG_REMAP | 0x0 instead of OLED_CMD_SEG_REMAP | 0x1
```

#### 4. Characters are Rotated Wrong
**Problem**: Individual characters need different rotation
**Solution**: Modify font transformation algorithm
```c
// For 90° clockwise: if (font_data[7-j] & (1 << i))
// For 90° counter-clockwise: if (font_data[j] & (1 << i))
// For 180°: if (font_data[7-j] & (1 << (7-i)))
```

## Testing Different Orientations

### Method 1: Display Commands Only
Modify only the OLED initialization commands in `oled_init()`:
```c
// Test different combinations
ret |= oled_write_cmd(OLED_CMD_SEG_REMAP | 0x0);  // or 0x1
ret |= oled_write_cmd(OLED_CMD_COM_SCAN_INC);     // or OLED_CMD_COM_SCAN_DEC
```

### Method 2: Font Rendering Only
Modify only the font transformation in `oled_write_char()`:
```c
// Test different font transformations
if (font_data[j] & (1 << i))           // Current working version
if (font_data[j] & (1 << (7-i)))       // Alternative
if (font_data[7-j] & (1 << i))         // Another alternative
```

### Method 3: Combined Approach
Test combinations of both display commands and font rendering.

## Coordinate System

### Display Memory Layout
```
SSD1306 Memory Map (128x64):
Row 0: [0,0] to [127,0]
Row 1: [0,1] to [127,1]
...
Row 63: [0,63] to [127,63]
```

### Character Positioning
```c
oled_set_cursor(x, y);  // x: column (0-15), y: row (0-7 for 8x8 font)
```

## Best Practices

1. **Test with Simple Text**: Use "TEST", "1234", "ABCD" for orientation testing
2. **One Change at a Time**: Modify only one parameter per test
3. **Document Changes**: Keep track of what works and what doesn't
4. **Use Serial Output**: Monitor ESP_LOGI messages for debugging
5. **Backup Working Code**: Save working configurations before experimenting

## Reference Commands

### SSD1306 Command Definitions
```c
#define OLED_CMD_SEG_REMAP       0xA0
#define OLED_CMD_COM_SCAN_INC    0xC0
#define OLED_CMD_COM_SCAN_DEC    0xC8
#define OLED_CMD_SET_COM_PINS    0xDA
```

### Common COM Pin Configurations
- `0x02`: Alternative COM pin configuration
- `0x12`: Standard COM pin configuration (recommended)
- `0x22`: Alternative configuration for some displays

## Conclusion

The current configuration provides correct orientation for the ESP32 SSD1306 OLED display. The key factors are:
1. Proper OLED initialization commands
2. Correct font data transformation
3. Appropriate character positioning

For different displays or mounting orientations, adjust the parameters systematically using the troubleshooting guide above.
