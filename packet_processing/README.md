## **1. Program: Bitfields**  
### **File:** `packet_processing/mpegts_header_ops.c`

---

### **Problem Statement**
The 4-byte MPEG-TS (MPEG2 Transport Stream) header is composed of multiple fields with fixed bit lengths. Extracting and inserting these fields at high speed can be challenging.  
This program demonstrates how to represent and manipulate binary packet headers efficiently in memory using **C bitfields**.

---

### **MPEG-TS Header Bitfield Layout**

| Bits | Field                           |
|------|----------------------------------|
| 8    | sync_byte                        |
| 1    | transport_error_indicator        |
| 1    | payload_unit_start_indicator     |
| 1    | transport_priority               |
| 13   | PID                              |
| 2    | scrambling_control               |
| 1    | adaptation_field_control         |
| 1    | payload_flag                     |
| 4    | continuity_counter               |

---

### **Implementation Brief**
- Defined a **4-byte MPEG-TS header** using `struct` with **bitfields**.
- Assigned **PID**, flags, and continuity counter.
- Displayed the header in **hex** and **binary** formats.
- Modified fields to observe how changes reflect in memory.
  
---

### **Industry-Level Learning**

1. **Memory-efficient data structures**  
   Bitfields reduce memory usage and closely mirror real protocol headers.

2. **Packed structures**  
   Prevents unwanted padding—critical in embedded systems and network packet parsing.

3. **Debugging techniques**  
   Hex and binary output mimics behavior of packet inspection tools (Wireshark, hexdump).

4. **Type safety**  
   Using `uint8_t` and fixed-width integers ensures consistent cross-platform behavior.

5. **Modular design**  
   Helper functions like `print_header()` improve readability, reusability, and maintainability.

---
