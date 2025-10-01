# Integration Guide: Adding Car Support in Software

This guide explains how to integrate toy cars with a game or application using the **Switchway firmware**.

---

## 1. Write the Car Code to the NFC Tag
- Program the tag’s **page 9** with the following 4 bytes:
  - `0xCA 0xB1` → Authentication marker (ensures authenticity)
  - `0xXX 0xYY` → Car code (16-bit identifier, up to ~65,000 cars)
- **Example:** `0xCA 0xB1 0xAB 0xCD`

>  Car codes must be **raw bytes**, not NDEF formatted.

---

## 2. Pair the Controller
- Connect the Switchway controller to the host system via **Bluetooth HID**.
- No custom driver is needed — the device is recognized as a standard game controller.

---

## 3. HID Input Mapping
Switchway exposes its inputs as follows:

| Input        | HID Mapping  | Notes                                      |
|--------------|-------------|--------------------------------------------|
| Joystick X   | Axis 1      | 16-bit signed integer                      |
| Joystick Y   | Axis 2      | 16-bit signed integer                      |
| Car Code     | Axis 3      | Updates when a tag is detected; `0x0000` when no tag present |
| Button 1–3   | Generic Btn | Three physical buttons on the joystick     |

---

## 4. Implement Car Swapping in Software
- Continuously **poll Axis 3 (Car Code)**.
- When the value changes from `0x0000` → non-zero, a new car has been placed on the portal.
- Map the received car code to in-game actions, such as:
  - Spawning a vehicle
  - Unlocking a car skin
  - Switching loadouts mid-race

---

## 5. Handle Edge Cases
- If the car is removed, **Axis 3 resets to `0x0000`**.
- Multiple cars may share the same code — uniqueness is determined by the game’s mapping logic.
- Future firmware may extend the car code structure with **versioning bytes** for backward-compatible updates.


---
