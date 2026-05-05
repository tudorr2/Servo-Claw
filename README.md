<div align="center">

<br/>

```
████████╗██╗  ██╗███████╗██████╗░███╗░░░███╗░█████╗░░█████╗░██╗░░░░░░█████╗░░██╗░░░░░░░██╗
╚══██╔══╝██║  ██║██╔════╝██╔══██╗████╗░████║██╔══██╗██╔══██╗██║░░░░░██╔══██╗░██║░░██╗░░██║
░░░██║░░░███████║█████╗░░██████╔╝██╔████╔██║██║░░██║██║░░╚═╝██║░░░░░███████║░╚██╗████╗██╔╝
░░░██║░░░██╔══██║██╔══╝░░██╔══██╗██║╚██╔╝██║██║░░██║██║░░██╗██║░░░░░██╔══██║░░████╔═████║░
░░░██║░░░██║  ██║███████╗██║░░██║██║░╚═╝░██║╚█████╔╝╚█████╔╝███████╗██║░░██║░░╚██╔╝░╚██╔╝░
░░░╚═╝░░░╚═╝  ╚═╝╚══════╝╚═╝░░╚═╝╚═╝░░░░╚═╝░╚════╝░░╚════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░
```

# THERMO · CLAW · CONTROLLER

**Handheld 3D printed claw controller with dual servo control and real-time NTC thermistor monitoring**  

<br/>

![Raspberry Pi Pico](https://img.shields.io/badge/Platform-Raspberry%20Pi%20Pico2W-A22846?style=for-the-badge&logo=raspberrypi&logoColor=white)
![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![3D Printed](https://img.shields.io/badge/Build-3D_Printed-lightgrey?style=for-the-badge&logo=printables&logoColor=white)
![Status](https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge)
![Web UI](https://img.shields.io/badge/Web_UI-Coming_Soon-orange?style=for-the-badge)

<br/>

---

</div>

<br/>

## ⚡ Overview

ThermoClaw is a **handheld, 3D printed** servo claw controller. The enclosure and claw mechanism are fully 3D printed, housing dual servos driven from a single joystick axis with complementary motion logic — while simultaneously monitoring ambient temperature via a **10KΩ NTC thermistor** using the Steinhart-Hart B-parameter equation.

A future revision will expose all sensor data (servo positions, live temperature) through a **browser-based web interface** for remote monitoring and control.

<br/>

---

## 🖨️ 3D Printed Build

The entire chassis is **3D printed** — designed to be compact, handheld, and self-contained.

| Part                  | Material    | Notes                                   |
| --------------------- | ----------- | --------------------------------------- |
| Main body / enclosure | PLA or PETG | Rigid shell housing Arduino + servos    |
| Claw fingers          | PLA or PETG | Current version                         |
| **Claw fingers (v2)** | **TPU 95A** | **Upcoming — improves grip on objects** |

<br/>

---

## 🔩 Hardware

| Component         | Value / Model                 | Pin  |
| ----------------- | ----------------------------- | ---- |
| Servo             | Standard PWM Servo            | `D2` |
| Servo             | Standard PWM Servo            | `D4` |
| Joystick (X-axis) | Analog joystick               | `A0` |
| Mode Switch       | Momentary button (active LOW) | `D3` |
| NTC Thermistor    | 10KΩ @ 25°C, B=3977K          | `A1` |
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

---
<br/>
---
</div>
