# ✈️ AeroLogic Flight Management System

[![Language: C++](https://img.shields.io/badge/Language-C++-00599C?style=for-the-badge&logo=c%2B%2B)](https://cplusplus.com/)
[![Course: Semester 6](https://img.shields.io/badge/Course-Semester_6-blueviolet?style=for-the-badge)]()

## 🛫 Project Overview
The **AeroLogic Flight Management System** is a sophisticated console application that dives deep into advanced Data Structures and Algorithms. The system eschews standard array arrays or vector libraries in favor of custom, dynamically allocated **Linked Lists** to manage an airline's volatile roster of flights, passenger bookings, and ticket cancellations dynamically and memory-efficiently.

## 💺 Key Features
*   **Dynamic Data Structures:** Built entirely upon custom Singly and Doubly Linked Lists for O(1) dynamic insertions.
*   **Flight Rostering:** Add, remove, and sort commercial flights dynamically based on various parameters.
*   **Passenger Manifests:** Robust tracking for passenger reservations, allowing waitlisting when flights reach max capacity.
*   **Memory Safe:** Strict pointer management and garbage collection (destructors) preventing memory leaks.

## 🖥️ Tech Stack
*   **Programming Language:** C++
*   **Core Concepts:** Linked Lists, Pointers, Dynamic Memory Allocation (Heap), OOP.

## 📷 Screenshots

![Main Menu Operations](assets/screenshot_1.png)
*Figure 1: Main interactive node manipulation menu.*

![Flight Operations](assets/screenshot_2.png)
*Figure 2: Viewing the real-time manifest built dynamically in memory.*

![Passenger Tracking](assets/screenshot_3.png)
*Figure 3: Details on passenger booking queues.*

![System Diagnostics](assets/screenshot_4.png)
*Figure 4: Core flight metrics calculated through linked list traversal.*

## 📂 Project Structure
```text
Flight_Management_System_TDS/
├── src/            # Source code containing custom Linked List object implementations
├── bin/            # Executable files
├── docs/           # Documentation and analysis
└── assets/         # Console screenshots
```

## ⚙️ Installation & Setup
1. Clone the repository.
2. Install a C++ compiler environment (GCC/MinGW).
3. Navigate to the `src` directory containing the list configurations.
4. Compile the source logic: `g++ main.cpp LinkedList.cpp -o aerologic_sys`

## 🚁 How to Run
1. Run the compiled executable via terminal: `./aerologic_sys`
2. Utilize the terminal menu interface to simulate flight management.
