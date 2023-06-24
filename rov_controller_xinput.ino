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
 *                Using Left Joystick with 4 push buttons. 
 *                Joystick position/speed can be toggled by push button
 *                Tested on Pro Micro(has to be flashed as Leonardo).
 *
 *                To flash: Press reset just as the IDE shows "Uploading" promt
 *                IT WILL BRICK THE DEVICE IF THIS IS NOT DONE.
 *                For more on XInput: https://github.com/dmadison/ArduinoXInput
 *                             .
 */

#include <XInput.h>

/* Pinout */
#define button_num        11

#define forward_pin       3
#define backward_pin      A1
#define right_pin         14
#define left_pin          16

#define up_pin            10
#define down_pin          2

#define speed_toggle_pin  7

#define cam_up_pin        4
#define cam_down_pin      5

#define lights_toggle_pin 6
#define util_toggle_pin   15

#define the_joystick      JOY_LEFT

/* Type definitions */
typedef struct button_t{
  const uint8_t pin;
  const uint8_t ctrl_button;
}button_t;

typedef int motor_speed_t;

enum motor_speed{
  NEUTRAL,
  SLOW,
  FAST,
  SPEED_STATE_NUM ,
};

typedef struct joystick_coords_t{
  int x;
  int y;  
 }joystick_coords_t;

/* Pin function assignment table */

const button_t button_table[button_num]={
  {.pin = forward_pin,        .ctrl_button = JOY_LEFT},
  {.pin = backward_pin,       .ctrl_button = JOY_LEFT},
  {.pin = right_pin,          .ctrl_button = JOY_LEFT},
  {.pin = left_pin,           .ctrl_button = JOY_LEFT},

  {.pin = up_pin,             .ctrl_button = BUTTON_Y},
  {.pin = down_pin,           .ctrl_button = BUTTON_A},

  {.pin = speed_toggle_pin,   .ctrl_button = BUTTON_B},

  {.pin = cam_up_pin,         .ctrl_button = BUTTON_RB},
  {.pin = cam_down_pin,       .ctrl_button = BUTTON_LB},

  {.pin = lights_toggle_pin,  .ctrl_button = BUTTON_R3},
  {.pin = util_toggle_pin,    .ctrl_button = BUTTON_X},
};




static boolean toggle_prev_state;
static motor_speed_t stick_state = SLOW;
static joystick_coords_t joystick_xy = {0,0};

void increase_motor_speed(motor_speed_t * stick_state){
  stick_state[0]=stick_state[0]+1;

  if(stick_state[0] == SPEED_STATE_NUM){
    stick_state[0] = stick_state[0]%SPEED_STATE_NUM + 1; //to skip 0 and SPEED_STATE_NUM
  }
}

void setup() {
  //Every button will be a two-state push button
  //iterating on all the buttons

  // joystick will  be for push buttons
  XInput.setJoystickRange(-(FAST), FAST);
  XInput.setJoystick(the_joystick,joystick_xy.x,joystick_xy.y);
  
  for(int i = 0;i<button_num;i++){
    pinMode(button_table[i].pin,INPUT_PULLUP);
  }

  toggle_prev_state = 0;


	XInput.begin();
  
}

void loop() {
  //zeroing joystick values to be able to use addition at setting
  joystick_xy = {0,0};
  
  for(int i = 0;i<button_num;i++){
    //read every button's state
    bool current_button = !digitalRead(button_table[i].pin);
    //handling the push buttons as a joystick, getting pos from stick_state, set with button
    if(button_table[i].ctrl_button == the_joystick ){
      
      switch(button_table[i].pin){
        case forward_pin:
          joystick_xy.y += stick_state*current_button; // += to handle simultaneous button pushes
          break;
        case backward_pin:
          joystick_xy.y += -stick_state*current_button; // if zero nothing is done
          break;
        case right_pin:
          joystick_xy.x += stick_state*current_button;
          break;
        case left_pin:
          joystick_xy.x += -stick_state*current_button;
          break;
      }
       
     //setting speed state here with toggling button
    }else if(button_table[i].pin == speed_toggle_pin){
     
      
      if(!toggle_prev_state && current_button){ // only toggle from 0->1 change,  on button push
//        static bool toggle_indicator = 0;
//        toggle_indicator = !toggle_indicator;
//        XInput.setButton(button_table[i].ctrl_button,toggle_indicator);

          stick_state = stick_state%(SPEED_STATE_NUM-1) + 1;//cycling through discrete speeds and skipping 0 and SPEED_STATE_NUM
          
      }
      toggle_prev_state = current_button;

    }else{
      XInput.setButton(button_table[i].ctrl_button,current_button);
    }
  }
  //setting joystick after all the buttons states are read
  XInput.setJoystick(the_joystick,joystick_xy.x,joystick_xy.y);
  //send all the button states to the host
  XInput.send();
}
