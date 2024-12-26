#include <Servo.h>
 
int servoPin = 9;
int coldButtonPin = 5;
int hotButtonPin  = 4;
 
Servo servo;

String incomingString = ""; // for incoming serial data

int outputHotPin = 7;
int outputColdPin = 8;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(outputHotPin, OUTPUT);
  pinMode(outputColdPin, OUTPUT);
  pinMode(coldButtonPin, INPUT);
  pinMode(hotButtonPin, INPUT);
  servo.attach(servoPin);
  Serial.begin(115200);
}

void loop() {
  //delay(1000);
  // send data only when you receive data:
  //Serial.println(Serial.available());
  if(digitalRead(coldButtonPin) || digitalRead(hotButtonPin))
  {
      if (digitalRead(coldButtonPin))
      {
        digitalWrite(outputColdPin, !digitalRead(outputColdPin));
        
      }
      if (digitalRead(hotButtonPin))
      {
        digitalWrite(outputHotPin, !digitalRead(outputHotPin));
      }
      while(digitalRead(coldButtonPin) || digitalRead(hotButtonPin))
      {Serial.println(digitalRead(coldButtonPin));}
  }
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingString = Serial.readString();

    // say what you got:
    //Serial.print("I received: ");
    //Serial.println(incomingString);


    if(incomingString == "on")
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(outputColdPin, HIGH);
      digitalWrite(outputHotPin, HIGH);
    }
    else if(incomingString == "off")
    {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(outputColdPin, LOW);
      digitalWrite(outputHotPin, LOW);
    }
    else if(incomingString == "HOT1")
    {
      digitalWrite(outputHotPin, HIGH);
    }
    else if(incomingString == "hot0")
    {
      digitalWrite(outputHotPin, LOW);
    }
    else if(incomingString == "COLD3")
    {
      digitalWrite(outputColdPin, HIGH);
    }
    else if(incomingString == "cold2")
    {
      digitalWrite(outputColdPin, LOW);
    }
    else if(incomingString == "flush")
    {
       servo.write(0);
      delay(2000);
      servo.write(180);
      delay(2000);
      servo.write(0);
  
    }
    
    Serial.flush(); 

    

    
    
  }
  //Serial.println("end loop");
  Serial.flush();
    //Serial.write("on");
}
