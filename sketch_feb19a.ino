/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
* by Dejan Nedelkovski,
* www.HowToMechatronics.com
*/

// Motor control tutorial: https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/

////L293D
////Pins 3,5,6,9,10,11 provide 8 bit PWM output with analogWrite

//L293D 1
const int ShellUP  = 12;  // Pin 14 of L293
const int ShellDOWN = 13;  // Pin 10 of L293

//L293D 2
const int LF  = 10; // Pin  7 of L293
const int LB= 11;  // Pin  2 of L293

const int RF = 9;
const int RB = 3;

//radio
//pins A0-A5 are pins 14-19, input pins only (slower than regular pins?)
const int shellPin = 16;
const int fwdPin = 15;
const int turnPin = 14;


const int turniness = 255;
const int maxMotSpd = 255;

// defines variables
long duration;
int distance;


int stateU;
//
//SETUP 
//
void setup() {

// Set all the motor control pins to outputs
  pinMode(ShellUP, OUTPUT);
  pinMode(ShellDOWN, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(RF, OUTPUT);
  pinMode(RB, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(LF, LOW);
  digitalWrite(LB, LOW);
  digitalWrite(RF, LOW);
  digitalWrite(RB, LOW);

  //manual control code
  pinMode(shellPin,INPUT);
  pinMode(fwdPin,INPUT);
  pinMode(turnPin,INPUT);
  
  //Serial.begin(9600); // Starts the serial communication
}



//
// BASIC FUNCTIONS/ACTIONS
//
void stop(){
    //And this code will stop motors
    analogWrite(LF, 0);
    analogWrite(LB, 0);
    analogWrite(RF, 0);
    analogWrite(RB, 0);    
    analogWrite(ShellUP, 0);
    analogWrite(ShellDOWN, 0);

  }

void turnDir(int wheel,int spd,bool dir){
    if (wheel==0){
//      analogWrite(enA, spd);
      if(dir){
          analogWrite(LF, spd);
          analogWrite(LB, 0);
      } else{
          analogWrite(LF, 0);
          analogWrite(LB, spd);          
      }
    
    } else if(wheel==1){
//        analogWrite(enB, spd);
        if (dir){
        
          analogWrite(RF, spd);
          analogWrite(RB, 0);
        } else{
          analogWrite(RF, 0);
          analogWrite(RB, spd);
        }  
    } 
  }

//angle in degrees cuz easier
void move(float angle,float speed = 1.0,bool direction=true){
    float ls,rs;
    if (angle > 0){
      ls = maxMotSpd - angle * turniness;
      rs = maxMotSpd;
    }
    else if (angle <= 0){
      ls = maxMotSpd;
      rs = maxMotSpd + angle* turniness;
    }

    turnDir(1,int(ls*speed),direction);
    turnDir(0,int(rs*speed),direction);   
}


//TODO: Maybe fix the directions the motors spin for going up/down
bool shellState = false;
void shellDown(bool down){
//  analogWrite(enS, shellDownSpeed);
  if (shellState != down){
    if(!down){
      digitalWrite(ShellUP, LOW);
      digitalWrite(ShellDOWN, HIGH); 
    //delay(200);
      //digitalWrite(ShellUP, LOW);
      //digitalWrite(ShellDOWN, LOW);
    } else{
      digitalWrite(ShellUP, HIGH);
      digitalWrite(ShellDOWN, LOW);         
    }
    shellState = down; 
    
  }
         

}



int topZone = 1550; int bottomZone = 1350;
int minIn = 990; int maxIn = 1950;
float posToSignedByte(int in){
  //return in;
  //if we get no signal, do nothing...
  if (in == 0){return 0;}
  //if in deadzone, return 0 (assume no motion)
  if (in < topZone && in > bottomZone){return 0;}
  //if in lower half, return in [-255,0] proportional-ish to input
  float res = 0;
  if (in >= topZone) {
    res =  float(in-topZone)/float(maxIn-topZone);
  }
  //if in upper half, return in [0,255] proportional-ish to input
  if (in <= bottomZone) {
    res = float(in-bottomZone)/float(bottomZone-minIn);
  }
  return min(1.0,max(-1.0,res));
}

void loop() {
  delay(10);
  int shellIn = pulseIn(shellPin,HIGH,25000);
  //shellDown(true);
  if(shellIn > topZone){shellDown(false);}
  else if(shellIn < bottomZone){shellDown(true);}
  float fwdIn = posToSignedByte(pulseIn(fwdPin,HIGH,25000));
  float turnIn = posToSignedByte(pulseIn(turnPin,HIGH,25000));
  move(turnIn,abs(fwdIn), fwdIn > 0);
  
  
}
