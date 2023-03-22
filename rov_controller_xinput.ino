/*
 *  Project     ROV Controller Firmware
 *  @author     Barnabas Miklos
 *  @link       https://github.com/Vutyuka/rov_controller_xinput
 *  @license    GNU - Copyright (c) 2023 Barnabas Miklos
 *

 * This file is part of GCC.
 * GCC is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3, or (at your option) any later
 * version.

 * GCC is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * You should have received a copy of the GNU General Public License
 * along with GCC; see the file COPYING3.  If not see
 * <http://www.gnu.org/licenses/>.  
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *  Description:  ROV Controller firmware, emulating an XBOX gamepad,
 *                using XInput. An underwater casing will be used,
 *                so all buttons are two-state push buttons.
 *                Buttons represent Hall-effect sensors in the hardware.

 *                Basic controller functionality, easy to add and remove buttons:
 *                1. Add/remove pinout definition for pin.
 *                2. Add/remove pin to button_table.
 *                3. Change button_num.
 *
 *                Tested on Pro Micro(has to be flashed as Leonardo).
 *
 *                To flash: Press reset just as the IDE shows "Uploading" promt
 *                IT WILL BRICK THE DEVICE IF THIS IS NOT DONE.
 *                For more on XInput: https://github.com/dmadison/ArduinoXInput
 *                             .
 */

#include <XInput.h>

/* Type definitions */
typedef struct button_t{
  const uint8_t pin;
  const uint8_t ctrl_button;
}button_t;

/* Pinout */
#define button_num        11

#define forward_pin       7 
#define backward_pin      A1
#define right_pin         14
#define left_pin          16

#define up_pin            10
#define down_pin          2

#define speed_toggle_pin  3

#define cam_up_pin        4
#define cam_down_pin      5

#define lights_toggle_pin 6
#define util_toggle_pin   15

/* Pin function assignment table */

const button_t button_table[button_num]={
  {.pin = forward_pin,        .ctrl_button = DPAD_UP},
  {.pin = backward_pin,       .ctrl_button = DPAD_DOWN},
  {.pin = right_pin,          .ctrl_button = DPAD_RIGHT},
  {.pin = left_pin,           .ctrl_button = DPAD_LEFT},

  {.pin = up_pin,             .ctrl_button = BUTTON_Y},
  {.pin = down_pin,           .ctrl_button = BUTTON_A},

  {.pin = speed_toggle_pin,   .ctrl_button = BUTTON_B},

  {.pin = cam_up_pin,         .ctrl_button = BUTTON_RB},
  {.pin = cam_down_pin,       .ctrl_button = BUTTON_LB},

  {.pin = lights_toggle_pin,  .ctrl_button = BUTTON_R3},
  {.pin = util_toggle_pin,    .ctrl_button = BUTTON_X},
};

void setup() {
  //Every button will be a two-state push button
  //iterating on all the buttons
  for(int i = 0;i<button_num;i++){
    pinMode(button_table[i].pin,INPUT_PULLUP);
  }

	XInput.begin();
  
}

void loop() {

  for(int i = 0;i<button_num;i++){
    //read every button's state
    boolean current_button = !digitalRead(button_table[i].pin);
    //
    XInput.setButton(button_table[i].ctrl_button,current_button);
  }
  //send all the button states to the host
  XInput.send();
}
