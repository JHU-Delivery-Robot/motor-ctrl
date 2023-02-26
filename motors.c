#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#include "quadrature_encoder.pio.h"

int main() {
    stdio_init_all();

    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);

    gpio_set_function(2, GPIO_FUNC_PWM);
    gpio_set_function(3, GPIO_FUNC_PWM);

    // Below from https://www.i-programmer.info/programming/hardware/14849-the-pico-in-c-basic-pwm.html?start=2

    uint slice_0_1_num = pwm_gpio_to_slice_num(0);

    pwm_set_clkdiv_int_frac (slice_0_1_num, 38, 3);
    pwm_set_wrap(slice_0_1_num, 65465);
    pwm_set_chan_level(slice_0_1_num, PWM_CHAN_A, 6546);
    pwm_set_chan_level(slice_0_1_num, PWM_CHAN_B, 6546);
    pwm_set_enabled(slice_0_1_num, true);

    uint slice_2_3_num = pwm_gpio_to_slice_num(2);

    pwm_set_clkdiv_int_frac (slice_2_3_num, 38, 3);
    pwm_set_wrap(slice_2_3_num, 65465);
    pwm_set_chan_level(slice_2_3_num, PWM_CHAN_A, 3273);
    pwm_set_chan_level(slice_2_3_num, PWM_CHAN_B, 3273);
    pwm_set_enabled(slice_2_3_num, true);

    // Above from i-programmer source

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
        delta_left = new_value_left - old_value_left;
        old_value_left = new_value_left;
        
        new_value_right = quadrature_encoder_get_count(pio1, sm);
        delta_right = new_value_right - old_value_right;
        old_value_right = new_value_right;

        printf("position left %8d, delta left %6d, position right %8d, delta right %6d\n", new_value_left, delta_left, new_value_right, delta_right);
        sleep_ms(100);
    }
}