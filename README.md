# rpi-pico-stepper
Very simple program to control a stepper motor using the Pico C SDK via a Raspberry Pi Pico. This does assume that you have some basic knowledge of C, stepper motors, and how to wire/power them appropriately. This code is designed to be easily changed and altered to suit your needs. Once i figure out how to, I will include a basic wiring diagram.


SET UP

It is recommended to follow the directions in the Pico SDK for how to set up the approriate build environment. It was written by people much smarter than me who actually know what they are doing. Make sure that you have installed the SDK and that you export it to path. Once again, all this set up is explained in the documentation, which can be found here https://github.com/raspberrypi/pico-sdk. I would also recommend using the pico-project-generator for starting any future projects https://github.com/raspberrypi/pico-project-generator.


BUILD INSTRUCTIONS

Note: This will not work unless you have properly set up your build environment and installed the SDK to you system
``` 
git clone https://github.com/sgian-ime/rpi-pico-stepper
cd rpi-pico-stepper
mkdir build
cd build
cmake ..
make
```

RUN

Transfer this to your pico, and if everything has gone correctly, it should run no problems. Currently it is very basic, as will only make the stepper motor turn when a button is held down, and will switch to another motor if another button is pressed.
