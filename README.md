<div align="center">

<br/>

```
░██████╗███████╗██████╗░██╗   ██╗░█████╗░  ░█████╗░██╗░░░░░░█████╗░░██╗░░░░░░░██╗
██╔════╝██╔════╝██╔══██╗██║   ██║██╔══██╗  ██╔══██╗██║░░░░░██╔══██╗░██║░░██╗░░██║
╚█████╗░█████╗░░██████╔╝╚██╗░██╔╝██║░░██║  ██║░░╚═╝██║░░░░░███████║░╚██╗████╗██╔╝
░╚═══██╗██╔══╝░░██╔══██╗░╚████╔╝░██║░░██║  ██║░░██╗██║░░░░░██╔══██║░░████╔═████║░
██████╔╝███████╗██║░░██║░░╚██╔╝░░╚█████╔╝  ╚█████╔╝███████╗██║░░██║░░╚██╔╝░╚██╔╝░
╚═════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚════╝░  ░╚════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░
```

# SERVO · THERMAL CONTROLLER

**Dual-axis servo control with real-time NTC thermistor monitoring**  
_Arduino-based · Joystick driven · Web interface coming soon_

<br/>

![Arduino](https://img.shields.io/badge/Platform-Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Status](https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge)
![Web UI](https://img.shields.io/badge/Web_UI-Coming_Soon-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)

<br/>

---

</div>

<br/>

## ⚡ Overview

A precision servo control system built on Arduino, capable of driving **two independent servos** from a single joystick axis with complementary motion logic — while simultaneously monitoring ambient temperature via a **10KΩ NTC thermistor** using the Steinhart-Hart B-parameter equation.

A future revision will expose all sensor data (servo positions, live temperature) through a **browser-based web interface** for remote monitoring and control.

<br/>

---

## 🔩 Hardware

| Component         | Value / Model                 | Pin  |
| ----------------- | ----------------------------- | ---- |
| Servo — Drive     | Standard PWM Servo            | `D2` |
| Servo — Steer     | Standard PWM Servo            | `D4` |
| Joystick (X-axis) | Analog joystick               | `A5` |
| Mode Switch       | Momentary button (active LOW) | `D3` |
| NTC Thermistor    | 10KΩ @ 25°C, B=3977K          | `A2` |
| Fixed Resistor    | 10KΩ (voltage divider)        | —    |

<br/>

---

## 🌡️ Thermistor Wiring

The thermistor uses a **voltage divider** configuration for optimal sensitivity in the room-temperature range:

```
        5V
         │
    ┌────┴────┐
    │ NTC 10K │   ← Thermistor (not polarized)
    └────┬────┘
         │
         ├──────── A2  (analogRead)
         │
    ┌────┴────┐
    │  R 10K  │   ← Fixed resistor
    └────┬────┘
         │
        GND
```

> 💡 **Tip:** Add a **100nF ceramic capacitor** between `A2` and `GND` to filter ADC noise and reduce temperature jitter by 2–3°C.

At 25°C, both resistors are equal → ADC reads ~**512** (perfect midpoint sensitivity).

<br/>

---

## 🎮 Control Logic

### Normal Mode

The joystick X-axis (`A5`) maps `0–1023` → `0°–180°` for the drive servo.  
The steer servo runs **complementary** (mirrored): `abs(val - 180)`

```
Joystick 0%   →  Drive: 0°    Steer: 180°
Joystick 50%  →  Drive: 90°   Steer: 90°
Joystick 100% →  Drive: 180°  Steer: 0°
```

### Button Mode (`D3` held LOW)

Both servos lock to **90°** (neutral / center position).

<br/>

---

## 🧮 Temperature Formula

Uses the **Steinhart-Hart simplified B-parameter equation**:

```
         1
T = ─────────────────────
    ln(RT/RT₀)     1
    ──────────  +  ──
        B          T₀
```

Where:

- `RT` = measured thermistor resistance
- `RT₀` = 10,000 Ω (resistance at 25°C)
- `B` = 3977 K (thermistor B-constant)
- `T₀` = 298.15 K (25°C reference)

**Voltage divider resistance calculation (thermistor on top):**

```cpp
VRT = (5.0 / 1023.0) * analogRead(A2);   // ADC → voltage
VR  = VCC - VRT;                           // voltage across fixed R
RT  = (VR / VRT) * R;                     // thermistor resistance
```

<br/>

---

## 💻 Serial Output

```
Raw ADC: 514
Temperature:    24.73C
```

Expected ADC range at room temperature: **490 – 530**

<br/>

---

## 📁 Project Structure

```
servo-thermal-controller/
├── src/
│   └── main.ino          # Main Arduino sketch
├── docs/
│   └── wiring.png        # Wiring diagram
├── web/                  # 🚧 Web interface (upcoming)
│   ├── index.html
│   ├── dashboard.js
│   └── styles.css
└── README.md
```

<br/>

---

## 🌐 Upcoming: Web Interface

> **Version 2.0** will introduce a live browser dashboard for remote monitoring.

Planned features:

- 📊 **Live temperature graph** — real-time thermistor readings plotted over time
- 🎯 **Servo position indicators** — visual dial display for both servos
- 🔘 **Mode status** — shows Normal vs Button-lock mode
- 📡 **Serial-over-WebSocket** — Arduino streams data to a lightweight Node.js bridge
- 📱 **Mobile responsive** — monitor from any device on your local network

```
Arduino (Serial) ──► Node.js bridge ──► WebSocket ──► Browser Dashboard
```

<br/>

---

## 🚀 Getting Started

### Prerequisites

- Arduino IDE 1.8+ or Arduino IDE 2.x
- `Servo.h` — included with Arduino IDE

### Upload

```bash
# Clone the repo
git clone https://github.com/yourusername/servo-thermal-controller.git

# Open in Arduino IDE
# Select your board (Uno / Nano / Mega)
# Select your COM port
# Upload
```

### Monitor

Open Serial Monitor at **9600 baud** to view live temperature readings.

<br/>

---

## 🐛 Troubleshooting

| Symptom                       | Cause                                 | Fix                                          |
| ----------------------------- | ------------------------------------- | -------------------------------------------- |
| Temperature shows `-273.15°C` | ADC reads 0 — thermistor disconnected | Check wiring at `A2`                         |
| ADC reads `0` or `1023`       | Open circuit in voltage divider       | Verify both thermistor and 10KΩ resistor     |
| Servo not moving              | Wrong pin or not attached             | Check `servodr.attached()` in Serial Monitor |
| Temperature jittery           | ADC noise                             | Add 100nF cap between `A2` and `GND`         |

<br/>

---

## 📜 License

MIT License — see [`LICENSE`](LICENSE) for details.

<br/>

<div align="center">

---

_Built with solder and spite · Web interface incoming_

⭐ Star this repo if it helped you!

</div>
