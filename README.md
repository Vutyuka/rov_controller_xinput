# ROV Controller firmware

ROV Controller firmware, emulating an XBOX gamepad,
using XInput. An underwater casing will be used,
so all buttons are two-state push buttons.
Buttons represent Hall-effect sensors in the hardware.

Basic controller functionality, easy to add and remove buttons:

1. Add/remove pinout definition for pin.
2. Add/remove pin to button_table.
3. Change button_num.

Using Left Joystick with 4 push buttons. 
Joystick position/speed can be toggled by push button.

Any number of discrete speeds can be added to motor_speed_t.

Tested on Pro Micro(has to be flashed as Leonardo).

**To flash:** Press reset just as the IDE shows "Uploading" promt
IT WILL BRICK THE DEVICE IF THIS IS NOT DONE.

For more on XInput: https://github.com/dmadison/ArduinoXInput