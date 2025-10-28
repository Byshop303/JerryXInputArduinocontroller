# Jerry XInput Arduino Controller

This repository contains the Arduino sketch that powers my custom arcade stick. The stick uses an Arduino Leonardo (or any other ATmega32u4 board) running [dmadison's XInput library](https://github.com/dmadison/ArduinoXInput) so the PC sees it as an Xbox 360-compatible fight stick.

## What the sketch does

The sketch in [`src/src.ino`](src/src.ino):

- Configures every control input as an active-low digital button using `INPUT_PULLUP`, matching common-ground wiring harnesses used for arcade sticks.
- Maps the buttons to the corresponding XInput face buttons, start/back, bumper, and D-pad inputs.
- Treats the right trigger as a digital button that sends either a fully pressed or fully released value.
- Provides a toggle (wired to pin `D1`) that mirrors the D-pad onto the left analog stick for games that only read stick input.
- Sends the full controller state to the host over USB using XInput.

There are no analog thumbsticks or analog trigger readings in this build—every control is digital, making it ideal for arcade lever/button layouts.

## Pin assignments

| Function                      | Pin |
|-------------------------------|-----|
| Back / Select                 | 2   |
| Start                         | 3   |
| Y                             | 4   |
| X                             | 5   |
| RB                            | 6   |
| A                             | 7   |
| B                             | 8   |
| Right Trigger (digital)       | 9   |
| D-Pad Up / Down / Left / Right| 10 / 11 / 12 / 13 |
| Left Stick toggle (mirror)    | 1   |

If your layout uses different pins, adjust the constants defined at the top of [`src/src.ino`](src/src.ino).

## Getting started

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) and add the **ArduinoXInput** library by following the instructions in its repository.
2. Open `src/src.ino` in the IDE.
3. Select **Arduino Leonardo** (or another ATmega32u4-based board) under **Tools → Board**.
4. Upload the sketch to your board.

Once uploaded, the arcade stick enumerates as an XInput controller and is ready for PC or console use that accepts Xbox 360-compatible devices.
