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
