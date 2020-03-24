#include <Arduino.h>

#define Lpwm_pin  5     //adjusting speed 
#define Rpwm_pin  10    //adjusting speed

int pinLB=2;  // defining pin2 left rear
int pinLF=4;  // defining pin4 left front
int pinRB=7;  // defining pin7 right rear
int pinRF=8;  // defining pin8 right front

/* ABC
ST 000
LT 101
RT 011
FW 001
BK 111
 */

//A B C
//D5 D6 D7
int IN_A=11;//D5 Left 
int IN_B=9; //D6 Right
int IN_C=6; //D7 Control

//motor speed
unsigned char Lpwm_val =200;
unsigned char Rpwm_val = 200;

int State = 0;

//setup
void setup_pins(void)
{
  pinMode(pinLB,OUTPUT); // pin2
  pinMode(pinLF,OUTPUT); // pin4
  pinMode(pinRB,OUTPUT); // pin7 
  pinMode(pinRF,OUTPUT); // pin8
  pinMode(Lpwm_pin,OUTPUT); // pin11 (PWM) 
  pinMode(Rpwm_pin,OUTPUT); // pin10 (PWM)

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  pinMode(IN_A,INPUT_PULLUP);
  pinMode(IN_B,INPUT_PULLUP);
  pinMode(IN_C,INPUT_PULLUP);

}

void Set_Speed(unsigned char Left,unsigned char Right)
{
  analogWrite(Lpwm_pin,Left);
  analogWrite(Rpwm_pin,Right);
}

//control commands
void advance()     // going forward
{
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,HIGH); //motor moves right front
  digitalWrite(pinLB,LOW);
  digitalWrite(pinLF,HIGH); //motor moves left front
  
}
void turnR()        //turning right(dual wheel)
{
  digitalWrite(pinRB,HIGH);  //making motor move towards right rear
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,LOW);
  digitalWrite(pinLF,HIGH);  //making motor move towards left front
  
}
void turnL()        //turning left(dual wheel)
{
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,HIGH);   //making motor move towards right front
  digitalWrite(pinLB,HIGH);   //making motor move towards left rear
  digitalWrite(pinLF,LOW);  
}    
void stopp()         //stop
{
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,HIGH);
  
}
void back()          //back up
{
  digitalWrite(pinRB,HIGH);  //making motor move towards right rear
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,HIGH);  //making motor move towards left rear
  digitalWrite(pinLF,LOW);
}

//read pins, set state
void read_values()
{
  if (digitalRead(IN_C) == HIGH)
  {
    if (digitalRead(IN_B) == HIGH)
    {
      if (digitalRead(IN_A) == HIGH)
        State = 4; //Reverse 111
      else
        State = 2; //Right 011
    } else
    {
      if (digitalRead(IN_A) == HIGH)
        State = 1; //Left 101
      else
        State = 3; //Forward 001     
    }
  } else State = 0; //Stop 000
  
//  k_a = digitalRead(IN_A) ? 4 : 0;
//  k_b = digitalRead(IN_B) ? 2 : 0;
//  k_c = digitalRead(IN_C) ? 1 : 0;
//  State = k_a + k_b + k_c;
}


void control_car(void)
{
  if (!State) return;

  switch(State)
  {
  case 1:
    turnL();
    delay(20);//sync issues
    break;
  case 2:
    turnR();
    delay(20);//-''-
  default:
    break;
  }

  do
  {
    switch(State)
    {
    case 3:advance(); //UP 100
      break;
    case 4: back();   //back 111
    default: 
      break;      
    }
    //timeout of command
    int i = 5;
    if (i < 1)
    {
      i = 5;
      State = 0;
    } else i--;
    read_values();
  } while(State > 2);
  
  stopp();
}

void setup()
{
  setup_pins();
  Set_Speed(Lpwm_val,Rpwm_val);
  stopp();
//   Serial.begin(9600); //debug shit
//   Serial.println("Began print");
  read_values();
}
void loop() 
{  
  control_car();
  read_values(); // Receive the next value
//  Serial.println(State);
}
