# Switchwayz Firmware Architecture

The Switchwayz firmware is the core software that enables the controller to function as a Bluetooth HID joystick with integrated NFC car scanning.  
This document outlines how the firmware is structured, how inputs are mapped, and the responsibilities of each component.

---

## HID Configuration

- **Vendor ID (VID):** `0x8971`  
- **Product ID (PID):** `0xB101`  
- **Joystick Output:** 16-bit signed integer spanning the range `0x0000 → 0xFFFF`  
- **Generic Axis 3:** Reserved for NFC car codes  
- **Buttons:** 3 digital inputs mapped to `Generic Button 1–3`  

---

## Code Structure

The firmware is modular and divided into the following components:

- **BLE HID Descriptor**  
  Defines the joystick profile and Generic Axis 3 for car codes.

- **NFC Handler**  
  Reads data from the PN532 module, validates authentication bytes, and extracts car codes.

- **Input Manager**  
  Converts joystick readings into HID-compatible 16-bit values.

- **Car Code Handler**  
  Places the 2-byte car code into Generic Axis 3 of the HID report.

- **Main Loop**  
  Continuously polls joystick + NFC input and updates the HID state.

---

## Design Notes

- All HID values are reported in **standard formats** so no custom drivers are required.  
- Modular separation (NFC, HID, joystick) makes the firmware easier to extend or port.  
- Generic HID mapping ensures wide compatibility with different hosts.