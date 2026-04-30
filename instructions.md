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

## Environment Variables

- `ESPPORT` - Override default serial port
- `ESP_IDF_EXPORT_SCRIPT` - Path to ESP-IDF export script (default: `~/esp/esp-idf/export.sh`)

## Build Directory

Each script uses a separate build directory:
- `build_esp32s3_147/` - ESP32-S3 1.47" LCD builds
- `build_t_embed/` - T-Embed builds
- `build_waveshare_c6/` - Waveshare builds

Clean build by deleting the build directory.
