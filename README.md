# 🚦 Automatic Gate System with Interrupts (PIC16F877A)

This project implements an **automatic gate (barrier) system** using the **PIC16F877A microcontroller** and **C language (XC8 compiler)**.  
The barrier simulates a typical level crossing or parking gate, reacting to car sensors, open/close sensors, and an interrupt for train detection.

---

## 📖 About the Project
Developed at **UFSC - Universidade Federal de Santa Catarina**, this project demonstrates:

- Use of **GPIO inputs/outputs** for sensors and actuators  
- Implementation of **interrupts** to handle external events (train arrival)  
- Real-time control of LEDs, motors, and buzzer  
- Simulation of a **safe automated gate system**  

The system keeps the barrier open for vehicles unless a train passes, in which case the gate closes automatically for safety.

---

## ✨ Features
- **Car Detection**  
  - Waits for a car sensor (RB1).  
  - Starts the opening sequence if a car arrives.  

- **Gate Opening**  
  - Activates motor and LED indicators for opening.  
  - Stops when the “gate open” sensor (RB3) is triggered.  
  - Keeps the gate open for 20 seconds before closing.  

- **Gate Closing**  
  - Activates motor and LED indicators for closing.  
  - Stops when the “gate closed” sensor (RB2) is triggered.  

- **Interrupt Handling (Train Passing)**  
  - Triggered by RB0 external interrupt.  
  - Immediately closes the gate if it is opening/open.  
  - Activates a buzzer (RD4) during train passage.  
  - Waits until the train sensor clears, then resumes normal operation.  

---

## ⚙️ Hardware Requirements
- **Microcontroller**: PIC16F877A  
- **Clock**: 4 MHz XT oscillator  
- **Inputs (PORTB)**:  
  - RB0 → Train sensor (interrupt source)  
  - RB1 → Car sensor  
  - RB2 → Gate closed sensor  
  - RB3 → Gate open sensor  
- **Outputs (PORTD)**:  
  - RD0 → LED: Opening indicator  
  - RD1 → Motor: Opening  
  - RD2 → LED: Closing indicator  
  - RD3 → Motor: Closing  
  - RD4 → Buzzer (train alert)  

---

## 🛠️ How to Build and Run
1. Save the source code (e.g., `cancela.c`).  
2. Compile using **MPLAB X IDE + XC8 Compiler**:  
   ```bash
   xc8 --chip=16F877A cancela.c
