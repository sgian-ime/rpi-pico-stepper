#include "pico/stdlib.h"

// Define masks that will be used to set pins

#define SET_ALL_MASK    0x1ffc // 1111111111100
#define MOTOR_MASK      0x3fc  // 0001111111100
#define BUTTON_MASK     0x1c00 // 1110000000000

// Define the first GPIO pin which is used to 
// drive a motor, and the total number of pins
// used by a motor

#define FIRST_MOTOR     2
#define MOTOR_OFFSET    4

// Define the pins used to turn the motor left, right
// and the pin to switch motors

#define LEFT_BUTTON     10
#define RIGHT_BUTTON    11
#define SWITCH_BUTTON   12

// This could be done more efficiently by someone smarter, but this
// works for my very basic use case

const char spin_left[4] = {
    1, 2, 4, 8,
};

const char spin_right[4] = {
    8, 4, 2, 1
};

// A niave way of doing it, buy hey it runs. Can pass which motor
// to turn, starting from 0, how many steps to turn, which direction,
// and how long to wait between turns in milliseconds. 
// This was written for a ROHS 28BYJ-48 stepper motor, but this function
// should be compatable for a wide array of different motors. For my motor,
// motor takes 512 stesp to make a full revolution, and can be passed a minimum
// sleep time of 2ms. Your milage may vary, have fun tinkering :)

void motor_turn (char motor, int steps, const char direction[4], int sleep_time) {
    // This sets how much the steps in the direction array will be bit shifted
    // so that they align with the correct motor
    int shift = FIRST_MOTOR + (MOTOR_OFFSET * motor);

    for (int i = 0; i < steps; i++) {
        // Step through the direction array, bit shift each step by the correct
        // amount, then apply that mask to the motor pins so that they activate,
        // sleep, then deactivate them. Continue for all pins.
        for (int val = 0; val < 4; val++) {
            int mask = direction[val] << shift;
            gpio_set_mask(mask);
            sleep_ms(sleep_time);
            gpio_clr_mask(mask);
        }
    }
}

// The same function, but it just always does one turn, slightly more effiecent
// for the below code.

void single_motor_turn (char motor, const char direction[4], int sleep_time) {

    int shift = FIRST_MOTOR + (MOTOR_OFFSET * motor);

    for (int val = 0; val < 4; val++) {
        int mask = direction[val] << shift;
        gpio_set_mask(mask);
        sleep_ms(sleep_time);
        gpio_clr_mask(mask);
    }
}

// I am aware that it becomes slightly inefficent to pass an array to the funciton,
// bit shift it, and reference an index every time a motor needs to turn, but I find
// that passing an array that makes the code easier to read, and I'd rather have 
// slightly inefficent, easy to read and understand code than super optimised code that
// requires a degree in rocket surgery to understand, especially given the use case being
// a very simple motor turning program.

int main() {
    
    // Initialise the pins, and sets them up based on the defined masks
    
    stdio_init_all();
    
    gpio_init_mask(SET_ALL_MASK);

    gpio_set_dir_out_masked(MOTOR_MASK);
    gpio_set_dir_in_masked(BUTTON_MASK);

    gpio_pull_up(LEFT_BUTTON);
    gpio_pull_up(RIGHT_BUTTON);
    gpio_pull_up(SWITCH_BUTTON);

    // Very simple while loop which runs a function when a certain button
    // is pressed/held.

    char current_motor = 0; 
    while (true) {
        
        if (!gpio_get(SWITCH_BUTTON)) {
            
            // XORs the current motor variable, and gives a slight delay
            // I've found that with out the slight delay is just keeps 
            // switching too quickly to be useful
            
            sleep_ms(100);
            current_motor ^= 1;
            sleep_ms(100);
        }
        
        if (!gpio_get(LEFT_BUTTON)) {
            single_motor_turn(current_motor, spin_left, 2);
        }
        
        if (!gpio_get(RIGHT_BUTTON)) {
            single_motor_turn(current_motor, spin_right, 2);
        }
    };
    return 0;
}
