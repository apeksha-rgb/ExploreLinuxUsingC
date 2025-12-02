## **2. Program: Memory Management (Memory Leak Demonstration)**  
### **File:** `memory_management/memory_leak.c`

---

### **Problem Statement**
This program demonstrates dynamic memory allocation and deallocation in Linux.  
It helps monitor RAM usage and understand how **memory leaks** occur when allocated memory is not freed properly.

---

### **Implementation Brief**

- Allocates multiple **1MB chunks** (~1GB total) dynamically.
- Tracks all allocations using a **linked list**.
- Touches allocated memory using `memset()` so Linux commits real RAM.
- Waits for **user input** before freeing all allocated memory.
- Observes memory usage in real time using Linux monitoring tools.

---

### **Industry-Level Learning**

1. **Dynamic memory management**  
   Demonstrates disciplined use of `malloc()` and `free()`.

2. **Linux memory behavior**  
   Explains virtual memory, committed RAM, and physical usage.

3. **Monitoring memory**  
   Uses `top`, `htop`, `/proc` filesystem, and **Valgrind** for leak detection.

4. **Linked-list usage**  
   Stores and safely frees large allocations.

5. **Robust programming**  
   Avoids memory leaks, prevents dangling pointers, and handles large allocations safely.

6. **Production mindset**  
   Encourages system-aware, memory-efficient, and scalable design.

---

## **Linux RAM Monitoring**

While the program runs, Linux offers multiple ways to track memory usage:

---

### **1. Using `top`**

```bash
top
````

* Look for the **memory_leak** process
* **RES** → Actual RAM used
* **VIRT** → Virtual memory
* Press **q** to exit

---

### **2. Using `htop` (Recommended)**

```bash
htop
```

* Find **memory_leak** process
* Watch RAM usage grow during allocation
* Exit using **F10**

---

### **3. Using `/proc` filesystem (Precise Monitoring)**

```bash
watch -n1 cat /proc/$(pidof memory_leak)/status | grep -E "VmRSS|VmSize"
```

* **VmRSS:** Actual RAM used
* **VmSize:** Total virtual memory reserved
* Auto-refreshes every second

---

### **4. Using Valgrind to Detect Leaks**

```bash
valgrind --leak-check=full ./memory_leak
```

* Detects unfreed memory
* Industry best practice to validate memory correctness

---

## **Observation Steps**

* Run program:

  ```bash
  ./memory_leak
  ```
* Open **htop** in another terminal to observe memory growth.
* Press **ENTER** inside the program to free all memory.
* Watch RAM usage drop in **top/htop**.


