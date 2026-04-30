# Build Instructions

## Prerequisites

- ESP-IDF installed (v5.x recommended)
- ESP-IDF environment variables set or export script path known

## Build Scripts

Use the provided shell scripts to build and flash:

| Board | Script |
|-------|--------|
| ESP32-S3 1.47" LCD | `./buildAndFlash_esp32s3_147.sh` |
| LilyGo T-Embed CC1101 | `./buildAndFlash_T-Embed.sh` |
| Waveshare ESP32-C6 1.9" | `./buildAndFlash_waveshare_c6_1.9.sh` |

## Usage

```bash
./buildAndFlash_esp32s3_147.sh [options]
```

### Options

- `--port <device>` - Serial port (e.g., `/dev/ttyACM0`, `/dev/cu.usbmodem*`)
- `--monitor` - Open serial monitor after flashing
- `--build-only` - Build without flashing
- `--help` - Show help

### Examples

```bash
# Build and flash with auto-detected port
./buildAndFlash_esp32s3_147.sh

# Build and flash with monitor
./buildAndFlash_esp32s3_147.sh --port /dev/ttyACM0 --monitor

# Build only
./buildAndFlash_esp32s3_147.sh --build-only
```

## Battery Monitoring Setup (ESP32-S3 1.47" LCD)

### Hardware Connections

**Battery Voltage (ADC):**
- Battery+ → 100k resistor → GPIO3 → 100k resistor → GND
- Voltage divider gives Vbat/2 (4.2V → 2.1V, safe for ESP32 ADC)

**Battery Temperature (NTC):**
- NTC thermistor between BAT_TEMP pin (GPIO4) and GND
- 10k pull-up resistor from GPIO4 to 3.3V
- Uses typical 10k NTC with B-value = 3950

### Software Support

The board header (`board_esp32s3_147.h`) is configured with:
- `BOARD_PIN_BATTERY_ADC` = GPIO3
- `BOARD_ADC_DIVIDER_RATIO` = 2.0 (for equal 100k/100k divider)
- `BOARD_PIN_BATTERY_TEMP` = GPIO4
- NTC parameters: R25 = 10kΩ, B-value = 3950

Battery voltage and temperature will appear in Flipper settings automatically.

## Environment Variables

- `ESPPORT` - Override default serial port
- `ESP_IDF_EXPORT_SCRIPT` - Path to ESP-IDF export script (default: `~/esp/esp-idf/export.sh`)

## Build Directory

Each script uses a separate build directory:
- `build_esp32s3_147/` - ESP32-S3 1.47" LCD builds
- `build_t_embed/` - T-Embed builds
- `build_waveshare_c6/` - Waveshare builds

Clean build by deleting the build directory.
