#include <XInput.h>

// Digital button pin assignments
const uint8_t PIN_BTN_A      = 2;
const uint8_t PIN_BTN_B      = 3;
const uint8_t PIN_BTN_X      = 4;
const uint8_t PIN_BTN_Y      = 5;
const uint8_t PIN_BTN_LB     = 6;
const uint8_t PIN_BTN_RB     = 7;
const uint8_t PIN_BTN_BACK   = 8;
const uint8_t PIN_BTN_START  = 9;
const uint8_t PIN_BTN_GUIDE  = 10;
const uint8_t PIN_BTN_LS     = 11;
const uint8_t PIN_BTN_RS     = 12;

const uint8_t PIN_DPAD_UP    = A8;
const uint8_t PIN_DPAD_DOWN  = A9;
const uint8_t PIN_DPAD_LEFT  = A10;
const uint8_t PIN_DPAD_RIGHT = A11;

// Analog stick pin assignments
const uint8_t PIN_LX = A0;
const uint8_t PIN_LY = A1;
const uint8_t PIN_RX = A2;
const uint8_t PIN_RY = A3;

// Trigger pin assignments
const uint8_t PIN_LT = A4;
const uint8_t PIN_RT = A5;

// Analog calibration constants
const int ANALOG_CENTER = 512;      // Half of the 10-bit ADC range (0-1023)
const int ANALOG_DEADZONE = 40;     // Small deadzone to filter noise
const int ANALOG_MIN = 0;
const int ANALOG_MAX = 1023;

// Utility helper that normalizes a 10-bit analog reading to the
// signed 16-bit range expected by the XInput library for thumbsticks.
int normalizeStick(int rawValue) {
  int offset = rawValue - ANALOG_CENTER;

  if (abs(offset) < ANALOG_DEADZONE) {
    return 0;
  }

  // map() uses long internally; constrain to avoid overflow.
  long mapped = map(offset,
                    ANALOG_MIN - ANALOG_CENTER,
                    ANALOG_MAX - ANALOG_CENTER,
                    -32768,
                    32767);

  if (mapped < -32768) mapped = -32768;
  if (mapped > 32767) mapped = 32767;

  return static_cast<int>(mapped);
}

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
    PIN_BTN_A, PIN_BTN_B, PIN_BTN_X, PIN_BTN_Y,
    PIN_BTN_LB, PIN_BTN_RB, PIN_BTN_BACK, PIN_BTN_START,
    PIN_BTN_GUIDE, PIN_BTN_LS, PIN_BTN_RS,
    PIN_DPAD_UP, PIN_DPAD_DOWN, PIN_DPAD_LEFT, PIN_DPAD_RIGHT
  };

  for (uint8_t pin : buttonPins) {
    pinMode(pin, INPUT_PULLUP);
  }
}

void loop() {
  // Update face buttons
  XInput.setButton(BUTTON_A, readButton(PIN_BTN_A));
  XInput.setButton(BUTTON_B, readButton(PIN_BTN_B));
  XInput.setButton(BUTTON_X, readButton(PIN_BTN_X));
  XInput.setButton(BUTTON_Y, readButton(PIN_BTN_Y));
  XInput.setButton(BUTTON_LB, readButton(PIN_BTN_LB));
  XInput.setButton(BUTTON_RB, readButton(PIN_BTN_RB));
  XInput.setButton(BUTTON_BACK, readButton(PIN_BTN_BACK));
  XInput.setButton(BUTTON_START, readButton(PIN_BTN_START));
  XInput.setButton(BUTTON_GUIDE, readButton(PIN_BTN_GUIDE));
  XInput.setButton(BUTTON_L3, readButton(PIN_BTN_LS));
  XInput.setButton(BUTTON_R3, readButton(PIN_BTN_RS));

  // Update D-Pad (hat switch) directions
  XInput.setDpad(readButton(PIN_DPAD_UP),
                 readButton(PIN_DPAD_DOWN),
                 readButton(PIN_DPAD_LEFT),
                 readButton(PIN_DPAD_RIGHT));

  // Update analog sticks
  XInput.setJoystick(JOY_LEFT,
                     normalizeStick(analogRead(PIN_LX)),
                     normalizeStick(analogRead(PIN_LY)));
  XInput.setJoystick(JOY_RIGHT,
                     normalizeStick(analogRead(PIN_RX)),
                     normalizeStick(analogRead(PIN_RY)));

  // Update triggers
  XInput.setTrigger(TRIGGER_LEFT, normalizeTrigger(analogRead(PIN_LT)));
  XInput.setTrigger(TRIGGER_RIGHT, normalizeTrigger(analogRead(PIN_RT)));

  // Push the update to the host
  XInput.send();

  delay(5); // Small delay to reduce USB traffic
}
