/* Here we just copy codes that works perfectely based on our touchscreen setup*/

#include "ArCOM.h"
ArCOM Serial1COM(Serial1); // UART serial port
ArCOM Serial3COM(Serial3); // UART serial port
ArCOM Serial4COM(Serial4); //UART serial port
ArCOM SerialCOM(Serial); // listen to the PC and send the ID

#define SPOUT_MOTOR_BOTTOM 1
#ifdef SPOUT_MOTOR_BOTTOM
#define LOW_DIR_S HIGH
#define HIGH_DIR_S LOW
#else
#define LOW_DIR_S LOW
#define HIGH_DIR_S HIGH
#endif



int i = 0;
int readByte1 = 0;
int readByte2 = 0;
char newTag1[16];
char newTag2[16];
char newTag3[16];
long int serialClocker = millis();
int serialResetTime = 10;
bool serial1read = false;
bool serial2read = false;
bool serial3read = false;
bool movingIntoChamber = true;
bool movingIntoCage = false;
bool door1open = false;
bool door1close = true;
bool door2open = false;
bool door2close = true;


// Motors varaibles
unsigned long Motor1Clocker = micros(); // timer to modulate speed of first motor
unsigned long Motor2Clocker = micros(); // timer to modulate speed of first motor
int stepPulse = 10; // duration of stepper pulse in microseconds
int Motor1Inc = 1000; // incremental motor1 motion to modulate speed
int Motor2Inc = 1000; // incremental motor1 motion to modulate speed


/* #################################################
  ############## PIN CONFIGURATION ###################
  #################################################### */

// Pins for stepper - spouts
#define PIN_GATE1_OPEN 7 
#define PIN_GATE1_CLOSE 8 
#define PIN_MOTOR1_DIR 13
#define PIN_MOTOR1_STEP 14

#define PIN_GATE2_OPEN 5
#define PIN_GATE2_CLOSE 6
#define PIN_MOTOR2_DIR 17
#define PIN_MOTOR2_STEP 18




void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600); //baud rate for UART bpod serial communication
  Serial3.begin(9600); //baud rate for UART bpod serial communication
  // Set servo pins to output mode
  pinMode(PIN_MOTOR2_DIR,OUTPUT);
  pinMode(PIN_MOTOR2_STEP,OUTPUT);
  pinMode(PIN_MOTOR1_DIR,OUTPUT);
  pinMode(PIN_MOTOR1_STEP,OUTPUT);

  // Set pin modes for input lines and stepper range  
  pinMode(PIN_GATE2_OPEN, INPUT_PULLUP);
  pinMode(PIN_GATE2_CLOSE, INPUT_PULLUP);
  pinMode(PIN_GATE1_OPEN, INPUT_PULLUP);
  pinMode(PIN_GATE1_CLOSE, INPUT_PULLUP);

}


/* ##########################################################
  ############## SERIAL EVENTS FUNCTIONS ####################
  ###########################################################*/
void serialEvent(){ // function to continusely listen to the chamber to receive bytes/ here we should receive byte about intochamber and intocage
// object detection input should be here too!!

  while(Serial.available()){
    readByteSerial = SerialCOM.readByte();
    if (readByteSerial == 1){
      movingIntoChamber = true;
    }
    if (readByteSerial == 2){
      movingIntoCage = true;
    }
  }


}



void serialEvent1() {
  serialClocker = millis();
  // Variable to hold each byte read from the serial buffer
  while(Serial1.available()){
    readByte1 = Serial1COM.readByte();    

    if (readByte1 != 2 && readByte1 != 13 && readByte1 != 10 && readByte1 != 3) {
      newTag1[i] = readByte1;
      i++;
    }
  }
  serial1read = true;
}

void serialEvent3() {
  serialClocker = millis();
  // Variable to hold each byte read from the serial buffer
  while(Serial3.available()){
    readByte2 = Serial3COM.readByte();    

    if (readByte2 != 2 && readByte2 != 13 && readByte2 != 10 && readByte2 != 3) {
      newTag2[i] = readByte2;
      i++;
    }
  }
  serial2read = true;
}

void serialEvent4() { // pin number 16
  serialClocker = millis();
  // Variable to hold each byte read from the serial buffer
  while(Serial4.available()){
    readByte3 = Serial4COM.readByte();    

    if (readByte3 != 2 && readByte3 != 13 && readByte3 != 10 && readByte3 != 3) {
      newTag3[i] = readByte3;
      i++;
    }
  }
  serial3read = true;
}



/////////////////////////////* Loop *///////////////////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:
  
  if (i > 0){
    if ((millis() - serialClocker) > serialResetTime) {  // done with stim trigger
        
        
        // reset serial array for next read
        i = 0;
        memset(newTag1, 0, sizeof(newTag1));  // initialize as 0s
        memset(newTag2, 0, sizeof(newTag2));  // initialize as 0s
        memset(newTag3, 0, sizeof(newTag3));  // initialize as 0s

        // decide what to do next
        if (serial1read){
            
            door1open = true; // door 1 should be opened
            serial1read = false;
            //Serial.println("Here");
        }
        else if (serial2read){
          
          if(movingIntoChamber){
            door1close = true;
            door2open = true;
            serial2read = false;
          }
          else if (movingIntoCage) { // combin back from the chamber to the cage
            door1open = true;
            door2close = true;
            serial2read = false;
          }
        }
        else if (serial3read) {
            if(movingIntoChamber){
            door2close = true;
            serial3read = false;
          }
          else if (movingIntoCage) { // combin back from the chamber to the cage
            door2open = true;
            serial3read = false;
          }
        }

        }
    }
  

  // check door pins to see if doors need to be moved to agree with desired state
  if ((micros() - Motor1Clocker) >= Motor1Inc){ // move left spout motor
        Motor1Clocker = micros();
  
  if (door1open){ // here desired state is: door1 = open pin
       
          digitalWriteFast(PIN_MOTOR1_DIR,LOW); // make sure stepper is moving in the right direction // CHECK if it is true
          delayMicroseconds(10); // short delay to ensure direction has changed
          sendStep(PIN_MOTOR1_STEP, stepPulse); // make a step 
          if (!digitalReadFast(PIN_GATE1_OPEN)){ // door is comletely open now
            door1open = false;
          }
          
          }

      // check opening/close pins and send motor step if we want to open
      // check the pint - are we open? if not send one step
      // send step in an if statement
  

   if (door1close){ // here desired state is: door1 = close pin
          digitalWriteFast(PIN_MOTOR1_DIR,HIGH); // make sure stepper is moving in the right direction // CHECK if it is true
          delayMicroseconds(10); // short delay to ensure direction has changed
          sendStep(PIN_MOTOR1_STEP, stepPulse); // make a step
          if (!digitalReadFast(PIN_GATE1_CLOSE)){ // door is comletely open now
            door1close = false;
          }
          }
      // check opening/close pins and send motor step if we want to close
  }
  
if ((micros() - Motor2Clocker) >= Motor2Inc){ // move left spout motor
        Motor2Clocker = micros();
  
if (door2open){ // here desired state is: door2 = open pin
          digitalWriteFast(PIN_MOTOR2_DIR,LOW); // make sure stepper is moving in the right direction // CHECK if it is true
          delayMicroseconds(10); // short delay to ensure direction has changed
          sendStep(PIN_MOTOR2_STEP, stepPulse); // make a step 
          if (!digitalReadFast(PIN_GATE2_OPEN)){ // door is comletely open now
            door2open = false;
          }
          }

 if (door2close){ // here desired state is: door2 = close pin
          digitalWriteFast(PIN_MOTOR2_DIR,HIGH); // make sure stepper is moving in the right direction // CHECK if it is true
          delayMicroseconds(10); // short delay to ensure direction has changed
          sendStep(PIN_MOTOR2_STEP, stepPulse); // make a step 
          if (!digitalReadFast(PIN_GATE2_CLOSE)){ // door is comletely open now
            door2close = false;

          }
          }
  }
}

/*#################################################
################### Additional Functions###########
###################################################*/
void sendStep(int cPin, int pulseTime) { // send stepper pulse as long as control signal is high
  digitalWriteFast(cPin, HIGH); // send step
  delayMicroseconds(pulseTime); // keep step signal high for pulseTime in microseconds. Should be at least 2 or longer.
  digitalWriteFast(cPin, LOW);
}


void sendID(byte header, char* ID){
  Serial.write(header);
  Serial.println(ID);
}

















