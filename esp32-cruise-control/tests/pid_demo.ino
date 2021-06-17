//PID demo program
//by Dale Heatherington  Jan 10 2016
//Hardware board is Arduino Pro mini

//Define analog ports
const int setPointPot = 0;      //Pot that controls position
const int processValuePot = 1;  //Pot connected to motor
const int Ppot = 2;             //Pot controls Kp
const int Ipot = 3;             //Pot controls Ki
const int Dpot = 4;             //Pot controls Kd

const int enableMotor = 12;     // pin 12 output high enables PWM to motor

const int zSwitch = 2;          //Switch to enable process value pot is digital pin 2

int setPoint,processValue,P,I,D;
long  error, lastError, pid,t_now;
long d,i,p;
long Kp,Ki,Kd;

void setup() {
  Serial.begin(9600);
  pinMode(zSwitch,INPUT);
  pinMode(enableMotor,OUTPUT);
  digitalWrite(enableMotor,HIGH);
  
  //Setup timer 2 for 62500 Hz PWM.  Hardware wired for locked antiphase pwm.
  pinMode(11,OUTPUT);   //For OCR2A
  pinMode(3,OUTPUT);    //For OCR2B  (not used here but could be 2nd motor pwm)
  TCCR2B = 0x01;      // clk/1 prescale, 8 bit fast pwm 62.5 khz
  TCCR2A = 0xf3;      //Set OC2 A & B on compare match, fast PWM
  OCR2A = 128;        //Arduino pin 11 for fast PWM output.  Set for 50:50 duty cycle (zero speed)
  OCR2B = 128;        //Arduino pin 3  (not used)

  p = 0;
  i = 0;
  d = 0;

  t_now = millis() + 500;
}


void loop() {
 
//Read the  3 PID pid coefficient pots (value range is 0..1023 )
 P = analogRead(Ppot);
 I = analogRead(Ipot);
 D = analogRead(Dpot);

 //Scale the coefficients as needed (trial and error)
 Kp = P / 4;      // 0..255
 Ki = I / 4;     // 0..255
 Kd = D ;      // 0..1023
 
 setPoint = analogRead(setPointPot) - 512;    // get set point, range -512 to +511
 processValue = analogRead(processValuePot) - 512;  //get actual servo position, range -512 to +511
 
 if(digitalRead(zSwitch) == LOW) setPoint = -400;  //Disable set point pot when LOW and set -400
 
error = processValue - setPoint;    //Error is difference between process value and set point

p = error ;                     // Proportional is just the error

i = i + error;                  //Integral

//Optional: create a dead band so the so integrel won't hunt back and fourth
//if(abs(error) >  1) i = i + error;  // Integrate error if error > 1
//if(error == 0) i = 0;               //Clear intergal if zero error

i = constrain(i,-2000,2000);      //Prevent i from going to +/- infinity

d = error  - lastError ;        // error differential 
lastError = error;              // Save last error for next loop

pid = ((Kp * p)/4) + ((Ki * i)/512) + (Kd * d) ;  // Do PID 

pid = pid / 10;                          //Divide pid  for scaling  (  trial and error)
 
pid = constrain(pid,-127,127);            //Constrain to valid PWM range -127 to 127.  zero = stopped
OCR2A = pid + 128;                        //Set Locked antiphase PWM motor speed 1..255
 
 

 if(millis() >= t_now){                  //Print debug info twice per second 
 debug();
  t_now = millis() + 500;
 }
 
 delay(10);                           //Do loop 100 times per second  (10ms delay)

}

void debug()
{
  Serial.print(error);
  Serial.print("    ");
  Serial.print(Kp);
  Serial.print(" ");
  Serial.print(Ki);
  Serial.print(" ");
  Serial.print(Kd);
  Serial.print("     ");
  Serial.print(pid);
  Serial.print("     ");  
  Serial.print(p);
  Serial.print(" ");
  Serial.print(i);
  Serial.print(" ");
  Serial.print(d);
  Serial.println(" ");
  
  }

