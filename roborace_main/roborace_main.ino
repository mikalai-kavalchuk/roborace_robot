#include <NewPing.h>
 
#define PING_WAIT_TIME 35 
#define FRONT_WHEEL_SPEED 225 
 
#define SONAR1_TRIGGER_PIN  13
#define SONAR1_ECHO_PIN     12
#define SONAR2_TRIGGER_PIN  5
#define SONAR2_ECHO_PIN     4
#define SONAR3_TRIGGER_PIN  8
#define SONAR3_ECHO_PIN     7

#define MAX_DISTANCE 300
#define MIN_LENGTH 0

#define INT1 11
#define INT2 10
#define INT3  6
#define INT4  9

enum
{
    STOP,
    FORWARD,
    REVERSE,
    LEFT,
    RIGHT,
    FORWARD_LEFT,
    FORWARD_RIGHT,
    BACK_LEFT,
    BACK_RIGHT
};

int direction = STOP;
int speed = 120;
int s1,s2,s3 = 10;
int wall_counter = 0;

NewPing sonar1(SONAR1_TRIGGER_PIN, SONAR1_ECHO_PIN, MAX_DISTANCE);
NewPing sonar2(SONAR2_TRIGGER_PIN, SONAR2_ECHO_PIN, MAX_DISTANCE);
NewPing sonar3(SONAR3_TRIGGER_PIN, SONAR3_ECHO_PIN, MAX_DISTANCE);

void reversion(void)
{
  analogWrite(INT1, 0);
  analogWrite(INT2, speed);

  analogWrite(INT3, 0);
  analogWrite(INT4, 0);
}

void forward(void)
{
  analogWrite(INT1, speed);
  analogWrite(INT2, 0);

  analogWrite(INT3, 0);
  analogWrite(INT4, 0);
}

void left(void)
{
  analogWrite(INT1, 0);
  analogWrite(INT2, 0);

  analogWrite(INT3, FRONT_WHEEL_SPEED);
  analogWrite(INT4, 0);
}

void right(void)
{
  analogWrite(INT1, 0);
  analogWrite(INT2, 0);

  analogWrite(INT3, 0);
  analogWrite(INT4, FRONT_WHEEL_SPEED);
}

void stop(void)
{
  analogWrite(INT1, 0);
  analogWrite(INT2, 0);

  analogWrite(INT3, 0);
  analogWrite(INT4, 0);
}

void forward_left(void)
{
  analogWrite(INT1, speed);
  analogWrite(INT2, 0);

  analogWrite(INT3, FRONT_WHEEL_SPEED);
  analogWrite(INT4, 0);
}

void forward_right(void)
{
  analogWrite(INT1, speed);
  analogWrite(INT2, 0);

  analogWrite(INT3, 0);
  analogWrite(INT4, FRONT_WHEEL_SPEED);
}

void reversion_left(void)
{
  analogWrite(INT1, 0);
  analogWrite(INT2, speed);

  analogWrite(INT3, FRONT_WHEEL_SPEED);
  analogWrite(INT4, 0);
}

void reversion_right(void)
{
  analogWrite(INT1, 0);
  analogWrite(INT2, speed);

  analogWrite(INT3, 0);
  analogWrite(INT4, FRONT_WHEEL_SPEED);
}


void update_motor(int state)
{ 
  switch(state)
  {
    case FORWARD: forward(); break;
    case REVERSE: reversion(); break;
    case LEFT: left(); break;
    case RIGHT: right(); break;
    case STOP: stop(); break;  
    case FORWARD_LEFT: forward_left(); break;
    case FORWARD_RIGHT: forward_right(); break;
    case BACK_LEFT: reversion_left(); break;
    case BACK_RIGHT: reversion_right(); break;  
  }
}
 
int get_diff(int value1, int value2)
{
  if(value1>value2) return value1-value2;
  else return value2 -value1;  
}

void reverse_littlebit()
{
    if(s1<s3)direction = BACK_LEFT;
    else direction = BACK_RIGHT;
    speed = 200;
    update_motor(REVERSE);
    delay(700);
    speed = 200;
    update_motor(direction);
    delay(100);
}


void setup() {
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  
  delay(PING_WAIT_TIME);
  int tmp = sonar1.ping_cm();
  if(tmp > MIN_LENGTH) s1 = tmp;
  
  delay(PING_WAIT_TIME);
  tmp = sonar2.ping_cm();
  if(tmp > MIN_LENGTH) s2 = tmp;

  delay(PING_WAIT_TIME);
  tmp = sonar3.ping_cm();
  if(tmp > MIN_LENGTH) s3 = tmp;

  
  int diff = get_diff(s1, s3);   
  if(diff > 8)
  {
    if(s1<s3)direction = FORWARD_RIGHT;
    else direction = FORWARD_LEFT;
  }
  else direction = FORWARD;
  
  if(s2 < 70) speed = 90; 
  else if(s2 < 200) speed = 150;
  else speed = 255;

  if(diff>30) speed = 95;

  if((s2 < 20)||(s1 < 5)||(s3 < 5)) 
  {
    wall_counter++;
    if(wall_counter > 3)
    {
      wall_counter= 0;
      reverse_littlebit();
    }
  }
  else
  {
    wall_counter = 0;
  }
  
  update_motor(direction);
}
