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

// Functions for writing to the LCD screen

void startupScreen(int a, int b) {

  lcd.print("Tetrode Stepper");

  lcd.setCursor(0,1);

  lcd.print("for Arduino v");
  lcd.print(a);
  lcd.print(".");
  lcd.print(b);

  delay(2000);     

}

void showTurns() {

  if (inputs.isUpdated || twister.isUpdated) {

    lcd.clear();
    lcd.print("FWD  REV  COUNT");
    lcd.setCursor(0,1);

    printTurns(inputs.fwdTurns);
    printTurns(inputs.revTurns);
    lcd.print("  ");
    printTurns(twister.totalTurns);

    inputs.isUpdated = false;
    twister.isUpdated = false;
  }
}

void showTurningDialog() {

  if (inputs.isUpdated || twister.isUpdated) {

    lcd.clear();
    lcd.print("FWD  REV  COUNT");
    lcd.setCursor(0,1);

    printTurns(inputs.fwdTurns);
    printTurns(inputs.revTurns);
    lcd.print("...");

    inputs.isUpdated = false;
    twister.isUpdated = false;
  }
}

void allDone() {
int r = random(0,20);
lcd.clear();

switch(r) {
  case 0: 
      lcd.print("It is");
      lcd.setCursor(0,1);
      lcd.print("certain.");
      break;
  case 1:  
      lcd.print("It is");
      lcd.setCursor(0,1);
      lcd.print("decidedly so.");
      break;
  case 2:  
      lcd.print("Without");
      lcd.setCursor(0,1);
      lcd.print("a doubt.");
      break;
  case 3:  
      lcd.print("Yes,");
      lcd.setCursor(0,1);
      lcd.print("definitely.");
      break;
  case 4:  
      lcd.print("You may");
      lcd.setCursor(0,1);
      lcd.print("rely on it.");
      break;
  case 5:  
      lcd.print("As I see it,");
      lcd.setCursor(0,1);
      lcd.print("yes.");
      break;
  case 6:  
      lcd.print("Most");
      lcd.setCursor(0,1);
      lcd.print("likely."); 
      break;
  case 7:  
      lcd.print("Outlook");
      lcd.setCursor(0,1);
      lcd.print("good.");
      break;
  case 8:  
      lcd.print("Yes.");
      lcd.setCursor(0,1);
      lcd.print("");
      break;
  case 9:  
      lcd.print("Signs point");
      lcd.setCursor(0,1);
      lcd.print("to yes.");
      break;
  case 10:  
      lcd.print("Reply hazy,");
      lcd.setCursor(0,1);
      lcd.print("try again.");
      break;
  case 11:  
      lcd.print("Ask again");
      lcd.setCursor(0,1);
      lcd.print("later.");
      break;
  case 12:  
      lcd.print("Better not");
      lcd.setCursor(0,1);
      lcd.print("tell you now.");
      break;
  case 13:  
      lcd.print("Cannot predict");
      lcd.setCursor(0,1);
      lcd.print("now.");
      break;
  case 14:  
      lcd.print("Concentrate and");
      lcd.setCursor(0,1);
      lcd.print("ask me again.");
      break;
  case 15:  
      lcd.print("Don't");
      lcd.setCursor(0,1);
      lcd.print("count on it.");
      break;
  case 16:  
      lcd.print("My reply is");
      lcd.setCursor(0,1);
      lcd.print("no.");
      break;
  case 17:  
      lcd.print("My sources say");
      lcd.setCursor(0,1);
      lcd.print("no.");
      break;
  case 18:  
      lcd.print("Outlook not so");
      lcd.setCursor(0,1);
      lcd.print("good.");
      break;
  default: 
      lcd.print("Very");
      lcd.setCursor(0,1);
      lcd.print("doubtful.");
}
}

void printTurns(int nTurns) {

  if (nTurns < 100 && nTurns >= -9) {
    lcd.print(" ");
  }
  if (nTurns < 10 && nTurns >= 0) { 
    lcd.print(" ");
  }
  lcd.print(nTurns);
  lcd.print("  ");

}

void resumeDialog() { // allow user to cancel restart

  boolean resume = true;

  int remainingSeconds = 5;

  lcd.clear();
  lcd.print("Twisting error");
  lcd.setCursor(0,1);
  lcd.print("CONTINUE in ");
  lcd.print(remainingSeconds);

  unsigned long theTime = millis();

  while (remainingSeconds > 0) {
    while (millis() - theTime < 1000) {
      inputs.check(twister.isTurning);
      if (inputs.buttonState == LOW) {
        remainingSeconds = 0;  
        resume = false;
        EEPROM.write(0,0);
        EEPROM.write(1,0);
        delay(500);
        break; 
      }
    }
    lcd.setCursor(12,1);
    remainingSeconds--;
    lcd.print(remainingSeconds);
    theTime = millis();
  }

  if (resume) {
    twister.totalTurns = remainingTurns;
    twister.isTurning = true;
    twister.isUpdated = true;

    switch (turnDirection) {
      case 0:
        twister.isTurningFWD = true;
        break;
      case 1:
        twister.isTurningFWD = false;
        break;
    }
  }

}


