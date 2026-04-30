/**
 * @file board_esp32s3_147.h
 * Board definition: Generic ESP32-S3 with 1.47" LCD (172x320)
 * Pinout based on Smoochie V2 / Bruce firmware
 *
 * MCU:      ESP32-S3
 * Display:  ST7789 172x320 RGB565 via SPI
 */

#pragma once

/* ---- Board metadata ---- */
#define BOARD_NAME        "ESP32-S3 1.47 LCD"
#define BOARD_TARGET      "esp32s3"

/* ---- Hardware Button Pins ---- */
#define BOARD_PIN_BUTTON_BOOT   0   /* BOOT button (active low) */

/* ---- Battery ADC (TP4056 setup) ---- */
/* Wiring: Battery+ -> 100k resistor -> GPIO3 -> 100k resistor -> GND */
/* Voltage divider gives Vbat/2, so 4.2V -> 2.1V (safe for ADC) */
#define BOARD_PIN_BATTERY_ADC   3   /* GPIO3 for battery voltage via voltage divider */
#define BOARD_ADC_DIVIDER_RATIO (2.0f)  /* Vbat/2 for equal 100k/100k divider */

/* ---- Battery Temperature (NTC thermistor) ---- */
/* Wiring: NTC thermistor between BAT_TEMP pin and GND, 10k pull-up to 3.3V */
/* Typical 10k NTC at 25°C, B-value = 3950 */
#define BOARD_PIN_BATTERY_TEMP  4   /* GPIO4 for NTC temperature sensor */
#define BOARD_BATTERY_NTC_B_VALUE   3950  /* B-value of the NTC thermistor */
#define BOARD_BATTERY_NTC_R25       10000 /* NTC resistance at 25°C (10k ohms) */
#define BOARD_BATTERY_NTC_R_PULLUP  10000 /* Pull-up resistor value (10k ohms) */

/* ---- LCD Pins (ST7789 via SPI) ---- */
#define BOARD_PIN_LCD_MOSI      17  /* SDA */
#define BOARD_PIN_LCD_SCLK      18  /* SCL */
#define BOARD_PIN_LCD_DC        15
#define BOARD_PIN_LCD_CS        7
#define BOARD_PIN_LCD_RST       16
#define BOARD_PIN_LCD_BL        6   /* Backlight PWM */

/* ---- LCD Display Configuration ---- */
#define BOARD_LCD_H_RES         320     /* Native width after swap_xy */
#define BOARD_LCD_V_RES         172     /* Native height after swap_xy */
#define BOARD_LCD_SPI_HOST      SPI2_HOST
#define BOARD_LCD_SPI_FREQ_HZ   (40 * 1000 * 1000)
#define BOARD_LCD_CMD_BITS      8
#define BOARD_LCD_PARAM_BITS    8
#define BOARD_LCD_SWAP_XY       true
#define BOARD_LCD_MIRROR_X      true
#define BOARD_LCD_MIRROR_Y      false
#define BOARD_LCD_INVERT_COLOR  true
#define BOARD_LCD_GAP_X         0
#define BOARD_LCD_GAP_Y         34
#define BOARD_LCD_BL_ACTIVE_LOW false   /* Backlight is active-high */
#define BOARD_LCD_COLOR_ORDER_BGR false

/* Flipper framebuffer → display color mapping (RGB565, byte-swapped for SPI) */
#define BOARD_LCD_FG_COLOR      0x20FD  /* Orange 0xFD20 byte-swapped */
#define BOARD_LCD_FG_COLOR_RB   0x20FD  /* Same for now */
#define BOARD_LCD_BG_COLOR      0x0000  /* Black */

/* ---- SD Card Pins (Dedicated SPI3 bus: MOSI=10, SCLK=1, MISO=5, CS=4) ---- */
#define BOARD_PIN_SD_CS         4
#define BOARD_PIN_SD_MISO       5
#define BOARD_PIN_SD_MOSI       10
#define BOARD_PIN_SD_SCLK       1
#define BOARD_SD_SPI_HOST       SPI3_HOST

/* ---- Touch Controller — NOT PRESENT ---- */
#define BOARD_PIN_TOUCH_SCL     UINT16_MAX
#define BOARD_PIN_TOUCH_SDA     UINT16_MAX
#define BOARD_PIN_TOUCH_RST     UINT16_MAX
#define BOARD_PIN_TOUCH_INT     UINT16_MAX
#define BOARD_TOUCH_I2C_ADDR    0x00
#define BOARD_TOUCH_I2C_PORT    I2C_NUM_0
#define BOARD_TOUCH_I2C_FREQ_HZ 0
#define BOARD_TOUCH_I2C_TIMEOUT 0

/* ---- SubGHz / CC1101 — NOT PRESENT ---- */
#define BOARD_PIN_CC1101_SCK    UINT16_MAX
#define BOARD_PIN_CC1101_CSN    UINT16_MAX
#define BOARD_PIN_CC1101_MISO   UINT16_MAX
#define BOARD_PIN_CC1101_MOSI   UINT16_MAX
#define BOARD_PIN_CC1101_GDO0   UINT16_MAX

/* ---- NRF24L01+ ---- */
#define BOARD_PIN_NRF24_CE     38
#define BOARD_PIN_NRF24_CSN    40
#define BOARD_PIN_NRF24_MOSI   48
#define BOARD_PIN_NRF24_MISO    41
#define BOARD_PIN_NRF24_SCK     47

/* ---- Infrared ---- */
#define BOARD_PIN_IR_TX          20
#define BOARD_PIN_IR_RX          21

/* ---- Features ---- */
#define BOARD_HAS_TOUCH         0
#define BOARD_HAS_ENCODER       0
#define BOARD_HAS_SD_CARD       1
#define BOARD_HAS_BLE           1
#define BOARD_HAS_RGB_LED       0
#define BOARD_HAS_VIBRO         0
#define BOARD_HAS_SPEAKER       0
#define BOARD_HAS_IR            1
#define BOARD_HAS_IBUTTON       0
#define BOARD_HAS_RFID          0
#define BOARD_HAS_NFC           0
#define BOARD_HAS_SUBGHZ        0

/* Battery / Power stubs (required for compilation even if not used) */
#define FURI_HAL_POWER_VIRTUAL_CAPACITY_MAH     (1000U)
#define BQ_I2C_PORT             I2C_NUM_0
#define BQ_I2C_SDA              UINT16_MAX
#define BQ_I2C_SCL              UINT16_MAX
#define BQ27220_ADDR            0x55
#define BQ25896_CHARGE_LIMIT    1280
#define HIGH_DRAIN_CURRENT_THRESHOLD (-200)
