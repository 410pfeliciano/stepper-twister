 //    Tetrode Twister Control Software
//    Copyright (C) 2011 Josh Siegle
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.


// ------------------------------ //
//    SOFTWARE VERSION NUMBER:
int ver[2] = {3, 1};
// ------------------------------ //
//    PCB VERSION NUMBER:
int verPCB = 1; 

// -------------------------------------------------------------------- //
/*    DEBUG MODE?
 
 If debugMode is set to 'true', photocell values and thresholds
 will be printed to the serial port. These can be visualized 
 using the accompanying Processing sketch, 'PhotocellVisualization.pde'
 
 If printState is set to 'true', total turns, forward turns, reverse turns,
 button state, and counter state will be printed to the serial port.
 
 */
 
boolean debugMode = false;
boolean printState = true;

/*   SAVE STATE?

 If saveState is set to true, the Arduino will save its current status
 after each turn. The state can then be restored if the Arduino loses
 power or is reset while turning.

*/

boolean saveState = false;

// -------------------------------------------------------------------- //

// BEGIN ACTUAL CODE:

// Includes
#include <AccelStepper.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "TwisterClasses.h"

// Initialize servo and LCD
LiquidCrystal lcd(12,11,5,4,3,2);

// Initialize Stepper object
// 1 = Use chopper driver
// A2 (P38) = step command
// A1 (P37) = direction command 
AccelStepper stepper(1, A2, A1);
StepperControl stepperCont;

// Initialize twister classes

// Buzzer and dials
Buzzer buzzer(6); // Board 1.1 = 6, Board 1.0 = 7
Inputs inputs(7,4,5); // Board 1.1 = 7, Board 1.0 = 6

// State tracker
StateTracker twister;
//Photocells photocells(2, 3);

// EEPROM variables
int remainingTurns;
int turnDirection;

void setup()
{

  // Set default stepper driver options
  stepperCont.setParameters();
  stepper.setMaxSpeed(stepperCont.getMaxSPS());
  stepper.setAcceleration(stepperCont.getMaxSPS()/4);
  
  // start up servo and LCD screen
  lcd.begin(16,2);

  if (debugMode || printState) {
    Serial.begin(9600);
  }
  
    startupScreen(ver[0],ver[1]);  // Display the welcome message
    
    

//  // Read from EEPROM to determine if an error occurred during the last twist:
//  if (saveState) {
//    remainingTurns = EEPROM.read(0);
//    turnDirection = EEPROM.read(1);
//  }
}

void loop()
{
  // Main information update function
  inputs.check(twister.isTurning);
  
  // If we are actually turning the motor
  if (twister.isTurning) { 

	// Are we there yet?
	if (stepper.distanceToGo() == 0) {
            if (twister.isTurningFWD && !twister.finished) {
              
                  
                // Set steppar target distance
                stepperCont.setDistance(inputs.fwdTurns);
    		stepper.moveTo(stepperCont.targetPos);
                twister.isTurningFWD = false;
                twister.finished = false;
            }
            else if (!twister.isTurningFWD && !twister.finished) {
                
              // Indicate turn around
//                buzzer.low();
//                delay(500);
                  
                stepperCont.setDistance(inputs.fwdTurns - inputs.revTurns);
                stepper.moveTo(stepperCont.targetPos);
                twister.isTurningFWD = true;
                twister.finished = true;
            }
            else
            {
              
              // Indicate that we are finished
//               buzzer.low();
//               delay(100);
//               buzzer.high();
//               delay(100);
//                buzzer.low();
//               delay(100);
//               buzzer.high();
//               delay(100);

               
               allDone();
               twister.isTurningFWD = true;
               twister.isTurning = false;
               twister.finished = false;
               twister.isUpdated = true;
               stepper.setCurrentPosition(0);
               delay(2000);
            }
	}
		
	stepper.run();

  }

  
      stepperCont.getCompletedTurns(stepper.currentPosition());
	
      if (twister.totalTurns != stepperCont.completedTurns) {
  	
        twister.totalTurns = stepperCont.completedTurns;
        twister.isUpdated = true; // necessary to update LCD
      }

  showTurns(); // update LCD screen

  if (inputs.buttonState == LOW) { // check for button press
    twister.respondToButton();
    Serial.print("Button pressed\n");
    printVariables();
    delay(250);
  }

  
  //updateServo();
  
//  if (saveState) {
//    updateEEPROM();
//  }


}





