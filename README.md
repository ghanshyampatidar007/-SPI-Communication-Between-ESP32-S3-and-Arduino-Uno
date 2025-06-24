# 🔁 SPI Communication Between ESP32-S3 and Arduino Uno

This project demonstrates **full-duplex SPI communication** between an **ESP32-S3 (running Zephyr RTOS)** and an **Arduino Uno**. The goal is to send and receive **one character** from each side during every SPI transaction.

---

## 🚀 Project Overview

- **ESP32-S3 acts as SPI Master** using SPI2.
- **Arduino Uno acts as SPI Slave** using standard SPI.
- The master (ESP32-S3) sends a character (e.g., `'A'`) to the slave.
- The slave (Arduino Uno) receives that character and simultaneously sends a reply character (e.g., `'B'`) back.
- Both devices print their sent and received values to their serial monitors.

This setup demonstrates how **full-duplex SPI** can be used to exchange one byte of data in each transaction.

---

## 📦 Components Used

- ESP32-S3 DevKitC microcontroller
- Arduino Uno
- Jumper wires
- Common GND connection
- Optional: Logic level shifter (for voltage safety on MISO line)

---

## 📡 SPI Pin Connections

| Signal | ESP32-S3 Pin | Arduino Uno Pin | Direction           | Description                     |
|--------|--------------|------------------|---------------------|---------------------------------|
| MOSI   | GPIO11       | D11              | ESP32 → Arduino     | Master Out, Slave In            |
| MISO   | GPIO13       | D12              | Arduino → ESP32     | Master In, Slave Out (3.3V safe)|
| SCLK   | GPIO12       | D13              | ESP32 → Arduino     | Clock from Master               |
| CS     | GPIO10       | D10              | ESP32 → Arduino     | Manual Chip Select              |
| GND    | GND          | GND              | Shared Ground        | Must be connected               |

---

## 🧠 Key Features

- **Full-Duplex Communication:** Each SPI transfer sends and receives one byte.
- **Manual CS Handling:** The ESP32-S3 manually controls chip select using a GPIO.
- **Logging:** Both boards log their actions via serial port.
- **Works Without Zephyr Overlay File:** All SPI configurations are done directly in the application.

---

## 📈 How It Works

1. ESP32-S3 sends a byte (e.g., `'A'`) to the Arduino.
2. Arduino receives this byte and replies with another byte (e.g., `'B'`) using SPI interrupt.
3. ESP32-S3 simultaneously receives the reply during the transfer.
4. Each device prints the transmitted and received characters on its serial monitor.
5. The transaction repeats every 2 seconds.

---

## 📟 Example Output

### ESP32-S3 Serial Monitor:
