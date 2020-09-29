# arduino-cruise-control
Open Source Cruise Control module for cars that don't have CC and have cable driven throttle body

# Components
## Hardware
### Longan Labs - OBD-II CAN Bus GPS Development Kit
This development kit allows you to access your vehicle’s CAN bus via the OBD-II connector. The development kit can be connected (plugged) to your vehicle’s OBD-II port (On-Board Diagnostics port). 

 * Store: https://www.longan-labs.cc/1030003.html
 * Docs: https://docs.longan-labs.cc/1030003/

![OBD-II CAN Bus GPS Development Kit](docs/images/longan-obd/Longan-Labs-OBD-Dev-Kit.jpg)

### Cytron - MDD3A
The Cytron MDD3A motor controller can control 2 DC motors or 1 stepper motor. The continuous current per level is 3 A (5 A high). The supply voltage is from 4 V to 16 V. The controller works with a logical voltage of 1.8 V, 3.3 V, 5 V and 12 V (PWM and DIR), thanks to which it is possible to control using a microcontroller such as Arduino or Raspberry.

 * Store: https://www.cytron.io/p-3amp-4v-16v-dc-motor-driver-2-channels
 
![Cytron MDD3A](docs/images/cytron/Cytron-MDD3A.jpg)

### Honda Cruise Control Actuator Motor - Denso MX100300-1170
Denso MX100300-1170 throttle cable control motor from a Honda Accord 1.8 from 1998. Used to operate throttle body with steel cable. 
![Honda Denso Cruise Control Actuator](docs/images/denso/top.jpg)
![Honda Denso Cruise Control Actuator](docs/images/denso/side.jpg)