#include <IRremote.h>

const int TRIGGER_PIN_LEFT = 12  ;// Arduino pin tied to trigger pin on the ultrasonic sensor.
const int ECHO_PIN_LEFT    = 11 ;
const int TRIGGER_PIN_RIGHT = 10 ; // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int ECHO_PIN_RIGHT = 2 ; // Arduino pin tied to echo pin on the ultrasonic sensor.
const int MAX_DISTANCE = 200; // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
const int proxPin = A0;
const int proxPowerPin = 13; //to power off when using ir
const int maxWallDistance = 25;
const int WALL_AHEAD = 0;
const int NO_WALL_AHEAD = 1;
const double cmsPerMS = 33.205;

// Distance threshold for intersection
const int distanceThresh = 20;

// IR codes
const int IR_LEFT = 0x162a;
const int IR_RIGHT= 0x662a;
const int IR_FORWARD = 0x562a;
const int IR_BACK = 0x362a;

// Turn Codes
const int CODE_LEFT = 10;
const int CODE_RIGHT = -10;
const int CODE_FORWARD = 20;
const int CODE_BACK = -20;



// Motor pin config
int pin_m_left_pos = 7;
int pin_m_left_neg = 8;
int pin_m_right_pos = 4;
int pin_m_right_neg = 5;
int pin_m_right_pwm = 6;
int pin_m_left_pwm = 9;


// IRremote config
int RECV_PIN = A1;
IRrecv irrecv(RECV_PIN);
decode_results results;


// Timer config
unsigned long long prev_time = 0;
int max_duration = 1000; // in ms

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(10);
    pinMode(proxPin, INPUT);
    pinMode(TRIGGER_PIN_LEFT, OUTPUT);
    pinMode(ECHO_PIN_LEFT, INPUT);
    pinMode(TRIGGER_PIN_RIGHT, OUTPUT);
    pinMode(ECHO_PIN_RIGHT, INPUT);
    analogWrite(pin_m_left_pwm, 150);
    analogWrite(pin_m_right_pwm, 150);
    pinMode(pin_m_left_pos, OUTPUT);
    pinMode(pin_m_left_neg, OUTPUT);
    pinMode(pin_m_right_pos, OUTPUT);
    pinMode(pin_m_right_neg, OUTPUT);
    irrecv.enableIRIn();
    pinMode(proxPowerPin, OUTPUT);
}


void loop()
{
	int proxEmpty = digitalRead(proxPin);
	bool isBetweenWalls = true;
	
	// Get distance from left wall
	digitalWrite(TRIGGER_PIN_LEFT, HIGH);
	delay(1);
	digitalWrite(TRIGGER_PIN_LEFT, LOW);
	double leftDistance = (pulseIn(ECHO_PIN_LEFT, HIGH, 50000))*cmsPerMS/2000;
	
	// Get distance from right wall
	digitalWrite(TRIGGER_PIN_RIGHT, HIGH);
	delay(1);
	digitalWrite(TRIGGER_PIN_RIGHT, LOW);
	double rightDistance = (pulseIn(ECHO_PIN_RIGHT, HIGH, 50000))*cmsPerMS/2000;

	Serial.print("right:");
	Serial.print(rightDistance);
	Serial.print(", left:");
	Serial.println(leftDistance);

	// forward();
	if(leftDistance > distanceThresh || rightDistance > distanceThresh){
		stop();
		if(getIRCode()){
			Serial.println(results.value, HEX);
			Serial.println(IR_LEFT, HEX);
			switch(results.value){
				case IR_LEFT:
					hardTurnLeft();
					forward();
					break;
				case IR_RIGHT:
					hardTurnRight();
					forward();
					break;
				case IR_FORWARD:
					forward();
					break;
				case IR_BACK:
					stop();
					break;
				default:
					break;
			}
		}
		
	}
	else if(leftDistance > rightDistance){
		softTurnLeft();
	}
	else {
		softTurnRight();
	}

}


void forward()
{
    digitalWrite(pin_m_left_pos, 1);
    digitalWrite(pin_m_left_neg, 0);
    digitalWrite(pin_m_right_pos, 1);
    digitalWrite(pin_m_right_neg, 0);
    analogWrite(pin_m_left_pwm, 150);
    analogWrite(pin_m_right_pwm, 150);
    delay(200);
}

void backward()
{
	digitalWrite(pin_m_left_pos, 0);
    digitalWrite(pin_m_left_neg, 1);
    digitalWrite(pin_m_right_pos, 0);
    digitalWrite(pin_m_right_neg, 1);
    analogWrite(pin_m_left_pwm, 150);
    analogWrite(pin_m_right_pwm, 150);
    delay(200);
}

void stop()
{
    digitalWrite(pin_m_left_pos, 0);
    digitalWrite(pin_m_left_neg, 0);
    digitalWrite(pin_m_right_pos, 0);
    digitalWrite(pin_m_right_neg, 0);
}

void hardTurnLeft()
{
	digitalWrite(pin_m_left_pos, 0);
    digitalWrite(pin_m_left_neg, 0);
    digitalWrite(pin_m_right_pos, 1);
    digitalWrite(pin_m_right_neg, 0);
    analogWrite(pin_m_left_pwm, 150);
    analogWrite(pin_m_right_pwm, 150);
    delay(800);
}
void hardTurnRight()
{
	digitalWrite(pin_m_left_pos, 1);
    digitalWrite(pin_m_left_neg, 0);
    digitalWrite(pin_m_right_pos, 0);
    digitalWrite(pin_m_right_neg, 0);
    analogWrite(pin_m_left_pwm, 150);
    analogWrite(pin_m_right_pwm, 150);
    delay(800);
}
void softTurnLeft()
{
	digitalWrite(pin_m_left_pos, 1);
    digitalWrite(pin_m_left_neg, 0);
    digitalWrite(pin_m_right_pos, 1);
    digitalWrite(pin_m_right_neg, 0);
    analogWrite(pin_m_left_pwm, 60);
    analogWrite(pin_m_right_pwm, 100);
    //soft delay
}
void softTurnRight()
{
	digitalWrite(pin_m_left_pos, 1);
    digitalWrite(pin_m_left_neg, 0);
    digitalWrite(pin_m_right_pos, 1);
    digitalWrite(pin_m_right_neg, 0);
    analogWrite(pin_m_left_pwm, 80);
    analogWrite(pin_m_right_pwm, 60);
    //soft delay
}
void turnBack()
{
	digitalWrite(pin_m_left_pos, 1);
    digitalWrite(pin_m_left_neg, 0);
    digitalWrite(pin_m_right_pos, 0);
    digitalWrite(pin_m_right_neg, 1);
    analogWrite(pin_m_left_pwm, 150);
    analogWrite(pin_m_right_pwm, 150);
    //hard delay
}

bool getIRCode()
{
	bool ifFoundIR = false;
	digitalWrite(proxPowerPin, LOW);
	delay(1);
	unsigned long long currTime = millis();
	while(millis() < currTime + 500)
	{
		ifFoundIR = irrecv.decode(&results);
		if(ifFoundIR)
		{
			break;
		}
	}
	irrecv.resume();
	digitalWrite(proxPowerPin, HIGH);
	return ifFoundIR;
}
