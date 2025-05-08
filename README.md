# Smart Weather Clock

A real-time clock system with configurable alarms and integrated weather sensing, built on the STM32G474RE microcontroller. Combines internal and external sensors with a simple user interface and Bluetooth-based data expansion.

---

## Project Goals

This project was developed as a learning exercise with the following goals:

- Practice **bare-metal programming** using direct register access on STM32 microcontrollers
- Design the system so that most tasks are handled via **interrupts**, not the main loop — to reduce power consumption
- Learn to build embedded projects using a **build system (CMake)** instead of IDE-generated Makefiles
- Improve fluency with **Git version control** and good repository structure
- Gain experience in **PCB design**

---

## Features

- Real-Time Clock (RTC) with support for a single configurable alarm
- Weather sensing:
  - **Internal sensors**:
    - Temperature & Pressure: LPS25HB (I2C)
    - Humidity: SHT20 (I2C)
  - **External sensors** (via Bluetooth HC-05):
    - Temperature, Pressure, Humidity, Rain Detection
- Periodic weather update every 10 seconds
- Automatic low-power sleep mode:
  - System enters sleep after completing all tasks in the main loop
  - Wakes up on interrupts (e.g., timer or button press)
- User-selectable weather data source (internal or external)
- Alarm notifications:
  - Display alert
  - External buzzer module with built-in sound generator (controlled via GPIO)
- LCD display: 20x4 alphanumeric, driven by HD44780 controller
- User interface:
  - Push-buttons for navigation and settings
  - Potentiometer for adjusting LCD contrast

---

## Hardware Overview

- **Microcontroller**: STM32G474RE
- **Display**: 20x4 LCD with HD44780 controller (parallel interface)
- **Internal Sensors**:
  - LPS25HB (temperature and pressure, I2C)
  - SHT20 (humidity, I2C)
- **External Sensor Module**:
  - Communicates over UART via HC-05 Bluetooth
  - Sends temperature, pressure, humidity, and rain data
- **Buzzer Module**:
  - External buzzer with internal sound generator
  - Controlled via logic-level GPIO
- **User Inputs**:
  - Buttons:
    - OK / Mode
    - Up
    - Down
    - Left
    - Right
  - Potentiometer for LCD contrast

---

## How It Works

1. On startup, the system initializes:
   - MCU embedded peripherals (RTC, UART, GPIO, RTC etc.)
   - Internal sensors
   - LCD

3. Default behavior after power reset:
   - RTC time = 00:00:00
   - Alarm = disabled
   - Weather readings from internal sensors

4. System behavior:
   - Sensor readings are updated every 10 seconds
   - Every second current time and sensor readings are displayed on the screen
   - After processing all events and interrupts, the system enters low-power mode until the next interrupt

5. The default screen shows:
   - Current time
   - Alarm status
   - Sensor data (internal by default)

6. Pressing **OK / Mode** opens a menu with 4 options. The currently selected option is marked with `>` e.g:
```
   > 1. weather indoor  
     2. weather outdoor  
     3. set time  
     4. set alarm
```

7. Navigation:
   - **Up / Down** to move through options
   - **OK / Mode** to select
   - In `Set Time` and `Set Alarm`:
     - **Left / Right** to move between HH:MM:SS
     - **Up / Down** to adjust values

8. Alarm behavior:
   - When alarm is enabled and the current time matches the alarm:
     - "ALARM" screen is displayed
     - Buzzer is activated via GPIO

---

## User Interface

After system startup and peripheral initialization, the main screen displays:
- Current time (reset to 00:00:00 after power-off — no backup battery for RTC)
- Alarm status (disabled after power reset)
- Live weather readings from internal sensors

### Navigation Summary

- **OK / Mode**: Enter menu / confirm selection
- **Up / Down**: Scroll through menu or increment/decrement values
- **Left / Right**: Switch between hours, minutes, seconds

---

## Getting Started

1. Clone this repository.
2. Build the project using CMake
3. Flash the firmware to STM32G474RE (e.g., Nucleo board).
4. Connect all peripherals and sensors as shown in the [wiring diagram](pcb/main_board/main_board_schematic.png).


