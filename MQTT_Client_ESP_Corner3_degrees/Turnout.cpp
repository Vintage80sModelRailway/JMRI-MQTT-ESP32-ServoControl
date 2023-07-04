// Turnout.cpp
#include "Turnout.h"
#include <ESP32Servo.h>

Turnout::Turnout() {
  useSlowMotion = false;
  currentPWMVal = 1500;
  thisServo.setPeriodHertz(50);
  pin = -1;
}

Turnout::Turnout(String JMRIId, int Pin, int ThrownVal, int ClosedVal, bool UsesDegrees) {
  jMRIId = JMRIId;
  thrownVal = ThrownVal;
  closedVal = ClosedVal;
  useSlowMotion = false;
  currentPWMVal = 1500;
  thisServo.setPeriodHertz(50);
  pin = Pin;
  usesDegrees = UsesDegrees;
  if (usesDegrees) {
    currentPWMVal = 90;
  }
  else {
    currentPWMVal = 1500;
  }
}

Turnout::Turnout(String JMRIId, int Pin, int ThrownVal, int ClosedVal, bool UsesDegrees, int StepSize, int DelayTime) {
  jMRIId = JMRIId;
  thrownVal = ThrownVal;
  closedVal = ClosedVal;
  useSlowMotion = true;
  stepSize = StepSize;
  delayTime = DelayTime;
  //currentPWMVal = 1500;
  thisServo.setPeriodHertz(50);
  pin = Pin;
  usesDegrees = UsesDegrees;
}

void Turnout::CheckState() {
  if (requiredState == currentState) {
    thisServo.detach();
    return;
  }

  //Serial.println("Uses slow "+String(useSlowMotion)+" req "+String(requiredPWMVal)+" current "+String(currentPWMVal));
  if (useSlowMotion && requiredPWMVal != currentPWMVal) {
    //Serial.println("Process slow");
    ProcessMoveSlow();
  }
  else {
    thisServo.detach();
  }

  //Serial.println("Req "+String(requiredPWMVal)+" curremt "+String(currentPWMVal));
}

void Turnout::ProcessMoveSlow() {
  //Serial.println("Process move sloe");
  bool moveIsComplete = false;
  unsigned long currentMillis = millis();

  if (requiredPWMVal != currentPWMVal && currentMillis - previousMillis >= delayTime)
  {
    //Serial.println("Turnout "+jMRIId+" current PWM "+String(currentPWMVal));
    previousMillis = currentMillis;
    //Protect thee motors from accidentally silly, potentially damaging values
//    if (requiredPWMVal > 600 && requiredPWMVal < 2351)
//    {
      if (requiredPWMVal > currentPWMVal)
      {
        int intendedPWMValue = currentPWMVal + stepSize;
        if (intendedPWMValue > requiredPWMVal)
        {
          intendedPWMValue = requiredPWMVal;
        }

        currentPWMVal = intendedPWMValue;
        //Serial.println("Intended PWM " + String(intendedPWMValue));
        if (usesDegrees) {
          Serial.println("Degrees move to " + String(intendedPWMValue)+" "+jMRIId);
          thisServo.write(intendedPWMValue);
        } else {
          //Serial.println("Microseconds move to " + String(intendedPWMValue)+" "+jMRIId);
          thisServo.writeMicroseconds(intendedPWMValue);
        }

        if (requiredPWMVal == intendedPWMValue)
        {
          moveIsComplete = true;
        }
      }
      else// (requiredPosition < CurrentPWMValue[pin])
      {
        int intendedPWMValue = currentPWMVal - stepSize;
        if (intendedPWMValue < requiredPWMVal)
        {
          intendedPWMValue = requiredPWMVal;
        }

        currentPWMVal = intendedPWMValue;
        if (usesDegrees) {
          Serial.println("Degrees move to " + String(intendedPWMValue)+" "+jMRIId);
          thisServo.write(intendedPWMValue);
        } else {
          //Serial.println("Microseconds move to " + String(intendedPWMValue)+" "+jMRIId);
          thisServo.writeMicroseconds(intendedPWMValue);
        }
        if (requiredPWMVal == intendedPWMValue)
        {
          moveIsComplete = true;
        }
      }
    //}
    if (moveIsComplete) {
      currentState = requiredState;
      currentPWMVal = requiredPWMVal;
      thisServo.detach();
      Serial.println("Slow move complete current PWM " + String(currentPWMVal)+" "+jMRIId);

    }
  }

  //return moveIsComplete;
}

void Turnout::ProcessMove() {

}

void Turnout::Start() {
  int channel = thisServo.attach(pin);
  Serial.println("Attached "+jMRIId+" pin "+String(pin)+" on channel "+String(channel));
}

void Turnout::Stop() {
  thisServo.detach();
}
