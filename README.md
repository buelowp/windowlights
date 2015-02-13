<h1>Painting Windows with NeoPixels</h1>
<h2>What is it?</h2>
<p>The idea here is to use NeoPixel strips to be able to provide holiday color to my front windows. To that end, I hacked up a PCB, bought a lot of NeoPixel strips, and glued in a Teensy 3.1 and GPS to help me keep track of time. This is what I'm using for the code.</p>
<h2>Setup</h2>
<p>You need the teensyduino from PJRC to build this. I'm using Teensy because it's a real 32 bit environment, runs 4 times as fast, and provides me the memory space to run a bunch of different light schemes without having to reprogram the unit all the time. You could probably also do this with a Mega, but you may run into issues as it takes the Mega a lot longer to do some of the math.</p>
<p>Download the code here and create a new project in the IDE. You only need to build the WindowLights.ino, but all the other .cpp and .h files need to be local. Once it's built, you can upload it. From there, it should just work as advertised. When Christmas rolls around, you'll see Christmas lights. When Halloween rolls around, you'll see Halloween lights. And so on.</p>
<h2>What do you need to do this?</h2>
<ul>
<li>I created a PCB using ExpressPCB. They are nice because it's low cost, high quality boards, the SW is clunky, but easy to learn and not Eagle, and they ship three, so when you accidentally hose the first one for a bad solder, you can just hack together the second one. <a href="http://www.expresspcb.com">ExpressPCB</a>
<li>The Teensy 3.1 is a great little M4 chip from Freescale. It has enough memory to do lots of complicated stuff, but the biggest bang for your buck with the Teensy is that it can do complicated 32 bit math much faster than an Arduino which can only do 16 bit math without having to do a lot of assembly gymnastics to handle larger numbers. <a href="www.pjrc.com">PJRC</a>
<li>NeoPixel strips from Adafruit are a superfast way to control LEDs withough spending a ton of money. <a href="https://www.adafruit.com/category/168">Adafruit NeoPixels</a>
<li>A GPS module. The idea for using the GPS module is that it's a constant source of perfect time. No need to set the clock if you have an RTC, no extra battery to worry about, and it always knows where it is. This is nice, because calculating sunrise and sunset depends on where you are on the globe. <a href="https://www.adafruit.com/products/746">Adafruit GPS</a>
</ul>

<h2>What does it do</h2>
So, basically, we get our location and time from GPS. Using that, we can calculate when the sun will rise and set for the day. With the sunrise and sunset data, we can turn the program for a specific holiday on and off at reasonable times. Say 30 minutes before sunset until midnight. It's a no touch no thought solution. The next version will include a push button or two to allow for turning the lights on and off at any time of the day, and to cycle through the light show or possible turn on a generic light show for non holiday days. Don't know yet.
