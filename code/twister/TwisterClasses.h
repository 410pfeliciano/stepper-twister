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


#ifndef TwisterClasses_h
#define TwisterClasses_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

// Stepper pins and constants
#define STEPMODE		1 		// Stepper mode 1 = use driver
#define SPR				48	 	// steps per rotation for ST-PM35
#define DIR				15		// A1, step direction
#define STEP			16		// A2, step command
#define PFD				17 		// A3, percent fast decay
#define MS1				10 		// microstep config 1
#define MS2				9 		// microstep config 2
#define STEPEN			8		// Step enable

// Input pins
#define STARTSTOP       7		// stop/start button
#define KNOB1			4 		// knob 1 analog input
#define KNOB2			5 		// knob 2 analog input

// Buzzer
#define BUZZER			6 		// Buzzer pin

// LCD
#define LCDRS			12	 	// register select
#define LCDEN			11		// enable
#define LCDD4			5		// data pin
#define LCDD5			4		// data pin
#define LCDD6			3		// data pin
#define LCDD7			2		// data pin

#define PFD				17 		// A3, percent fast decay
#define MS1				10 		// microstep config 1
#define MS2				9 		// microstep config 2
#define STEPEN			8		// Step enable


class Buzzer
{
  public:
    Buzzer(int pin);
    void high();
    void low();
  private:
	int _pin;
};

class Inputs
{
  public:
    int buttonState, fwdTurns, revTurns;
    boolean isUpdated;
    Inputs(int pin1, int pin2, int pin3);
    void check(boolean isTurning);
  private:
    int _buttonPin, _knobPin1, _knobPin2;
};
    
    
class StateTracker
{
  public:
    int totalTurns;
    boolean isUpdated;
    boolean isTurning;
    boolean isTurningFWD;
    boolean finished;
    
    StateTracker();
    void respondToButton();
};

class StepperControl
{
	public:
		StepperControl();
		float maxRPS;
		boolean stepen;
		boolean ms1;
		boolean ms2;
		boolean pfd;
                boolean stepDir;
                long targetPos;
		int completedTurns;
		void setParameters(void);
                void setDirection(bool forward);
		void updatePosition();
		void setDistance(int nturns);
                void getCompletedTurns(long currentCompletedSteps);
                float getMaxSPS(void);
	private:
		int _nUSteps; 	// number of microsteps/step
		int _maxSPS;	// max microsteps/second
  	
};
    
//class Photocells
//{
//  public:
//    int values[2];
//    int thresh[2];
//    unsigned int counter;
//    
//    Photocells(int pin1, int pin2);
//    
//    int update(boolean dir, long time);
//    
//  private:
//    int pins[2];
//    int nValues;
//    int history1[25];
//    int history2[25];
//    int maxVal[2];
//    int minVal[2];
//    int quarterTurns;
//    boolean canBeTriggered[2];
//    unsigned long triggerTime[2];
//    void checkThreshold();
//};

#endif


