# UGV - Unmanned Ground Vehicle

A 4-Wheel Unmanned Ground Vehicle designed for remote monitoring, surveillance, obstacle detection, and environmental sensing.

## Overview

This project is a working prototype of a 4-wheel Unmanned Ground Vehicle (UGV) built using ESP8266 NodeMCU and ESP32-CAM.

The vehicle can be remotely controlled while providing live video and monitoring environmental conditions such as gas concentration, temperature, and humidity.

The system is designed as a modular platform that can be further upgraded with advanced communication, mapping, automation, and AI capabilities.

## Features

- 4-Wheel Drive
- Remote Motor Control
- Live Video Streaming
- Pan-Tilt Camera
- Gas Detection using MQ-2
- Temperature & Humidity Monitoring using DHT11
- Obstacle Detection using HC-SR04
- Buzzer Alert System
- Blynk IoT Monitoring & Control
- Rechargeable Battery System
- Modular Hardware Architecture

## Hardware

| Component | Function |
|---|---|
| ESP8266 NodeMCU | Main rover controller |
| ESP32-CAM | Live video streaming |
| L298N | Motor driver |
| DC Gear Motors | Vehicle movement |
| Servo Motors | Camera pan & tilt |
| MQ-2 | Gas detection |
| DHT11 | Temperature & humidity monitoring |
| HC-SR04 | Obstacle detection |
| Buzzer | Local alert |
| Li-ion Battery Pack | Power supply |
| Buck Converter | Voltage regulation |

## System Architecture

```text
                         REMOTE USER
                              │
                              ↓
                       BLYNK IoT CONTROL
                              │
                              ↓
                       ESP8266 NODEMCU
                     ┌────────┼────────┐
                     ↓        ↓        ↓
                 MOTOR      HC-SR04   PAN/TILT
                 CONTROL   OBSTACLE    CONTROL
                                         │
                                         ↓
                                     ESP32-CAM
                                         │
                                         ↓
                                    LIVE VIDEO


              MQ-2 ─────────→ GAS MONITORING
              
              DHT11 ────────→ TEMPERATURE &
                               HUMIDITY DATA
              
              BUZZER ───────→ LOCAL ALERT
Working Principle
The UGV is remotely controlled through Blynk IoT using the ESP8266 NodeMCU.
The NodeMCU controls the drive motors, ultrasonic obstacle detection, and camera pan-tilt mechanism.
The ESP32-CAM provides live video streaming for remote visual monitoring.
The MQ-2 sensor is used for gas detection, while the DHT11 monitors temperature and humidity.
The HC-SR04 detects nearby obstacles, and the buzzer provides a local alert when required.
Basic Workflow
SENSE
  ↓
PROCESS
  ↓
TRANSMIT
  ↓
MONITOR
  ↓
RESPOND
Software
Arduino IDE
C/C++
ESP8266
ESP32-CAM
Blynk IoT
Wi-Fi
ESP32-CAM Web Server
Repository Structure
UGV-Unmanned-Ground-Vehicle/
│
├── README.md
│
├── hardware/
│   ├── circuit-diagram.png
│   └── components-list.md
│
├── software/
│   ├── nodemcu/
│   │   └── rover_control.ino
│   │
│   └── esp32-cam/
│       └── camera_stream.ino
│
├── images/
│   ├── rover-front.jpg
│   ├── rover-side.jpg
│   └── prototype.jpg
│
└── docs/
    ├── architecture.png
    └── project-documentation.pdf
Future Improvements
The current prototype is designed with a modular architecture that allows future upgrades.
Planned improvements include:
Long-range communication using LoRa
LiDAR-based mapping and localization
Semi-autonomous navigation
AI-based video and hazard analysis
Additional environmental sensors
Improved obstacle detection
Multi-UGV coordination
Centralized fleet monitoring
Applications
The UGV platform can be adapted for:
Remote inspection
Industrial monitoring
Hazardous-area observation
Environmental sensing
Search and inspection applications
Educational robotics
Research and development
Project Status 
```
## Working Prototype

The current version demonstrates remote movement, live video streaming, environmental sensing, obstacle detection, and local alert functionality.
This project is currently intended for educational, experimental, and research purposes and is not certified for operation in hazardous or explosive environments.
Development
This project is being continuously improved through hardware integration, software development, testing, and experimentation.
Development Flow
IDEA
  ↓
PROTOTYPE
  ↓
TESTING
  ↓
IMPROVEMENT
  ↓
ADVANCED UGV
Author
Abhishek Jadhav
GitHub: @Abhishekjadhav13⁠�
License
This project is intended for educational and experimental use.
