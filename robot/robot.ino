/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
* by Dejan Nedelkovski,
* www.HowToMechatronics.com
*/

// Motor control tutorial: https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/


// defines pins numbers for ultrasonic sensors
const int trigPin = 12;
const int echoPin = 13;
//L293D
// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;


const int turniness = 150;
const int maxMotSpd = 255;
const int spinSpeed = 50


// defines variables
long duration;
int distance;


void setup() {

  
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

// Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  

  Serial.begin(9600); // Starts the serial communication
}



void loop() {

}

void stop(){
    //And this code will stop motors
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);    

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

    analogWrite(enA, (int)ls );
    analogWrite(enB, (int)rs );

    
    if (direction){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    }else{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    }
    delay(2000); 
    
  }

  void spin(int direction){
    analogWrite(enA, spinSpeed );
    analogWrite(enB, spinSpeed );
    
    if (direction){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    }else{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    }
    delay(2000); 
  }


void getUltra(){
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
  return distance
  }
  
