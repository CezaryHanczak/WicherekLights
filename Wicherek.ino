#include <Adafruit_NeoPixel.h>

#define PI 3.1415926535897932384626433832795
#define input 13
#define strobo_off 60
#define nav_off 140

Adafruit_NeoPixel right_chassis = Adafruit_NeoPixel(8, 3, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel left_chassis = Adafruit_NeoPixel(8, 5, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel left_wing = Adafruit_NeoPixel(3, 6, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel right_wing = Adafruit_NeoPixel(3, 9, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel rear_and_stab = Adafruit_NeoPixel(4, 11, NEO_RGB + NEO_KHZ800);
//Adafruit_NeoPixel led_b = Adafruit_NeoPixel(1, 1, NEO_RGB + NEO_KHZ400);

void setup() 
{
  right_chassis.begin();
  left_chassis.begin();
  left_wing.begin();
  right_wing.begin();
  rear_and_stab.begin();

  off();
  pinMode(input, INPUT);
}

double _2pi = 2 * PI ;
double stab_i = 0;
unsigned long long int last_stab = 0;

double wing_state = 0;
unsigned long long int last_wing = 0;

int chassis_now = 0;
int chassis_dest = 0;
unsigned long long int last_chassis = 0;
 
int input_pwm;
unsigned long long int last_input = 0;

void loop() 
{
  if(millis() - last_input > 100)
    input_pwm = pulseIn(input,HIGH);

  
  if(input_pwm > 1490 && input_pwm < 1600)
  { 
      beacon();
      wing_nav_strobo();
      chassis_dest = 0;
      chassis();
  }
  else if (input_pwm > 1600 && input_pwm < 1700)
  {
      beacon();
      wing_nav_strobo();
      chassis_dest = 100;
      chassis();
  }
  else if (input_pwm > 1700 && input_pwm < 1800)
  {
      beacon();
      wing_nav_strobo();
      chassis_dest = 180;
      chassis();
  }
  else if (input_pwm > 1800 && input_pwm < 2100)
  {
      beacon();
      wing_nav_strobo();
      chassis_dest = 255;
      chassis();
  }
  else if (input_pwm > 900 && input_pwm < 1100)
  {   
      off();
  }
  else if (input_pwm > 1100 && input_pwm < 1200)
  {   
      rainbow();
  }
  else if (input_pwm > 1200 && input_pwm < 1300)
  {   
      random_colors();
  }
  else if (input_pwm > 1300 && input_pwm < 1400)
  {   
      wing_nav_strobo();
      police();
  }
  else if (input_pwm > 1400 && input_pwm < 1480)
  {   
      off();
      contour(255,0,0);
      contour(0,255,0);
      contour(0,0,255);
  }
  
  
}

void beacon(void)
{
  //BEACON------------------
  rear_and_stab.setPixelColor(2, rear_and_stab.Color(0,0,0));
  rear_and_stab.setPixelColor(3, rear_and_stab.Color(0,0,0));
  rear_and_stab.show();

  if(stab_i > _2pi)
    stab_i = 0;
    
  int led_right = (pow((sin(stab_i) + 1), 2) / 4) * 255;
  int led_left = (pow((sin(stab_i + PI) + 1),2) / 4) * 255;

  rear_and_stab.setPixelColor(2, rear_and_stab.Color(0,led_left,0));
  rear_and_stab.setPixelColor(3, rear_and_stab.Color(0,led_right,0));
  rear_and_stab.show();
  if(millis() - last_stab >= 5)
  {
    stab_i += 0.10;
    last_stab = millis();
  }
  //BEACON END ------------------
}

void wing_nav_strobo(void) 
{
  if(wing_state == -1)
  {
    right_wing.setPixelColor(0, right_wing.Color(nav_off,0,0));
    right_wing.setPixelColor(1, right_wing.Color(nav_off,0,0));
    left_wing.setPixelColor(0, left_wing.Color(0,nav_off,0));
    left_wing.setPixelColor(1, left_wing.Color(0,nav_off,0));
    
    right_wing.setPixelColor(2, right_wing.Color(strobo_off,strobo_off,strobo_off));
    left_wing.setPixelColor(2, left_wing.Color(strobo_off,strobo_off,strobo_off));
    
    rear_and_stab.setPixelColor(0, rear_and_stab.Color(strobo_off,strobo_off,strobo_off));
    rear_and_stab.setPixelColor(1, rear_and_stab.Color(strobo_off,strobo_off,strobo_off));
    right_wing.show();
    left_wing.show();
    rear_and_stab.show();
    last_wing = millis();
    wing_state = 0;
    return;
  }
  
  if(millis() - last_wing > 2500 && wing_state == 0)
  {
    right_wing.setPixelColor(0, right_wing.Color(255,0,0));
    right_wing.setPixelColor(1, right_wing.Color(255,0,0));
    left_wing.setPixelColor(0, left_wing.Color(0,255,0));
    left_wing.setPixelColor(1, left_wing.Color(0,255,0));
    
    right_wing.setPixelColor(2, right_wing.Color(255,255,255));
    left_wing.setPixelColor(2, left_wing.Color(255,255,255));
    
    rear_and_stab.setPixelColor(0, rear_and_stab.Color(255,255,255));
    rear_and_stab.setPixelColor(1, rear_and_stab.Color(255,255,255));
    rear_and_stab.show();
    right_wing.show();
    left_wing.show();
    last_wing = millis();
    wing_state = 1;
    return;
  }

  if(millis() - last_wing > 80 && wing_state == 1)
  {
    right_wing.setPixelColor(0, right_wing.Color(nav_off,0,0));
    right_wing.setPixelColor(1, right_wing.Color(nav_off,0,0));
    left_wing.setPixelColor(0, left_wing.Color(0,nav_off,0));
    left_wing.setPixelColor(1, left_wing.Color(0,nav_off,0));
    
    right_wing.setPixelColor(2, right_wing.Color(strobo_off,strobo_off,strobo_off));
    left_wing.setPixelColor(2, left_wing.Color(strobo_off,strobo_off,strobo_off));
    
    rear_and_stab.setPixelColor(0, rear_and_stab.Color(strobo_off,strobo_off,strobo_off));
    rear_and_stab.setPixelColor(1, rear_and_stab.Color(strobo_off,strobo_off,strobo_off));
    right_wing.show();
    left_wing.show();
    rear_and_stab.show();
    last_wing = millis();
    wing_state = 2;
    return;
  }

  if(millis() - last_wing > 80 && wing_state == 2)
  {
    right_wing.setPixelColor(0, right_wing.Color(255,0,0));
    right_wing.setPixelColor(1, right_wing.Color(255,0,0));
    left_wing.setPixelColor(0, left_wing.Color(0,255,0));
    left_wing.setPixelColor(1, left_wing.Color(0,255,0));
    
    right_wing.setPixelColor(2, right_wing.Color(255,255,255));
    left_wing.setPixelColor(2, left_wing.Color(255,255,255));
    
    rear_and_stab.setPixelColor(0, rear_and_stab.Color(255,255,255));
    rear_and_stab.setPixelColor(1, rear_and_stab.Color(255,255,255));
    right_wing.show();
    left_wing.show();
    rear_and_stab.show();
    last_wing = millis();
    wing_state = 3;
    return;
  }

  if(millis() - last_wing > 80 && wing_state == 3)
  {
    right_wing.setPixelColor(0, right_wing.Color(nav_off,0,0));
    right_wing.setPixelColor(1, right_wing.Color(nav_off,0,0));
    left_wing.setPixelColor(0, left_wing.Color(0,nav_off,0));
    left_wing.setPixelColor(1, left_wing.Color(0,nav_off,0));
    
    right_wing.setPixelColor(2, right_wing.Color(strobo_off,strobo_off,strobo_off));
    left_wing.setPixelColor(2, left_wing.Color(strobo_off,strobo_off,strobo_off));
    
    rear_and_stab.setPixelColor(0, rear_and_stab.Color(strobo_off,strobo_off,strobo_off));
    rear_and_stab.setPixelColor(1, rear_and_stab.Color(strobo_off,strobo_off,strobo_off));
    right_wing.show();
    left_wing.show();
    rear_and_stab.show();
    last_wing = millis();
    wing_state = 0;
    return;
  }
  
}

void chassis(void)
{
  if(chassis_now == chassis_dest)
    return;
    
  if(chassis_now > chassis_dest && millis() - last_chassis > 25)
  {
    chassis_now -= 1;
    last_chassis = millis();
    for(int i = 0; i < 8; i++)
    {
      right_chassis.setPixelColor(i, right_chassis.Color(chassis_now,chassis_now,chassis_now)); 
      left_chassis.setPixelColor(i, left_chassis.Color(chassis_now,chassis_now,chassis_now));
    }
    right_chassis.show();
    delay(5);
    left_chassis.show(); 
  }
  else if(chassis_now < chassis_dest && millis() - last_chassis > 25)
  {
    chassis_now += 1;
    last_chassis = millis();
    for(int i = 0; i < 8; i++)
    {
      right_chassis.setPixelColor(i, right_chassis.Color(chassis_now,chassis_now,chassis_now));
      left_chassis.setPixelColor(i, left_chassis.Color(chassis_now,chassis_now,chassis_now));    
    }
    right_chassis.show();
    delay(5);
    left_chassis.show();    
  }
}

void off(void)
{
  for(int i = 0; i < 8; i++)
  {
    right_chassis.setPixelColor(i, right_chassis.Color(0,0,0));
    left_chassis.setPixelColor(i, left_chassis.Color(0,0,0));
  }
  right_chassis.show();
  delay(4);
  left_chassis.show();

  for(int i = 0; i < 3; i++)
  {
    right_wing.setPixelColor(i, right_wing.Color(0,0,0));
    left_wing.setPixelColor(i, left_wing.Color(0,0,0));
  }
  right_wing.show();
  left_wing.show();

  for(int i = 0; i < 4; i++)
  {
    rear_and_stab.setPixelColor(i, rear_and_stab.Color(0,0,0));
  }
  rear_and_stab.show();
}


int rainbow_state = 0;
double rainbow_i = 0;
unsigned long long int last_rainbow = 0;

int green = 0;
int red = 255;
int blue = 0;

double Pi0_5 = 0.5 * PI;

void rainbow(void)
{
  if(rainbow_i >= Pi0_5)
    rainbow_state = 1;

  if(rainbow_i >= PI)
    rainbow_state = 2;

  if(rainbow_i >= PI + Pi0_5)
  {
    rainbow_i = 0;
    rainbow_state = 0;
  }
     
  if(rainbow_state == 0)
  {
      if(millis() - last_rainbow > 20)
      {
        all_set_color(green, red, blue);
        red = sin(rainbow_i + Pi0_5) * 255;
        green = sin(rainbow_i) * 255;
        blue = 0;
        rainbow_i += 0.01;
      }
  }
  else if(rainbow_state == 1)
  {
      if(millis() - last_rainbow > 20)
      {
        all_set_color(green, red, blue);    
        red = 0;
        green = sin(rainbow_i) * 255;
        blue = sin(rainbow_i - Pi0_5) * 255;
        rainbow_i += 0.01;
      }
  }
  else if(rainbow_state == 2)
  {
      if(millis() - last_rainbow > 20)
      {
        all_set_color(green, red, blue); 
        red = sin(rainbow_i - PI) * 255;
        green = 0;
        blue = sin(rainbow_i - Pi0_5) * 255;
        rainbow_i += 0.01;
      }
  }
}


void all_set_color(int green, int red, int blue)
{
  for(int i = 0; i < 8; i++)
  {
    right_chassis.setPixelColor(i, right_chassis.Color(green, red, blue));
    left_chassis.setPixelColor(i, left_chassis.Color(green, red, blue));
  }
  right_chassis.show();
  delay(5);
  left_chassis.show();
  delay(5);
  for(int i = 0; i < 3; i++)
  {
    right_wing.setPixelColor(i, right_wing.Color(green, red, blue));
    left_wing.setPixelColor(i, left_wing.Color(green, red, blue));
  }
  right_wing.show();
  left_wing.show();

  for(int i = 0; i < 4; i++)
  {
    rear_and_stab.setPixelColor(i, rear_and_stab.Color(green, red, blue));
  }
  rear_and_stab.show();
}

void police(void)
{
  rear_and_stab.setPixelColor(2, rear_and_stab.Color(strobo_off,strobo_off,strobo_off));
  rear_and_stab.setPixelColor(3, rear_and_stab.Color(strobo_off,strobo_off,strobo_off));
  rear_and_stab.show();
  
  for(int i = 0; i < 8; i++)
  {
    left_chassis.setPixelColor(i, left_chassis.Color(0,0,255));
    left_chassis.show();
    delay(10);
    left_chassis.setPixelColor(i, left_chassis.Color(0,0,0));
    left_chassis.show();
    delay(10);
  }
  for(int i = 7; i >= 0; i--)
  {
    left_chassis.setPixelColor(i, left_chassis.Color(0,0,255));
    left_chassis.show();
    delay(10);
    left_chassis.setPixelColor(i, left_chassis.Color(0,0,0));
    left_chassis.show();
    delay(10);
  }
  
  for(int i = 7; i >= 0; i--)
  {
    right_chassis.setPixelColor(i, right_chassis.Color(0,255,0));
    right_chassis.show();
    delay(10);
    right_chassis.setPixelColor(i, right_chassis.Color(0,0,0));
    right_chassis.show();
    delay(10);
  }
  for(int i = 0; i < 8; i++)
  {
    right_chassis.setPixelColor(i, right_chassis.Color(0,255,0));
    right_chassis.show();
    delay(10);
    right_chassis.setPixelColor(i, right_chassis.Color(0,0,0));
    right_chassis.show();
    delay(10);
  }
}

void contour(int green, int red, int blue)
{

  int k = 7;
  for(int i = 0; i < 8; i++, k--)
  {
    left_chassis.setPixelColor(i, left_chassis.Color(green, red, blue));
    left_chassis.show();
    right_chassis.setPixelColor(k, right_chassis.Color(green, red, blue));
    right_chassis.show();
    delay(80);
  }

  for(int i = 0; i < 3; i++)
  {
    right_wing.setPixelColor(i, right_wing.Color(green, red, blue));
    left_wing.setPixelColor(i, left_wing.Color(green, red, blue));
    right_wing.show();
    left_wing.show();
    delay(80);
  }

   rear_and_stab.setPixelColor(2, rear_and_stab.Color(green, red, blue));
   rear_and_stab.setPixelColor(3, rear_and_stab.Color(green, red, blue));
   rear_and_stab.show();
   delay(80);
   rear_and_stab.setPixelColor(0, rear_and_stab.Color(green, red, blue));
   rear_and_stab.setPixelColor(1, rear_and_stab.Color(green, red, blue));
   rear_and_stab.show();

    
  delay(200);
  k = 7;
  for(int i = 0; i < 8; i++, k--)
  {
    left_chassis.setPixelColor(i, left_chassis.Color(0,0,0));
    left_chassis.show();
    right_chassis.setPixelColor(k, right_chassis.Color(0,0,0));
    right_chassis.show();
    delay(80);
  }

  for(int i = 0; i < 3; i++)
  {
    right_wing.setPixelColor(i, right_wing.Color(0,0,0));
    left_wing.setPixelColor(i, left_wing.Color(0,0,0));
    right_wing.show();
    left_wing.show();
    delay(80);
  }

   rear_and_stab.setPixelColor(2, rear_and_stab.Color(0,0,0));
   rear_and_stab.setPixelColor(3, rear_and_stab.Color(0,0,0));
   rear_and_stab.show();
   delay(80);
   rear_and_stab.setPixelColor(0, rear_and_stab.Color(0,0,0));
   rear_and_stab.setPixelColor(1, rear_and_stab.Color(0,0,0));
   rear_and_stab.show();
   delay(100);
}

void random_colors(void)
{
  int red = random(255);
  int green = random(255 - green);
  int blue = random(255 - green - red);
  all_set_color(green, red, blue);

  if(random(0, 1000) > 900)
    contour(green, red, blue);
    
  delay(random(20, 400));
}
