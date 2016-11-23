# Art Hack Day EMS System Manager

### ems_side_sketch
Arduino Sketch.
This Sketch provides to control relay switch by serial signal "1", 9600bps.
Also When the arduino caught the signal 1, 
the relay is turned on 10 mm sec and turned off after turning on the ems and relay circuit,
So you dont need turn off ems explicitlly.

### sensor_side_sketch
Arduino Sketch.
This sketch provides to manage multi sensor are tilt1, tilt2, lead sensor is asinged 2,8,3 respectively.
Tilt1 send "1", when the turn on tilt sensor.
Simmlarly, tilt2 is "11", lead sensor is "25".

### serialRead
openFrameworks Project. This project provide the function which manages the sensor signal and an ems system.

