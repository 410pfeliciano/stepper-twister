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


// -------------------------------------------------------------------- //
/*    DEBUG MODE?
 
 If debugMode is set to 'true', photocell values and thresholds
 will be printed to the serial port. These can be visualized 
 using the accompanying Processing sketch, 'PhotocellVisualization.pde'
 
 If printState is set to 'true', total turns, forward turns, reverse turns,
 button state, and counter state will be printed to the serial port.
 
 */

boolean debugMode = false;

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
LiquidCrystal lcd(LCDRS, LCDEN, LCDD4, LCDD5, LCDD6, LCDD7);

// Initialize Stepper object
// 1 = Use chopper driver
// A2 = step command
// A1 = direction command 
AccelStepper stepper(STEPMODE, STEP, DIR);
StepperControl stepperCont;

// Buzzer and dials
Buzzer buzzer(BUZZER); // Board 1.1 = 6, Board 1.0 = 7
Inputs inputs(STARTSTOP,KNOB1,KNOB2); // Board 1.1 = 7, Board 1.0 = 6

// State tracker
StateTracker twister;

// EEPROM variables
int remainingTurns;
int turnDirection;

void setup()
{

	// Set default stepper driver options
	stepperCont.setParameters();
	stepper.setMaxSpeed(stepperCont.getMaxSPS());
	stepper.setAcceleration(stepperCont.getMaxSPS());

	// start up servo and LCD screen
	lcd.begin(16,2);

	if (debugMode) {
		Serial.begin(9600);
	}

	startupScreen(0,1);  // Display the welcome message
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
				buzzer.low();
				delay(500);

				stepperCont.setDistance(-inputs.revTurns);
				stepper.move(stepperCont.targetPos);
				twister.isTurningFWD = true;
				twister.finished = true;
			}
			else {

				// Indicate that we are finished
				buzzer.low();
				delay(100);
				buzzer.high();
				delay(100);
				buzzer.low();
				delay(100);
				buzzer.high();
				delay(100);

				// Reset the state machine
				allDone();
				twister.isTurningFWD = true;
				twister.isTurning = false;
				twister.finished = false;
				twister.isUpdated = true;
				stepper.setCurrentPosition(0);
				delay(2000);
			}
		}

		// Take a step
		stepper.run();

	}
	else {

		stepperCont.getCompletedTurns(stepper.currentPosition());

		if (twister.totalTurns != stepperCont.completedTurns) {
			twister.totalTurns = stepperCont.completedTurns;
			twister.isUpdated = true; // necessary to update LCD
		}

		// Update the number of completed turns
		showTurns(); // update LCD screen
	}

	if (inputs.buttonState == LOW) { // check for button press
		twister.respondToButton();
                twister.isUpdated = true;
		if (twister.isTurning) {
			showTurningDialog();

                        // Re call moveTo and reset the position to restart Accel process
                        long toGo = stepper.distanceToGo();
                        stepper.setCurrentPosition(0);
                        stepper.moveTo(toGo);
                }
		
		delay(250);
	}
}







