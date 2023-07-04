// Turnout.h
#ifndef Turnout_h
#define Turnout_h

#include <Arduino.h>
#include <ESP32Servo.h>

class Turnout {
  private:
    Servo thisServo;
    void ProcessMoveSlow();
    void ProcessMove();
    int pin;  
    int stepSize;
    int delayTime;
    bool useSlowMotion;
    bool usesDegrees;
    unsigned long millisAtLastChange;
    unsigned long previousMillis;
    
  public:  
    String jMRIId;
    String requiredState;
    int requiredPWMVal;
    int thrownVal;
    int closedVal;
    String currentState; 
    int currentPWMVal;  
    
    Turnout();
    Turnout(String JMRIId);
    Turnout(String JMRIId, int Pin, int ThrownVal, int ClosedVal, bool UsesDegrees);
    Turnout(String JMRIId, int Pin, int ThrownVal, int ClosedVal, bool UsesDegrees, int StepSize, int DelayTime);

    void CheckState();
    void Start();
    void Stop();
};

#endif
