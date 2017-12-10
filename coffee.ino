int machineStatePin = 3;
int machineSwitchPin = 6;
int shortCoffeeOutput = 4;
int longCoffeeOutput =  5;
int timeOn = 0;
int countOff =0;
int previousMachineState = 0;
int longCoffeeSignal = 1003;
int shortCoffeeSignal = 1001;
boolean shortCoffee = false;
boolean longCoffee = false;
boolean warming = false;
boolean machineIsReady = false;
// the setup routine runs once when you press reset:

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Starting...");
  // make the pushbutton's pin an input:
  pinMode(machineStatePin, INPUT);
  pinMode(longCoffeeOutput, OUTPUT);
   pinMode(shortCoffeeOutput, OUTPUT);
  pinMode(machineSwitchPin, OUTPUT);

  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

// the loop routine runs over and over again forever:
void loop() {
  if (mySwitch.available()) {
    int command = mySwitch.getReceivedValue();
    Serial.print("Received ");
    Serial.print( command );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );
    
    if(command == shortCoffeeSignal){
      shortCoffee = true;
      longCoffee = false;
    }else if(command == longCoffeeSignal){
      longCoffee = true;
      shortCoffee = false;
    }
    mySwitch.resetAvailable();
  }

  //Si la cafetiere n'est pas en chauffe et si la machine n'est pas prete et qu'un des deux signal à été recu on alume la cafetiere
  if(!warming && !machineIsReady && (shortCoffee || longCoffee)){
    digitalWrite(machineSwitchPin, HIGH);
    delay(500);
    digitalWrite(machineSwitchPin, LOW);
    warming = true;
    countOff = 0;
  }

    if(countOff > 100){
    Serial.println("System is off...");
    warming = false;
    machineIsReady = false;
    shortCoffee = false;
    longCoffee = false;
  }

  
  // read the input pin:
  int buttonState = digitalRead(machineStatePin);
  
  if (buttonState == 1) {
    countOff = 0;
    if (previousMachineState == 0) {
      timeOn = millis();
      Serial.println("Time on begin...");
      Serial.println(timeOn);
    }

    int now = millis();
    if ((now - timeOn) > 5000) {
      machineIsReady = true;
    }else{
      machineIsReady = false;
    }

    if(machineIsReady){
      warming = false;
      Serial.println("Machine is ready...");
      if (shortCoffee) {
        Serial.println("Short Coffee done!");
        digitalWrite(shortCoffeeOutput, HIGH);
        delay(500);
        digitalWrite(shortCoffeeOutput, LOW);
        shortCoffee = false;
        delay(2000);
      } else if (longCoffee) {
        Serial.println("Long Coffee done!");
        digitalWrite(longCoffeeOutput, HIGH);
        delay(500);
        digitalWrite(longCoffeeOutput, LOW);
        longCoffee = false;
         delay(2000);
      }
    }
  } else {
    countOff++;
    timeOn = 0;
    machineIsReady = 0;
  }

  previousMachineState = buttonState;
  delay(200);        // delay in between reads for stability

}
