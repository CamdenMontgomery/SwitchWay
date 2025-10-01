# Switchwayz NFC Specification

Switchwayz integrates NFC tags to enable "toy-to-life" style gameplay.  
Each toy car has a unique **car code** stored directly on its NFC tag.

---

## NFC Car Code Structure

- **Tag Type:** NTAG213 (13.56 MHz)  
- **Data Location:** Page 9 of the tag’s memory  
- **Format:** Raw bytes (not NDEF formatted)  

Structure:  
- **Authentication marker:** `0xCA B1` (2 bytes)  
- **Car code:** 2 bytes immediately following marker  

**Example (Page 9 Data):**  
`[..][0xCA][0xB1][0xAB][0xCD]`  

- Auth Marker = `0xCAB1`  
- Car Code = `0xABCD`  

---

## HID Mapping

- Car code is placed into **Generic Axis 3** of the HID report.  
- Axis is 16-bit, allowing for ~60,000 unique cars.

---

## Security Notes

- NTAG213 supports **password protection** and write-locking.  
- To prevent tampering, tags can be locked after writing the authentication marker + car code.  
- Optional: implement NFC authentication at firmware level to reject unauthorized tags.

---

## Advantages of This Design

- Flexible: developers can assign any hex code to represent cars, power-ups, or game logic.  
- Scalable: large address space (~60k cars).  
- Portable: uses raw bytes, no dependency on NDEF or external standards.