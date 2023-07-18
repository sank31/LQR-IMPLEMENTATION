
#define r_pin A4
#define y_pin A5

 
#define y_max 4
#define x_max 10

#define x_0 10
#define y_0 118

#define x_total 164
#define y_total 128

#define x_size 144
#define y_size 108

// include TFT and SPI libraries
#include <TFT.h>  
#include <SPI.h>


// pin definition for Arduino UNO
#define cs   10
#define dc   9
#define rst  8

int time_array[]={0,2,4,6,8,10};
float inp_1[100];
float inp_2[100];

float v1,v2;

bool v1_enabled = 1;
bool v2_enabled = 1;
long current_time = 0; //ms

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

void update_time_array()
{
  int last_num = time_array[5];
  //Serial.println("Time array:");
  for(int i = 0; i<6;i++){
    time_array[i] = last_num;
    last_num+=2;
    //Serial.print(String(time_array[i])+"\t");
  }
  //Serial.println("");

}
void setup() {
  Serial.begin(115200);
  pinMode(r_pin,INPUT);
  pinMode(y_pin,INPUT);
  
  //initialize the library
  TFTscreen.begin();
  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  //set the text size
  TFTscreen.setTextSize(2);
}
void draw_axis()
{
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.line(x_0,y_0,x_size+x_0,y_0);
  TFTscreen.line(x_0,y_0,x_0,y_total - y_size - y_0);
}
void draw_x_axis()
{
  TFTscreen.setTextSize(1);
  char cstr[5];
  char x_offset = -5;
  char y_offset = 1;
  itoa(time_array[0], cstr, 10);
  TFTscreen.text( cstr, x_0+x_offset, y_0+y_offset);
  
  itoa((int)(time_array[5] - time_array[0])/2 + time_array[0], cstr, 10);
  TFTscreen.text(cstr, x_0+(int)((float)x_size/2.0)+x_offset , y_0+y_offset);

  itoa(time_array[5], cstr, 10);
  TFTscreen.text(cstr, x_0+x_size+x_offset, y_0+y_offset);

  for (int i = 0; i < 10; i++){
    int x = x_0 + (i*x_size/10);
    TFTscreen.line(x,y_0-2,x,y_0+2);
  }
}
void draw_y_axis()
{
  TFTscreen.setTextSize(1);
  char cstr[5];
  char x_offset = -5;
  char y_offset = 1;
  
  itoa((int)y_max/2, cstr, 10);
  TFTscreen.text(cstr, x_0+x_offset , y_0-(int)(y_size/2) +y_offset);

  itoa((int)y_max, cstr, 10);
  TFTscreen.text(cstr, x_0+x_offset , y_0-(int)(y_size) +y_offset);

  for (int i = 0; i < 10; i++){
    int y = y_0 - (i*y_size/10);
    TFTscreen.line(x_0-2,y,x_0+2,y);
  }
}
void draw_circle()
{
  TFTscreen.circle(0,30,30);
}
void serial_adc()
{
  Serial.println("Input-1: "+String(v1)+"V");
  Serial.println("Input-2: "+String(v2)+"V");
  Serial.println(" ------------------------ ");
}
void draw_axes(){
  TFTscreen.background(0, 0, 0);
  draw_axis();
  draw_x_axis();
  draw_y_axis();
}
void measure_voltage(){
  v1 = (float)analogRead(r_pin)*5.0/1024;
  
  v2 = (float)analogRead(y_pin)*5.0/1024;


}
void draw_v1(){
  TFTscreen.stroke(0, 0, 255);
  
  int x = (current_time * x_size / ((float)(time_array[5]-time_array[0])*1000.0))+ x_0;
  
  int y = y_0 - (v1 * y_size/(float)y_max);
  
  //Serial.println("X: "+String(x)+"\t\tY: "+String(y));
  TFTscreen.point(x,y);
}
void draw_v2(){
  TFTscreen.stroke(0, 255, 0);
  
  int x = (current_time * x_size / ((float)(time_array[5]-time_array[0])*1000.0))+ x_0;
  
  int y = y_0 - (v2 * y_size/(float)y_max);
  
  //Serial.println("X: "+String(x)+"\t\tY: "+String(y));
  TFTscreen.point(x,y);
}
void loop() 
{
  draw_axes();

  while(1){
  measure_voltage();
  if(v1_enabled){
    draw_v1();
  }
  if(v2_enabled){
    draw_v2();
  }
  
  //serial_adc();

    current_time += 25;
    delay(25);
    if(current_time == 10000){
      current_time = 0;
      update_time_array();
      draw_axes();
      draw_x_axis();
    }
  }
  
}
