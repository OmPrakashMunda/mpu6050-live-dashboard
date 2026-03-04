# MPU6050 Live Dashboard

A real-time web dashboard for visualizing MPU6050 IMU data streamed from an ESP8266 over WebSocket.
Features a live 3D orientation model, motion intensity gauge, and raw sensor readings — all in a single HTML file with zero dependencies to install.

![Dashboard Preview](preview.png)

---

## Features

- **3D Orientation** — Live Three.js board model rotates with the physical sensor
- **Motion Gauge** — Canvas-drawn speedometer showing net acceleration in m/s²
- **Raw Data Panel** — Accelerometer (g), Gyroscope (°/s), and computed Roll/Pitch/Yaw
- **IP Input** — Connect to any ESP8266 directly from the browser, no code changes
- **Theme Switch** — Dark / Light mode
- **No backend needed** — Browser connects directly to ESP8266 WebSocket
- **Built-in Documentation** — Full math and wiring reference scrollable below the dashboard

---

## Hardware Required

| Component | Details |
|-----------|---------|
| ESP8266 | NodeMCU v1/v2 or any variant |
| MPU6050 | GY-521 module |
| Jumper wires | 4x |

### Wiring

```
MPU6050 Pin  →  ESP8266 (NodeMCU)
──────────────────────────────────
VCC          →  3.3V
GND          →  GND
SDA          →  D2  (GPIO 4)
SCL          →  D1  (GPIO 5)
AD0          →  GND (I²C address 0x68)
```

---

## Arduino Setup

### Libraries Required

Install via **Arduino IDE → Sketch → Manage Libraries**:

| Library | Author |
|---------|--------|
| `MPU6050` | Electronic Cats |
| `WebSockets` | Markus Sattler |
| `ArduinoJson` | Benoit Blanchon |

> ⚠️ Install **WebSockets** by Markus Sattler — not "ArduinoWebsockets" by Gil Maimon. They are different libraries.

### Flash the ESP8266

1. Open `firmware/firmware.ino` in Arduino IDE
2. Set your WiFi credentials:
   ```cpp
   const char* ssid     = "YOUR_SSID";
   const char* password = "YOUR_PASSWORD";
   ```
3. Select board: **Tools → Board → NodeMCU 1.0 (ESP-12E Module)**
4. Upload and open Serial Monitor at **115200 baud**
5. Note the IP address printed on boot

---

## Web Dashboard

1. Open `dashboard/index.html` in any modern browser
2. Enter the ESP8266 IP address in the input field
3. Click **Connect**
4. Status dot turns green — data streams live

> Both the ESP8266 and your browser must be on the **same WiFi network.**

---

## How It Works

### Complementary Filter (Orientation)

Fuses gyroscope and accelerometer to estimate roll and pitch:

```
roll  = 0.96 × (roll  + gx × dt × π/180) + 0.04 × atan2(ay, az)
pitch = 0.96 × (pitch + gy × dt × π/180) + 0.04 × atan2(-ax, √(ay²+az²))
yaw  += gz × dt × π/180   // gyro only
```

### Motion Gauge

Net dynamic acceleration excluding gravity:

```
magnitude = √(ax² + ay² + az²)
net_g     = |magnitude − 1.0|
net_ms²   = net_g × 9.81
```

Smoothed with exponential moving average: `smoothG = 0.8 × prev + 0.2 × net_g`

### Data Rate

ESP8266 broadcasts JSON at ~20Hz (every 50ms):
```json
{ "ax": 0.012, "ay": -0.004, "az": 0.998,
  "gx": 0.31,  "gy": -0.12,  "gz": 0.08 }
```

---

## Project Structure

```
mpu6050-live-dashboard/
├── firmware/
│   └── firmware.ino       # ESP8266 Arduino sketch
├── dashboard/
│   └── index.html         # Single-file web dashboard
├── preview.png            # Screenshot
└── README.md
```

---

## Known Limitations

- **Yaw drifts** over time — no magnetometer reference. Use MPU9250 for drift-free yaw.
- **Speed cannot be derived reliably** from accelerometer alone due to integration drift.
- Dashboard requires both devices on the same local network (no remote access by default).

---

## Built With

- [Three.js](https://threejs.org/) — 3D rendering
- [Tailwind CSS](https://tailwindcss.com/) — Utility styling (docs section)
- [ArduinoJson](https://arduinojson.org/) — JSON on ESP8266
- [WebSockets](https://github.com/Links2004/arduinoWebSockets) — WebSocket server on ESP8266

---

## License

MIT — free to use, modify, and distribute.

---

<p align="center">Made with ❤️ by Om Prakash Munda</p>
