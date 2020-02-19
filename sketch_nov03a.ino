//L293D 1
const int ShellUP  = 12;  // Pin 14 of L293
const int ShellDOWN = 13;  // Pin 10 of L293

//L293D 2
const int LF  = 10; // Pin  7 of L293
const int LB= 11;  // Pin  2 of L293

const int RF = 9;
const int RB = 3;

//IRs
const int IRL = 6;
const int IRR = 5;

//US
const int trigPin = 7;
const int echoPin = 8;

//This will run only one time.
void setup(){
 
    //Set pins as outputs
    pinMode(ShellUP, OUTPUT);
    pinMode(ShellDOWN, OUTPUT);
    pinMode(LF, OUTPUT);
    pinMode(LB, OUTPUT);
    pinMode(RF, OUTPUT);
    pinMode(RB, OUTPUT);

    pinMode(IRL, INPUT); //Left, right IR sensors
    pinMode(IRR, INPUT);

    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input

    Serial.begin(9600); //serial disables pins 0,1 i think

    delay(10000);
  
}
int duration; int distance;

void loop(){

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
    if (digitalRead(IRL) == LOW || (distance >= 0 && distance < 15)) {
      digitalWrite(LF,LOW); digitalWrite(LB,LOW); 
    } else {
      digitalWrite(LF,HIGH); digitalWrite(LB,LOW);
    }

    if (digitalRead(IRR) == LOW || (distance >= 0 && distance < 15)) {
      digitalWrite(RF,LOW); digitalWrite(RB,LOW); 
    } else {
      digitalWrite(RF,HIGH); digitalWrite(RB,LOW);
    }

}
