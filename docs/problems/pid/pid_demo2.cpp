// source: https://www.youtube.com/watch?v=LqQisRVhgPE

int settime = 1000;
float Kp=0.7, Ki=1, Kd=0.2;
float Pout, Iout, Dout;
float actPWD; // syngal wyjsciowy regulatora
float error, lastError;
float target_speed = 50; // target speed
float errorSum; // suma bledow dla czesci calkujacej
float Derror; // error rozniczkująca

const byte enkoderPin= 18;

unsigned int counter = 0;

void test (){
	digitalWrite(A_MOBOTPIN, HIGH);
	digitalWrite(A_MOBOTPIN, LOW);
}

void ISR_count(){
	counter++;
}

void ISR_timerone(){
	Timer1.detachInterrupt();
	current_speed = counter;
	counter = 0;
	Timer1.attachInterrupt(ISR_Timerone)
}

void setup{
// setup silniki i timery
}

void loop(){
    error = target_speed - current_speed;
    errorSum = errorSum + ((error+lastError)*0.5);
    Derror = (error - lastError);
    Pout = Kp * error;
    Iout = Ki * errorSum;
    Dout = Kd * Derror;
    if(Iout>255){ Iout = 255;}
    if(Iout<0){Iout = 0;}

    actPWM = Pout + Iout + Dout;

    if(actPWM > 255){ actPWM = 255;}
    if(actPWM < 0){ actPWM = 0;}

    lastError = error;
    return actPWM / 255.0;
}
