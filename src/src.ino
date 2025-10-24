#include <XInput.h>

// Digital button pin assignments (hardware uses active-low wiring)
const uint8_t PIN_BTN_BACK   = 2;  // Select
const uint8_t PIN_BTN_START  = 3;
const uint8_t PIN_BTN_Y      = 4;
const uint8_t PIN_BTN_X      = 5;
const uint8_t PIN_BTN_RB     = 6;
const uint8_t PIN_BTN_A      = 7;
const uint8_t PIN_BTN_B      = 8;
const uint8_t PIN_BTN_RT     = 9;  // Digital right trigger

const uint8_t PIN_DPAD_UP    = 10;
const uint8_t PIN_DPAD_DOWN  = 11;
const uint8_t PIN_DPAD_LEFT  = 12;
const uint8_t PIN_DPAD_RIGHT = 13;

// Trigger pin assignments
const uint8_t PIN_LT = A4; // Analog left trigger input

// Toggle pin assignment for enabling the left analog stick mirroring
const uint8_t PIN_LS_ENABLE = 1; // Button on D1

// Analog calibration constants
const int ANALOG_MIN = 0;
const int ANALOG_MAX = 1023;

// Utility helper that normalizes a 10-bit analog reading to the
// unsigned 8-bit range expected for analog triggers (0-255).
uint8_t normalizeTrigger(int rawValue) {
  long mapped = map(rawValue, ANALOG_MIN, ANALOG_MAX, 0, 255);
  if (mapped < 0) mapped = 0;
  if (mapped > 255) mapped = 255;
  return static_cast<uint8_t>(mapped);
}

bool readButton(uint8_t pin) {
  return digitalRead(pin) == LOW; // Using INPUT_PULLUP wiring
}

void setup() {
  XInput.begin();

  const uint8_t buttonPins[] = {
    PIN_BTN_BACK, PIN_BTN_START,
    PIN_BTN_Y, PIN_BTN_X, PIN_BTN_RB,
    PIN_BTN_A, PIN_BTN_B, PIN_BTN_RT,
    PIN_DPAD_UP, PIN_DPAD_DOWN, PIN_DPAD_LEFT, PIN_DPAD_RIGHT,
    PIN_LS_ENABLE
  };

  for (uint8_t pin : buttonPins) {
    pinMode(pin, INPUT_PULLUP);
  }
}

void loop() {
  static bool leftStickEnabled = false;
  static bool previousToggleState = false;

  bool togglePressed = readButton(PIN_LS_ENABLE);
  if (togglePressed && !previousToggleState) {
    leftStickEnabled = !leftStickEnabled;
  }
  previousToggleState = togglePressed;

  // Update face buttons
  XInput.setButton(BUTTON_BACK, readButton(PIN_BTN_BACK));
  XInput.setButton(BUTTON_START, readButton(PIN_BTN_START));
  XInput.setButton(BUTTON_Y, readButton(PIN_BTN_Y));
  XInput.setButton(BUTTON_X, readButton(PIN_BTN_X));
  XInput.setButton(BUTTON_RB, readButton(PIN_BTN_RB));
  XInput.setButton(BUTTON_A, readButton(PIN_BTN_A));
  XInput.setButton(BUTTON_B, readButton(PIN_BTN_B));

  bool dpadUp = readButton(PIN_DPAD_UP);
  bool dpadDown = readButton(PIN_DPAD_DOWN);
  bool dpadLeft = readButton(PIN_DPAD_LEFT);
  bool dpadRight = readButton(PIN_DPAD_RIGHT);

  // Update D-Pad (hat switch) directions
  XInput.setDpad(dpadUp, dpadDown, dpadLeft, dpadRight);

  // Update left analog stick based on D-Pad when enabled
  int16_t leftX = 0;
  int16_t leftY = 0;

  if (leftStickEnabled) {
    if (dpadLeft && !dpadRight) {
      leftX = -32768;
    } else if (dpadRight && !dpadLeft) {
      leftX = 32767;
    }

    if (dpadUp && !dpadDown) {
      leftY = -32768;
    } else if (dpadDown && !dpadUp) {
      leftY = 32767;
    }
  }

  XInput.setJoystick(JOY_LEFT, leftX, leftY);

  // Update triggers
  XInput.setTrigger(TRIGGER_LEFT, normalizeTrigger(analogRead(PIN_LT)));
  XInput.setTrigger(TRIGGER_RIGHT, readButton(PIN_BTN_RT) ? 255 : 0);

  // Push the update to the host
  XInput.send();

  delay(5); // Small delay to reduce USB traffic
}