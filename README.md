# Switchway

![Version](https://img.shields.io/badge/version-1.0.0--beta-blue)

---

## Overview

Switchway is a specialized controller for toy-to-life games designed to encourage **fast-paced hot swapping of physical toys**.  
It is a **Bluetooth HID device** running on battery power that uses **NFC technology** to scan toy cars and load them directly into a game.  

The design includes:
- One joystick (X/Y axis)
- Three buttons mounted directly on the stick
- NFC reader for toy car scanning
- BLE HID interface for compatibility with most platforms

This prototype was originally created in just two days for a hackathon competition. It demonstrates how physical toys can be directly mapped to digital game input without custom drivers.

---

## Core Features

- **NFC Reader**  
  Scans toy cars and communicates unique car codes to the host machine.  

- **One-Handed Play**  
  Large joystick with buttons on the stick allows gameplay with one hand, while the other hand swaps toys.  

- **Bluetooth Enabled & Battery Powered**  
  Wireless gameplay from short distances. Current prototype runs on **4×AA batteries**.  

- **Modular Design**  
  Internals are built into a uniform rectangular frame, making it simple to redesign or customize the outer casing.  

---

## Applications

Currently tailored toward **racing games**, Switchway acts as a direct HID input device.  
No firmware changes are required for standard use.  

In the future, the concept can be generalized for:
- Action/adventure games
- Puzzle or toy-to-life hybrids
- Educational and training tools  

---

## HID Configuration

- **Vendor ID (VID):** `0x8971`  
- **Product ID (PID):** `0xB101`  

| Input Type   | HID Mapping | Description                                                       |
|--------------|-------------|-------------------------------------------------------------------|
| Joystick X   | Axis 1      | Horizontal joystick movement (16-bit signed).                     |
| Joystick Y   | Axis 2      | Vertical joystick movement (16-bit signed).                       |
| Car Code     | Axis 3      | Reports NFC car code. `0x0000` when no car is present.             |
| Buttons 1–3  | Buttons     | Physical buttons mounted on the controller.                       |

---

## Future Roadmap

- [ ] Custom PCB for smaller form factor  
- [ ] NFC versioning bytes for extensible car definitions  
- [ ] More physical buttons  
- [ ] Software Development Kits (SDKs) for game integration  
- [ ] General overhaul for improved durability and usability  

---

## Installation

To flash and use the firmware:  

1. Install the **Arduino IDE**  
2. Install the **ESP32 Board Package** (via Board Manager)  
3. Install required libraries:  
   - [ESP32 BLE Gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad)  
   - [Adafruit PN532](https://github.com/adafruit/Adafruit-PN532)  
4. Clone this repository and upload the firmware to your ESP32.  

For detailed setup, see [`/firmware/README.md`](firmware/README.md).  

---

## Assembly Guide

1. Download the STL files from `/hardware`.  
2. Print and prepare the casing.  
3. Gather all components from the **Bill of Materials** in `/docs/bom.csv`.  
4. Required tools:  
   - Soldering kit  
   - Super glue (temporary use)  
   - Screwdriver set  
5. Assemble components following the wiring diagram in `/hardware/assembly.md`.  

Disclaimer: This is an early prototype, not a final consumer product. Expect fragility and tolerance issues.

---

## Support

This is a passion project by **Camden Montgomery** and **Jace Lopez Hernandez**.  
We don’t intend to commercialize it ourselves but want to keep it open source for the community.  

If you’d like to support ongoing development:  

Use the GitHub **Sponsor button** on this repo.  

Your support helps us improve the documentation, add new features, and provide tools for others to build games for Switchway.  

---

## License

This project is released under the **MIT License** (see [`LICENSE`](LICENSE) for details). 