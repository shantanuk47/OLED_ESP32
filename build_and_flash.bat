@echo off
echo ESP32 OLED Test Project - Build and Flash Script
echo ================================================

echo.
echo Checking for ESP-IDF installation...
if exist "C:\esp\esp-idf\export.bat" (
    echo Found ESP-IDF in C:\esp\esp-idf
    call "C:\esp\esp-idf\export.bat"
    echo Building with ESP-IDF...
    idf.py build
    if %ERRORLEVEL% EQU 0 (
        echo Build successful! Flashing to ESP32...
        idf.py -p COM3 flash monitor
    ) else (
        echo Build failed!
    )
) else if exist "%USERPROFILE%\esp\esp-idf\export.bat" (
    echo Found ESP-IDF in user profile
    call "%USERPROFILE%\esp\esp-idf\export.bat"
    echo Building with ESP-IDF...
    idf.py build
    if %ERRORLEVEL% EQU 0 (
        echo Build successful! Flashing to ESP32...
        idf.py -p COM3 flash monitor
    ) else (
        echo Build failed!
    )
) else (
    echo ESP-IDF not found. Trying PlatformIO...
    echo Note: Make sure the 'main' folder has been renamed to 'src'
    pio run
    if %ERRORLEVEL% EQU 0 (
        echo Build successful! Flashing to ESP32...
        pio run --target upload
        echo Starting serial monitor...
        pio device monitor
    ) else (
        echo Build failed! Make sure 'main' folder is renamed to 'src'
    )
)

echo.
echo ================================================
echo Build and flash process completed.
echo.
echo If you encounter issues:
echo 1. Make sure your ESP32 is connected via USB
echo 2. Check the COM port in platformio.ini (currently set to COM3)
echo 3. Install ESP-IDF or PlatformIO if not already installed
echo 4. Check the README.md for detailed instructions
pause
