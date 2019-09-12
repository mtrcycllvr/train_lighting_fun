/*

Train Car Lighting via WS2812 based "neo pixel" like strips..
Use a couple of variable resistors to control color and brightness.
Optionally plug in LED to EOT device..

https://oshpark.com/shared_projects/cVBEIdNU 
...if you want a matching circuit board supporting Teensy 3.2 and 3.4.

Basic code is relatively simple thanks to extensive WS2812B RGB LED library support..
Read the analog values, map them to LED value ranges, push data.  Blink the
EOT device in a realistic manner..

Verision 1.3-ish..

INFO:
I purchased a USA trains "Extended Vision Caboose",
http://www.usatrains.com/r12130.html
....and decided to update and improve the lighting a little bit.  To do that, I
scavenged unsused short bits of WS2812B based RGB light strips and a "Teensy"
micro controller that you would call a super charged arduino..
https://www.pjrc.com/store/teensy40.html

I glued six each of the inegrated LED/controler (the WS2812B thing I keep referring to)
to the top of the cabboose roof separate with a small bit of wire, you only need three,
power, ground and data..

This particular model also has a built in EOT, or End Of Train device, so I am 
"poorly" using a bunch of teensy horsepower to discretely fade the EOT device on and off
so it has more of an incandescent look to it..   I rescued a vintage red glass faceted 
indicator lens to replace the stock plastic part..  Putting that together with a higher
output 5MM LED and simulated warm up and cool down really creates an AWESOME look..

The underside manufacture's three way power switch gives you the coice of just the EOT
or the EOT and interior lights.  What I WAS going to do was use SW1 to change modes, but
now I'm lazy and lucky together and I can use SW1 on half of the power switch to detect which
mode it should run in at power on..  Easy peasy, and it means I'm done  programming, which
is nice.   Wanna see more effects?  Go crazy..  I have other unfinished projects to 
attend to..  ;-)

I made a little circuit board to mount and connect the Teensy to the variable resistors,
the LED's and a power input..
https://oshpark.com/shared_projects/cVBEIdNU


Eric Timberlake - 9.10.2019

todo: Make the EOT on/off functions re-entrant so they don't interfere with reading the
variable resistors in real time..  There MUST be a nice functional event timer library 
out there someplace, right? ;-)

*/

#include <WS2812Serial.h>                   // optimized library to control WS2812B LED's for Teensy based hardware.
#define USE_WS2812SERIAL                    // flag for FastLED to use the optimized library.
#include <FastLED.h>                        // FastLED lets us leverage optmized HSV commands, ie: driving the colors 
                                            // is a lot easier than just using WS2812Serial on it's own..

const int numled = 6;                       // how many WS2812B chips have you installed?
                                            // change this value to drive more lights, perhaps extended to other cars??
CRGB interiorLED[numled];                   // an array to manipulate WS2812B LED data later..  (color, brightness...)
byte drawingMemory[numled*3];               //  3 bytes per LED
DMAMEM byte displayMemory[numled*12];       // 12 bytes per LED

const int WSpin = 1;                        // the "spi-like" data out to the first WS2812..
                                            // this is restricted to certain pins not documented here!

// mode control via SW1 
const int SWpin = 2;                        // Switch 1 input hardware pin, D2..  Active by pulling Low.
boolean SW1 = false;                        // a variable to store the state of the switch in when we check it..

// WS2812 RGB LED color and brightness control variables..
int COLORpot = 0;                           // a way to store current color potentiometer value
const int COLORpin = 0;                     // use pot connected to A0 for COLOR settings..
int BRIGHTpot = 0;                          // a way to store current brightness potentiometer value
const int BRIGHTpin = 1;                    // use pot connected to A1 for brightness settings..

// EOT LED control variables..  Alter to suit your hardware and timing preferences..
int EOTperiod = 900;                        // flip the EOT LED back and forth at this interval!!
unsigned long currentEOTMillis = millis();  // A way to track time INTERVALS to set EOT blink rate
unsigned long startEOTMillis = millis();    // how to measure our time interval start point..
int EOTled = 6;                             // the PWM pin the EOT device LED is attached to
int EOTstate = 0;                           // track the on/off state of the EOT LED.

//... Do stuff in this function, ONCE, at power up.  Initilize libraries, set HW pins to read or write, etc..

void setup() {                              // one time configuration steps
  LEDS.addLeds<WS2812SERIAL,WSpin,RGB>(interiorLED,numled);
  pinMode(EOTled, OUTPUT);                  // configure the EOTdevice digital pin for output to drive buffer xsistor
  pinMode(SWpin, INPUT_PULLUP);             // configure D2 as a way to watch for active low on Switch 1
  digitalWrite(SWpin, HIGH);                // enable internal pull-up resistor for low true switch circuit.  (conntect to ground)
}


void loop() {                                                       // start void loop here...

// ...do stuff inside this ever repeating loop...

// start section to control WS2812 RGB LED color and brightness or in HSV, the 'value' portion of Hue Saturation Value tuple..
SW1 = digitalRead(SWpin);                                           // check the SW1 state, if active (low! Don't do this part!)
if ( SW1 == true){                                                  // see INTRO, for details of my specific caboose...
  BRIGHTpot = analogRead(BRIGHTpin);                                // take a read off of analog pin for value
  COLORpot = analogRead(COLORpin);                                  // take a read off of analog pin for value
  LEDS.setBrightness(map(BRIGHTpot, 0, 1023, 0, 255));              // set the brightness globally
  for (int i=0; i < numled; i++){                                   // itterate through WS2812 LED array data
      interiorLED[i] = CHSV(map(COLORpot,0,1023,0,255), 255, 255);  // set the color individually (Global brightness seems to overide V here)
      FastLED.show();                                               // push the data out to the LEDS!!
  }                                                                 // close set WS2812 RGB LED loop
}                                                                   // close "if we want lights AND EOT" or "Just EOT" keeping with OEM switch label
else {                                                              // if the switch is NOT active low (false in this logic!!)...
  FastLED.showColor(CHSV(0, 0, 0)); 
  LEDS.setBrightness(0);                                            // set the WS2812B LED's to off, effectively..
  FastLED.show();                                                   // push the now "off" data out to LED's...
}


// start section to control the EOT LED..  Which is ALWAYS ON, no choice, have a nice day..  ;-)
  currentEOTMillis = millis();                              // this variable minus the start time equates to the current INTERVAL
  if(currentEOTMillis - startEOTMillis > EOTperiod) {       // if the INTERVAL is greater than EOTperiod, flip the EOT state
    startEOTMillis = currentEOTMillis;                      // reset previous millis to continue waiting for next interval to pass..
    if(EOTstate == 0) {                                     // if the LED is off...
      EOTon();                                              // ...turn it on, "analog-like"!
      EOTstate = !EOTstate;                                 // flip the state so we turn it off next time!
    }
    else {                                                  // if the LED is already on, well then..
      EOToff();
      EOTstate = !EOTstate;
    }                                                       // close state sheck and flip
  }                                                         // close EOT on/off check

}                                                           // close void loop (go back and do it again, and agian, and...)


/*
Next come sub routines that we can call from INSIDE the 'void loop()' construct..
For instance, controlling the EOT device is a little clearer this way..  In void loop(), we check 
every few miliseconds if the EOT device should be reversed.  Then call sub routines below 
to actually perform that action..

Just how fast does an incandescent bulb light up?  It's NOT instantaneous (or near) like LED's are
narturally, so let's slow down the turning on process till it's visible to the human eye, but still "quick"..
*/

void EOTon(void){                 // bring EOT LED up slowly to simulate incadescent device warming up...
  for (int n=0; n < 255; n++){
    analogWrite(EOTled, n);
    delayMicroseconds(450);
  }
}

void EOToff(void){                // bring EOT LED down a bit slower to simulate incadescent device fade..
  for (int n=255; n > 0; n--){
    analogWrite(EOTled, n);
    delayMicroseconds(800);
  }
}