# Jerry XInput Arduino Controller

This repository contains an Arduino sketch that configures a microcontroller capable of running the [XInput](https://github.com/dmadison/ArduinoXInput) firmware as an Xbox 360-compatible controller. The sketch maps digital button inputs, analog sticks, and trigger potentiometers to the appropriate XInput reports.

## Sketch overview

The sketch in [`src/JerryXInputController.ino`](src/JerryXInputController.ino) demonstrates how to:

- Configure button pins with `INPUT_PULLUP` for common-ground wiring harnesses.
- Normalize 10-bit analog readings from thumbsticks and triggers to the signed/unsigned ranges expected by the XInput API.
- Send the full controller state (buttons, D-pad, sticks, triggers) to the host PC at regular intervals.

## Hardware expectations

The pin assignments assume the following connections:

| Function        | Pin |
|-----------------|-----|
| A/B/X/Y         | 2, 3, 4, 5 |
| LB/RB           | 6, 7 |
| Back/Start      | 8, 9 |
| Guide           | 10 |
| Left/Right Stick Click | 11, 12 |
| D-Pad Up/Down/Left/Right | A8, A9, A10, A11 |
| Left Stick X/Y  | A0, A1 |
| Right Stick X/Y | A2, A3 |
| Left/Right Trigger | A4, A5 |

Adjust the constants at the top of the sketch if your wiring uses different pins or if your analog controls need different deadzones or scaling.
