/**
 * @file target_input.c
 * Input driver for analog joystick with smart detection
 * 
 * Analog Joystick: X-axis on GPIO2 (ADC), Y-axis on GPIO3 (ADC)
 * Back button: GPIO9 (active low with pull-up)
 * 
 */

#include "target_input.h"

#include <furi.h>
#include <furi_hal_resources.h>
#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_err.h>
#include <input/input.h>

#define TAG "InputJoystick"

#define GPIO_BACK        11
#define GPIO_OK         9

#define ADC_SAMPLES     4
#define JOY_MIN        1200
#define JOY_MAX        2800

typedef struct {
    const char* name;
    gpio_num_t gpio;
    InputKey key;
    bool pressed;
    bool debounced;
    uint8_t debounce_count;
} JoyButton;

static JoyButton back_button = { "BACK", GPIO_BACK, InputKeyBack, false, false, 0 };
static JoyButton ok_button = { "OK", GPIO_OK, InputKeyOk, false, false, 0 };
static InputKey last_direction = InputKeyMAX;

static adc_oneshot_unit_handle_t adc_handle = NULL;
static int x_avg = 2048, y_avg = 2048;
static uint8_t sample_idx = 0;
static int x_buf[ADC_SAMPLES] = {2048, 2048, 2048, 2048};
static int y_buf[ADC_SAMPLES] = {2048, 2048, 2048, 2048};

static void input_publish(FuriPubSub* pubsub, InputKey key, InputType type, uint32_t sequence) {
    InputEvent event = {
        .sequence_source = INPUT_SEQUENCE_SOURCE_HARDWARE,
        .sequence_counter = sequence,
        .key = key,
        .type = type,
    };
    furi_pubsub_publish(pubsub, &event);
}

static void back_button_init(void) {
    gpio_config_t config_back = {
        .pin_bit_mask = (1ULL << GPIO_BACK),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&config_back);
    back_button.pressed = gpio_get_level(GPIO_BACK) == 0;
    back_button.debounced = back_button.pressed;
    
    gpio_config_t config_ok = {
        .pin_bit_mask = (1ULL << GPIO_OK),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&config_ok);
    ok_button.pressed = gpio_get_level(GPIO_OK) == 0;
    ok_button.debounced = ok_button.pressed;
}

static void adc_init_safe(void) {
    if(adc_handle != NULL) return;
    
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    
    adc_oneshot_new_unit(&init_config, &adc_handle);
    
    adc_oneshot_chan_cfg_t chan_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    
    adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_1, &chan_config);
    adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_2, &chan_config);
    
    FURI_LOG_I(TAG, "ADC oneshot initialized");
}

static void back_button_poll(FuriPubSub* pubsub, uint32_t* sequence) {
    bool raw = gpio_get_level(GPIO_BACK) == 0;
    
    if(raw == back_button.pressed) {
        if(back_button.debounce_count < 3) {
            back_button.debounce_count++;
        }
    } else {
        back_button.pressed = raw;
        back_button.debounce_count = 1;
    }
    
    if(back_button.debounce_count >= 3 && back_button.debounced != back_button.pressed) {
        back_button.debounced = back_button.pressed;
        
        if(back_button.debounced) {
            input_publish(pubsub, InputKeyBack, InputTypePress, *sequence);
            input_publish(pubsub, InputKeyBack, InputTypeShort, *sequence);
            input_publish(pubsub, InputKeyBack, InputTypeRelease, *sequence);
            (*sequence)++;
        }
    }
}

static void ok_button_poll(FuriPubSub* pubsub, uint32_t* sequence) {
    bool raw = gpio_get_level(GPIO_OK) == 0;
    
    if(raw == ok_button.pressed) {
        if(ok_button.debounce_count < 3) {
            ok_button.debounce_count++;
        }
    } else {
        ok_button.pressed = raw;
        ok_button.debounce_count = 1;
    }
    
    if(ok_button.debounce_count >= 3 && ok_button.debounced != ok_button.pressed) {
        ok_button.debounced = ok_button.pressed;
        
        if(ok_button.debounced) {
            input_publish(pubsub, InputKeyOk, InputTypePress, *sequence);
            input_publish(pubsub, InputKeyOk, InputTypeShort, *sequence);
            input_publish(pubsub, InputKeyOk, InputTypeRelease, *sequence);
            (*sequence)++;
        }
    }
}

static void joystick_poll(FuriPubSub* pubsub, uint32_t* sequence) {
    if(adc_handle == NULL) {
        adc_init_safe();
    }
    
    int raw_x, raw_y;
    adc_oneshot_read(adc_handle, ADC_CHANNEL_1, &raw_x);
    adc_oneshot_read(adc_handle, ADC_CHANNEL_2, &raw_y);
    
    x_buf[sample_idx] = raw_x;
    y_buf[sample_idx] = raw_y;
    sample_idx = (sample_idx + 1) % ADC_SAMPLES;
    
    int x_sum = 0, y_sum = 0;
    for(int i = 0; i < ADC_SAMPLES; i++) {
        x_sum += x_buf[i];
        y_sum += y_buf[i];
    }
    x_avg = x_sum / ADC_SAMPLES;
    y_avg = y_sum / ADC_SAMPLES;
    
    InputKey dir = InputKeyMAX;
    
    if(x_avg < JOY_MIN) {
        dir = InputKeyLeft;
    } else if(x_avg > JOY_MAX) {
        dir = InputKeyRight;
    } else if(y_avg < JOY_MIN) {
        dir = InputKeyUp;
    } else if(y_avg > JOY_MAX) {
        dir = InputKeyDown;
    }
    
    if(dir != last_direction) {
        if(last_direction != InputKeyMAX) {
            input_publish(pubsub, last_direction, InputTypeRelease, *sequence);
            (*sequence)++;
        }
        if(dir != InputKeyMAX) {
            input_publish(pubsub, dir, InputTypePress, *sequence);
            input_publish(pubsub, dir, InputTypeShort, *sequence);
            (*sequence)++;
        }
        last_direction = dir;
    }
}

void target_input_init(void) {
    back_button_init();
    FURI_LOG_I(TAG, "Input initialized: BACK=GPIO8, OK=GPIO9");
}

void target_input_poll(FuriPubSub* pubsub, uint32_t* sequence_counter) {
    joystick_poll(pubsub, sequence_counter);
    back_button_poll(pubsub, sequence_counter);
    ok_button_poll(pubsub, sequence_counter);
}