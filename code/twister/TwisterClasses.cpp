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


#include "TwisterClasses.h"

// Buzzer methods:

Buzzer::Buzzer(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Buzzer::high()
{
  tone(_pin,2000,200);
}

void Buzzer::low()
{
  tone(_pin,1000,200);
  
}

// Inputs methods:

Inputs::Inputs(int pin1, int pin2, int pin3)
{
	pinMode(pin1, INPUT);
	_buttonPin = pin1;
	_knobPin1 = pin2;
	_knobPin2 = pin3;
	buttonState = 0;
	fwdTurns = 0;
	revTurns = 0;
	isUpdated = true;
}

void Inputs::check(boolean isTurning)
{
	buttonState = digitalRead(_buttonPin); // read button state

	if (!isTurning) { // allow changes only if twister isn't turning

		// Board ver 1.1:
		int knob1 = analogRead(_knobPin1) / 4 * 100 / 255;
		int knob2 = analogRead(_knobPin2) / 4 * 100 / 255;

		// constrain values to be between 0 and 100
		knob1 = min(knob1,100); 
		knob1 = max(knob1,0);
		knob2 = min(knob2,100); 
		knob2 = max(knob2,0);

		if (knob1 != fwdTurns || knob2 != revTurns) { // check to see if values have been updated
			isUpdated = true;
		}

		Serial.print("knob 1: ");
		Serial.print(knob1);
		Serial.print("\n");
		fwdTurns = knob1;
		revTurns = knob2;

	}
}


// StateTracker methods:

StateTracker::StateTracker()
{
	isTurning = false;
	isTurningFWD = true;
	isUpdated = false;
	totalTurns = 0;
	finished = false;
}

void StateTracker::respondToButton()
{
	isTurning = !(isTurning);
}

// StepperControl Methods:

StepperControl::StepperControl()
{
	maxRPS = 20;
	stepen = true;
	ms1 = false;
	ms2 = true;
	pfd = true;
}

void StepperControl::setParameters(void)
{	
	// Set pin modes
	pinMode(STEPEN, OUTPUT);
	pinMode(MS1, OUTPUT);
	pinMode(MS2, OUTPUT);
	pinMode(PFD, OUTPUT);

	// Set microstep resolution
	if (!ms1 & !ms2)
		_nUSteps = 1;
	else if (ms1 & !ms2)
		_nUSteps = 2;
	else if (!ms1 & ms2)
		_nUSteps = 4;
	else
		_nUSteps = 8;

	// Set the max speed (steps/second)
	_maxSPS = (float)(maxRPS*SPR*_nUSteps);

	// Write to the parameter pins
	digitalWrite(STEPEN,!stepen);
	digitalWrite(MS1, ms1);
	digitalWrite(MS2, ms2); 
	digitalWrite(PFD, pfd); 
}

void StepperControl::setDirection(bool forward)
{
	stepDir = forward;
}

void StepperControl::setDistance(int turns)
{
	targetPos = (long)(turns*_nUSteps*SPR);
}

void StepperControl::getCompletedTurns(long currentCompletedSteps)
{
	completedTurns = ceil(currentCompletedSteps/(long)(_nUSteps*SPR));
}

float StepperControl::getMaxSPS(void)
{
   return _maxSPS; 
}

