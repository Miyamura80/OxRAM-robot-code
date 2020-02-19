/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
* by Dejan Nedelkovski,
* www.HowToMechatronics.com
*/

// Motor control tutorial: https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/


// defines pins numbers for ultrasonic sensors
const int trigPin = 7;
const int echoPin = 8;



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


const int turniness = 150;
const int maxMotSpd = 255;
const int spinSpeed = 50;
const int collideThreshold = 50;
const int shellDownSpeed = 50;
const int retreatTimeMs = 800;
const int retreatTimeMs2 = 400;

// defines variables
long duration;
int distance;


int stateU;
//
//SETUP 
//
void setup() {
  
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

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
  
  stateU = 0;
  Serial.begin(9600); // Starts the serial communication
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
void move(int angle,bool direction=true){
    float ls,rs;
    if (angle > 0){
      ls = maxMotSpd - angle * turniness;
      rs = maxMotSpd;
    }
    else if (angle <= 0){
      ls = maxMotSpd;
      rs = maxMotSpd - angle* turniness;
    }

    turnDir(0,(int) ls,direction);
    turnDir(1,(int) rs,direction);
 
    
}

void spin(bool direction=true){
  turnDir(0,spinSpeed,direction);
  turnDir(1,spinSpeed,direction);
}



//TODO: Maybe fix the directions the motors spin for going up/down
void shellDown(bool down){
//  analogWrite(enS, shellDownSpeed);
  if(down){
      digitalWrite(ShellUP, HIGH);
      digitalWrite(ShellDOWN, LOW);
  } else{
      digitalWrite(ShellUP, LOW);
      digitalWrite(ShellDOWN, HIGH);          
  }
  delay(1000);
  digitalWrite(ShellUP, LOW);
  digitalWrite(ShellDOWN, LOW);          

}


float getUltra(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
  }

//TODO: Remove later if redundant
bool isColliding(){
  float dist = getUltra();
  if(dist < collideThreshold){
    return true;
  } else{
    return false;
  }

}




//STATES:
//0: idle
//1: init
//2: yoink
//3: spin and search
//4: charge
//5: shell defence


//STATE 0
int idle(){
  delay(1000);
  return 5;
}

//STATE 1
//Just wait 10 seconds after initialization?
int initSeq(){
  delay(1000*10);
  return 2;
}

//STATE 2
int yoink(){
  move(0,false);
  delay(retreatTimeMs);
  move(90);
  delay(300);
  move(0,false);
  delay(retreatTimeMs2);
  stop();
  return 5;
}

//STATE 3
int spinSearch(){
  int checkFreq = 20;
  int foundThreshold = 90;


  spin();
  bool found = false;
  for (int i=0;i<checkFreq;i++){
    delay( (int)(1000/checkFreq));
    if (getUltra()<foundThreshold){
      found = true;
      break;
    }

  } 
  stop();
  if (found){
    return 4;
  }else{
    return 5;
  }

}

//STATE 4
int charge(){
  move(0);
  delay(1000);
  stop();
  return 5;
}

//STATE 5
int shellDefence(){
  shellDown(true);
  delay(1000*7);
  shellDown(false);
  return 3;

}

//STATE CONTROLLER
int stateController(int state){
 switch(state){
    case 0:
      state = idle();
    case 1:
      state = initSeq();
    case 2:
      state = yoink();
    case 3: 
      state = spinSearch();
    case 4:
      state = charge();
    case 5:
      state = shellDefence();
 } 
 return state;

}


void loop() {
  stateU = stateController(stateU);
}
