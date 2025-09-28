Switchwayz Firmware

This repository contains the firmware for the Switchwayz Controller, a proof-of-concept one-handed game controller that integrates NFC toy car scanning with Bluetooth HID input. The goal of this project is to show how physical toys can directly interact with digital games through a simple, extensible interface.

HID Configuration

Vendor ID (VID): 0x8971

Product ID (PID): 0xB101

Joystick Output: 16-bit signed integer spanning the full range 0x0000 → 0xFFFF

Generic Axis 3: Reserved for NFC car codes

NFC Car Code Specification

Car Codes: Any 4-digit hex value (e.g. 0xABCD)

Location: Read from page 9 of the NFC tag’s data

Format: Raw bytes, not NDEF formatted

Structure:

Authentication marker: 0xCAB1 (2 bytes)

Car code: 2 bytes immediately following authentication marker

Example (Page 9 Data):

[...][0xCA][0xB1][0xAB][0xCD]
             ^Auth   ^Car Code


HID Mapping: Car code is reported to the host on Generic Axis 3.

This system allows for ~60,000 unique cars to be represented.

Adding Car Support in Software

To integrate new toy cars into a game or application:

Write the car’s authentication bytes + car code to page 9 of the NFC tag as raw bytes.

Pair the Switchwayz controller with your software via BLE HID.

When a scan occurs, read Generic Axis 3 from the HID profile to retrieve the car’s code.

Map the car code to the desired in-game behavior (e.g., spawn a new vehicle, unlock abilities, etc.).

Code Structure

The firmware is organized into the following components:

BLE HID Descriptor: Defines the joystick profile and Generic Axis 3 for car codes.

NFC Handler: Reads page 9 from the PN532, verifies 0xCAB1 authentication bytes, and extracts the car code.

Input Manager: Converts joystick readings into 16-bit signed values and forwards them via BLE.

Car Code Handler: Places the extracted 2-byte car code onto Generic Axis 3 for the HID report.

Main Loop: Continuously polls NFC + joystick and updates the HID state.
