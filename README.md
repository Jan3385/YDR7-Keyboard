# YDR7-Keyboard

A mechanical keyboard build on YD-RP2040 with an OLED display and backlights

Build for **MX** switches

A sligthly modified 96% keyboard layout with 4 extra keys for managing onboard firmware (103 keys total)

![Keyboard layout](/keyboard-layout.svg)

![Keyboard parts](/CAD/parts.png)

## Why is this one special

It's a keyboard with a customisable OLED that helps to manage, and control parts of the firmware like changing backlighting color, showing typing speed and much more

The firmware is also open-source so anyone can customise and add features to their keyboard

## PCB

![PCB](/PCB/PCB.png)

![Schematic](/PCB/schematic.png)

The keyboard uses a custom PCB with the YD-RP2040 attached to it providing a USB-C connection along with power to the board

The YD-RP2040 is the devboard with a chip running this project. You may be able to use a standard Raspberry PI pico but that was not tested

## Case

The keyboard is comprised out of 3 parts. The panel which holds the switches, a back panel and a front panel

Due to their size its impossible to print an entire piece at once, thats why there are sliced versions in the `CAD/` folder. You can print them separately and then weld them with a soldering iron together. You should weld the front and the back piece but not the panels as they are held together by the frame anyway

![Sliced case](/CAD/Sliced/slided-parts.png)

> ![WARNING]
> Due to the size of the keyboard, even the sliced version requires minimal 216 x 139 mm printing plate

## Compiling & flashing firmware

> ![NOTE]
> You should have CMAKE and GCC installed along with Arm embedded GNU Toolchain `arm-none-eabi-gcc`

For compilation run the following commands in the project folder:
Note that the first command may take a while as it is downloading the SDK from git

> cmake -S Firmware -B Firmware/build -G "Ninja"

> cmake --build Firmware/build --target Keyboard_Firmware

To load the firmware into the RP2040 chip, press the **BOOT** button on the YD-RP2040 devboard and plug it into your computer. It should appear as a drive in which you insert the `Keyboard_Firmware.uf2` file inside the `Firmware/build/` folder. The keyboard should reboot itself and load the active firmware

To reflash the board, just repeat the steps above

## BOM

| Product name | Product description | Product link | Product unit cost inc. tax (€) | Product amount | Product total cost (€) | Running total (€ with tax) |
|---------------|----------------------|---------------|----------------------------------|----------------|------------------------|-----------------------------|
| YD-RP2040 | RPI pico-like microcontroller board | [link](https://www.aliexpress.com/item/1005006152339711.html) | 2.99€ | 1 | 2.99€ | 2.99€ |
| 74AHCT125DR | Logic level shifter IO | [link](https://www.aliexpress.com/item/1005010022053816.html) | 3.01€ | 1 | 3.01€ | 6.00€ |
| 100nF 0805 Ceramic Capacitor (>= 6.3V) | Decoupling capacitor | [link](https://www.aliexpress.com/item/1005007470747384.html) | 0.0173€ | 104 | 1.79€ | 7.79€ |
| 10kΩ 0805 Resistor | Pull-up resistor | [link](https://www.aliexpress.com/item/1005009805187579.html) | 0.019€ | 2 | 0.038€ | 7.83€ |
| 1N4148W Diode | Keyboard matrix diode | [link](https://www.aliexpress.com/item/4000685043735.html) | 0.0172€ | 103 | 1.77€ | 9.60€ |
| Kailh Hot-Swap Sockets for Gateron | PCB sockets for switches | [link](https://www.aliexpress.com/item/1005009594313632.html) | 8.24€/150 | 103 | 8.24€ | 17.84€ |
| Gateron Switch G Pro 3.0 | Mechanical keyboard switches | [link](https://kurokeeb.com/products/gateron-switch-g-pro-3-0) | 0.25€ | 103 | 25.75€ | 43.59€ |
| 1.5 Inch OLED Display 128x128 I2C | OLED Screen | [link](https://www.aliexpress.com/item/1005008534123571.html) | 6.79€ | 1 | 6.79€ | 50.38€ |
| SK6812 Mini RGB LED | Backlight LEDs | [link](https://www.fruugo.cz/200-ks-sk6812-mini-e-rgb-podobne-ws2812b-sk6812-3228-smd-pixelu-led/p-96905983-203991007) | 11.06€/200 | 103 | 11.06€ | 61.44€ |
| Keycap set | set of keycaps for keyboard | [link](https://www.aliexpress.com/item/1005007996240625.html) | 16.93€ | 1 | 16.93€ | 78.37€ |
| M3x8 screws | screws for PCB and case assembly | [link](https://www.aliexpress.com/item/1005008585550992.html) | 0.05€ | 8 | 0.40€ | 78.77€ |
| Hot melt inserts MxL4xOD4.2 | inserts for case assembly | [link](https://www.aliexpress.com/item/1005006071488810.html) | 0.11€ | 8 | 0.88€ | 79.65€ |
| Silicone feet 12x3mm | anti-slip feet for case | [link](https://www.aliexpress.com/item/1005006954839245.html) | 0.72€/4 | 4 | 0.72€ | 80.37€ |
