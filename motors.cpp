#include <cstdio>
#include <iostream>
#include <string>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#include "quadrature_encoder.pio.h"

void core1_entry() {
    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);

    gpio_set_function(2, GPIO_FUNC_PWM);
    gpio_set_function(3, GPIO_FUNC_PWM);

    // Below from https://www.i-programmer.info/programming/hardware/14849-the-pico-in-c-basic-pwm.html?start=2

    uint slice_0_1_num = pwm_gpio_to_slice_num(0);

    pwm_set_clkdiv_int_frac (slice_0_1_num, 38, 3);
    pwm_set_wrap(slice_0_1_num, 65465);
    pwm_set_chan_level(slice_0_1_num, PWM_CHAN_A, 4909);
    pwm_set_chan_level(slice_0_1_num, PWM_CHAN_B, 4909);
    pwm_set_enabled(slice_0_1_num, true);

    uint slice_2_3_num = pwm_gpio_to_slice_num(2);

    pwm_set_clkdiv_int_frac (slice_2_3_num, 38, 3);
    pwm_set_wrap(slice_2_3_num, 65465);
    pwm_set_chan_level(slice_2_3_num, PWM_CHAN_A, 4909);
    pwm_set_chan_level(slice_2_3_num, PWM_CHAN_B, 4909);
    pwm_set_enabled(slice_2_3_num, true);

    // Above from i-programmer source

    std::string str = "", before_comma = "", after_comma = "";
    int right_speed = 0, left_speed = 0;
    bool isViableString = true;
    std::string::size_type pos = 0;

    while (1) {
        std::cin >> str;
        pos = str.find(',');
        if (pos != std::string::npos) {
            before_comma = str.substr(0, pos);
            after_comma = str.substr(pos+1);

            isViableString = true;

            isViableString = !(before_comma == "" || after_comma == "");

            for (int i = 0; isViableString && i < before_comma.length(); i++) {
                isViableString = std::isdigit(before_comma.at(i)) || (i == 0 && before_comma.at(i) == '-');
            }
            for (int i = 0; isViableString && i < after_comma.length(); i++) {
                isViableString = std::isdigit(after_comma.at(i)) || (i == 0 && after_comma.at(i) == '-');
            }

            if (isViableString) {
                left_speed = std::stoi(before_comma);
                right_speed = std::stoi(after_comma);
                
                if (left_speed < -100) {
                    left_speed = -100;
                } else if (left_speed > 100) {
                    left_speed = 100;
                }
                
                if (right_speed < -100) {
                    right_speed = -100;
                } else if (right_speed > 100) {
                    right_speed = 100;
                }

                pwm_set_chan_level(slice_0_1_num, PWM_CHAN_A, 4909 + ((float) left_speed) * (1636.0 / 100.0));
                pwm_set_chan_level(slice_0_1_num, PWM_CHAN_B, 4909 + ((float) left_speed) * (1636.0 / 100.0));

                pwm_set_chan_level(slice_2_3_num, PWM_CHAN_A, 4909 + ((float) right_speed) * (1636.0 / 100.0));
                pwm_set_chan_level(slice_2_3_num, PWM_CHAN_B, 4909 + ((float) right_speed) * (1636.0 / 100.0));
            }
        }
    }
}

int main() {
    stdio_init_all();

    multicore_launch_core1(core1_entry);

    // Below from https://github.com/raspberrypi/pico-examples/blob/master/pio/quadrature_encoder/quadrature_encoder.c

    int new_value_left, delta_left, old_value_left = 0, new_value_right, delta_right, old_value_right = 0;

    const uint pio_0_PIN_AB = 10;
    const uint pio_1_PIN_AB = 20;

    const uint sm = 0;

    uint offset0 = pio_add_program(pio0, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio0, sm, offset0, pio_0_PIN_AB, 0);
    uint offset1 = pio_add_program(pio1, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio1, sm, offset0, pio_1_PIN_AB, 0);

    while (1) {
        new_value_left = quadrature_encoder_get_count(pio0, sm);
        // delta_left = new_value_left - old_value_left;
        // old_value_left = new_value_left;
        
        new_value_right = quadrature_encoder_get_count(pio1, sm);
        // delta_right = new_value_right - old_value_right;
        // old_value_right = new_value_right;

        std::cout << new_value_left << "," << new_value_right << std::endl;

        // printf("position left %8d, delta left %6d, position right %8d, delta right %6d\n", new_value_left, delta_left, new_value_right, delta_right);
        sleep_ms(5);
    }
}