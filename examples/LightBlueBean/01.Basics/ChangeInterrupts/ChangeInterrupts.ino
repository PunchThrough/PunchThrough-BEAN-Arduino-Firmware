/* 
  This sketch uses the Bean library to trigger interrupts when changes are detected on pins 0, 1, and 2. 
  
  Notes:
    - This is not a low-power sketch 
    - A Bean with a low battery might show a faint blue and green LED color
  
  This example code is in the public domain.
*/

volatile uint8_t red = 0;
volatile uint8_t green = 0;
volatile uint8_t blue = 0;

// Interrupts are meant to be fast, avoid doing Bean.* operations directly within ISRs
//   Set a flag and perform the desired operation within the superloop
void zeroChangeDetected( void )
{
  red += 16;
}

void oneChangeDetected( void )
{
  green += 16;
}

void twoChangeDetected( void )
{
  blue += 16;
}

void setup() {
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  Bean.attachChangeInterrupt(0, zeroChangeDetected);
  Bean.attachChangeInterrupt(1, oneChangeDetected);
  Bean.attachChangeInterrupt(2, twoChangeDetected);
  Bean.setLed(0xFF, 0xFF, 0xFF);  // Flash to show the sketch is running
  Bean.sleep(250);
}

void loop() {
  // Set the Bean's LED based upon RGBs changed by interrupts
  //   Eventually, overflow will turn the colors off
  Bean.setLed(red, green, blue);
  Bean.sleep(100);
}