## Train Car Lighting

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
