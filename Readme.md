# 🌱 IoT Garden Soil Temperature & Humidity Monitoring System

A wireless IoT system for real-time monitoring of soil temperature and humidity in gardens or farms, built with Arduino Uno boards, RF communication, and GSM connectivity.

> **Academic Project** — B.Sc. Computer Engineering, Yazd University (2021)  
> **Authors:** Sara Abbasi

---

## 📖 Overview

This project implements an IoT-based distributed sensor network to periodically collect soil temperature and humidity data from a garden. The data travels wirelessly from field sensor nodes to a central sink node, which then uploads it to a remote web server via GSM — all powered by solar-charged batteries, requiring no wired infrastructure.

The system helps prevent water waste, detect soil problems early, and enables data-mining algorithms to be applied on the collected readings for smarter agriculture decisions.

---

## 🏗️ System Architecture

```
[Sensor Node(s)]
  DHT11 + YL-69
  Arduino Uno
  ASK Transmitter
  Solar Panel + Battery
       │
       │  433 MHz RF (ASK)
       ▼
[Sink Node]
  ASK Receiver
  Arduino Uno
  SIM800L (GSM)
  Solar Panel + Battery
       │
       │  HTTP POST via GPRS/GSM
       ▼
[Web Server]
  Database
  Online Dashboard
```

- **Sensor nodes** collect soil data and broadcast it wirelessly via the ASK 433 MHz RF module.
- The **sink node** (receiver) aggregates data from all sensor nodes, then forwards it to the server using a SIM800L GSM module over GPRS.
- A **web server** stores the data in a database and displays it on a live dashboard accessible via public IP.
- All nodes are **solar-powered**, with rechargeable lithium batteries that are charged during the day, enabling fully autonomous operation.

---

## 🔩 Hardware Components

### Sensors

| Component | Purpose | Details |
|-----------|---------|---------|
| **DHT11** | Air temperature & humidity | Range: 0–50°C (±2°C), 20–80% RH (±5%) |
| **YL-69** | Soil moisture | Analog output, range 0–1023; <370 = wet, 370–600 = moist, ≥600 = dry |

### Communication Modules

| Component | Purpose | Details |
|-----------|---------|---------|
| **ASK 433 MHz** (TX) | Wireless data transmission | Amplitude Shift Keying, 433 MHz SAW resonator |
| **ASK 433 MHz** (RX) | Wireless data reception | RF tuner + amplifier + PLL decoder |
| **SIM800L** | GSM/GPRS connectivity | Quad-band 850/900/1800/1900 MHz, HTTP POST to server |

### Power System

| Component | Purpose | Details |
|-----------|---------|---------|
| **Sony ICR 18650** | Battery (×2 per node) | 4.2V, 3000 mAh Li-ion, rechargeable |
| **HN-010M Solar Panel** | Charging | 12V, 10W, monocrystalline cells |
| **LM2596 Buck Converter** | Voltage step-down | Converts 7.2V battery output to 4.2V for SIM800L |

### Microcontroller

- **Arduino Uno** — used in both sensor nodes and the sink node

---

## 📡 How It Works

### Step 1 — Sensor Node (Transmitter)
1. The **DHT11** reads ambient temperature and humidity (connected to digital pin 8).
2. The **YL-69** reads soil moisture via the analog pin A0.
3. The Arduino formats the readings into a string and sends it over the hardware TX pin to the **ASK transmitter module**.
4. The ASK module broadcasts the data at 433 MHz.

### Step 2 — Sink Node (Receiver)
1. The **ASK receiver module** picks up the 433 MHz signal and passes the decoded data to the Arduino via the hardware RX pin.
2. The sink node Arduino forwards the data to the **SIM800L** module over SoftwareSerial (pins 2 & 3).
3. The SIM800L establishes a GPRS connection and sends the data to the web server via an **HTTP POST** request.

### Step 3 — Server
1. The web server receives the POST request and stores the data in a **database**.
2. A web dashboard fetches the records and displays them online in real time.
3. The server is accessible externally via a **public IP** (placed in the DMZ of the local network).

---

## ⚡ Wiring Summary

### DHT11 → Arduino Uno
| DHT11 Pin | Arduino Pin |
|-----------|-------------|
| VCC | 5V |
| Data | Digital 8 |
| NC | — |
| GND | GND |

### YL-69 → Arduino Uno
| YL-69 Pin | Arduino Pin |
|-----------|-------------|
| VCC | 5V |
| Analog Out | A0 |
| Digital Out | (not used) |
| GND | GND |

### ASK Transmitter → Arduino Uno (Sensor Node)
| ASK TX Pin | Arduino Pin |
|------------|-------------|
| VCC | 5V |
| Data | TX (hardware) |
| GND | GND |

### ASK Receiver → Arduino Uno (Sink Node)
| ASK RX Pin | Arduino Pin |
|------------|-------------|
| VCC | 5V |
| Data | RX (hardware) |
| GND | GND |

### SIM800L → Arduino Uno (Sink Node)
| SIM800L Pin | Arduino Pin / Other |
|-------------|---------------------|
| VCC | LM2596 OUT+ (4.2V) |
| GND | LM2596 OUT− / Arduino GND |
| TX | Arduino Digital 2 |
| RX | Arduino Digital 3 (via 100Ω resistor) |
| Reset | Arduino Reset |
| NET | Helical antenna |
| U.FL | PCB antenna (optional) |

> **Note:** A 17.3 cm wire antenna (1/4 wavelength of 433 MHz) is recommended for both ASK modules to improve range.

---

## 💻 Code Structure

```
/
├── sender_node/          # Arduino sketch for sensor nodes (DHT11 + YL-69 + ASK TX)
└── sink_node/            # Arduino sketch for the sink node (ASK RX + SIM800L)
```

### Sender Node (sensor.ino)
- Reads DHT11 temperature & humidity
- Reads YL-69 analog soil moisture value
- Formats data as a plain string and transmits via Serial (ASK TX)

### Sink Node (receiver.ino)
- Receives serial data from ASK RX module
- Establishes GPRS connection using AT commands (APN: `mcinet`)
- Sends data to the server via HTTP POST

---

## 🔧 Setup & Requirements

### Arduino Libraries
- [`DHT sensor library`](https://github.com/adafruit/DHT-sensor-library) by Adafruit
- `SoftwareSerial` (built-in with Arduino IDE)

### Hardware Setup
1. Wire sensor nodes per the wiring table above.
2. Set the LM2596 buck converter output to **4.2V** before connecting the SIM800L.
3. Insert a SIM card with an active data plan into the SIM800L.
4. Connect the solar panel to the battery for autonomous charging.
5. Flash `sender_node` to sensor Arduinos and `sink_node` to the sink Arduino.

### Server Setup
- Host a web server with a database (e.g., MySQL + PHP).
- Create an endpoint to accept HTTP POST requests and store records.
- Assign a static public IP (or use DMZ forwarding) for external access.
- Update the IP in the sink node sketch: `AT+HTTPPARA="URL","YOUR_SERVER_IP:PORT"`

---

## 📊 Soil Moisture Reference (YL-69)

| Sensor Value | Soil Condition |
|-------------|----------------|
| < 370 | Fully Wet |
| 370 – 600 | Moist |
| ≥ 600 | Dry |

---

## 📚 References

1. [How to connect DHT11 with Arduino Uno](https://create.arduino.cc/projecthub/pibots555/how-to-connect-dht11-sensor-with-arduino-uno-f4d239)
2. [Using the YL-39/YL-69 Soil Humidity Sensor with Arduino](https://create.arduino.cc/projecthub/nekhbet/using-the-yl-39-yl-69-soil-humidity-sensor-with-arduino-968268)
3. [How to Power Your Arduino Using a Solar Panel](https://create.arduino.cc/projecthub/iasonas-christoulakis/how-to-power-your-arduino-using-solar-panel-4d29d8)
4. [SIM900/800 HTTP POST Request in JSON Format with Arduino](https://how2electronics.com/sim900-800-http-post-request-json-format-arduino/)

---

## 📄 License

This project was developed as a B.Sc. capstone project at Yazd University in summer 2021. Feel free to use it for educational purposes.
