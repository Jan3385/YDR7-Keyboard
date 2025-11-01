# YDR7-Keyboard

A mechanical keyboard build on YD-RP2040 with an OLED display and backlights

Build for MX switches

## Compiling firmware

> ![NOTE]
> You should have CMAKE and GCC installed along with Arm embedded GNU Toolchain `arm-none-eabi-gcc`

For compilation run the following commands in the project folder:
Note that the first command may take a while as it is downloading the SDK from git

> cmake -S Firmware -B Firmware/build -G "Ninja"

> cmake --build Firmware/build --target Keyboard_Firmware