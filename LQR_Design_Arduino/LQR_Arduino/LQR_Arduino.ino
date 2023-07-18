


#define r_pin A1   // ADC1
#define y_pin A0   // ADC0
#define u_pin 3    // PWM Pin

#define r_min 0.1  // min value of refrence
#define r_max 3.0  // max value of refrence

#define loop_refresh_rate 100.0     //loop_refresh_rate is in Hz. Ts = 1/loop_refresh_rate.
#define Ts (int)((1.0/loop_refresh_rate)*1000000.0)     //we need this time in micro seconds as we are using microseconds 
                                                        //function for main loop  ((1/loop_refresh_rate)*1000000) micro Seconds.
                                                        //time between 2 sucssecive samplings, in our case Ts = 10ms.

unsigned long loop_time;

//float Gain = 6.0;  // we will devide un by Gain to make it in range of (0-5V), so it may be given out at Arduino Pins.
float Gain = 6.7;  // 6.7 afetr practical implimentation

float rn = 1;    // default value is unit step.
float yn = 0;
float en_1 = 0;
float en = 0;
float an_1 = 0;
float an = 0;
float x2n = 0;
float un = 0;

float ki = -14.14213562;
float k2 = 11.592593279;

void setup() 
{
  pinMode(u_pin , OUTPUT);
  pinMode(y_pin ,INPUT);
  pinMode(r_pin ,INPUT);

  Serial.begin(57600);       //wait untill Serial is intialized.
  while(!Serial)        
  {
  }
  loop_time=micros()+Ts;            //start time measuring.
}

void loop() 
{
  //////////////////////////////////////////////// Control Equations Starts Here ////////////////////////////////////////////////
  float r = analogRead(r_pin)*5.0/1023.0;
  if (r >= r_min & r <= r_max)  // if ref value is in limit, then update, else not.
    rn = r;   
     
  yn = analogRead(y_pin)*5.0/1024.0;
  
  Serial.print("ref  :");
  Serial.print(rn);
  Serial.print("       y  :");
  Serial.println(yn);
  

  en = rn-yn;
  x2n= yn;
  an = ki * (0.005*en + 0.005*en_1) + an_1;
  un = -(k2*x2n + an);
  un /= Gain;

  an_1 = an;      // to be used in next an computations.
  en_1 = en;      // to be used in next en computations.

  analogWrite(u_pin, (un/5.0)*255);  // 8 Bit PWM on Mega/Uno/Nano
 
 //////////////////////////////////////////////// Control Equations Ends Here ////////////////////////////////////////////////
  

  ////////////////////////////////////////DON'T CHANGE IT//////////////////////////////////////
  //Loop time taken.
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if(loop_time<micros())
  {
    while(1)      //wait forever here.,
    {
      Serial.print("Cant be Refreshed at given  ");
      Serial.print(loop_refresh_rate);
      Serial.println("  Refreshed Rate ,Please decrease Refresh Rate.");
      delay(500);
    }
  }
  //To make sure every loop is exactly Ts milliseconds @ loop_refresh_rate Hz. a wait loop
  //is created by setting the loop_time variable to +Ts microseconds every loop.

  while(loop_time > micros());
  loop_time += Ts;  
  ////////////////////////////////////////////////////////////////////////////// 

}
