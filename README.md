# cpp_RC_Car_PC_Remote_Control
Control a 1/8th scale buggy from the computer using a steering wheel and pedals. Uses Logitech's SDK for getting wheel and pedal data
Qt for the PC GUI and PC serial communication. All coded in c++ for PC side and C for the hardware side.
# Prerequisites
You need Qt, I am using Qt 5.10. I used a STM32L100CDISCO prototyping board for the hardware side, with systemworkbenchforstm32 as the IDE
and stm32cubemx to setup the skeleton code.
# Explanation
So I pretty much reversed engineered the remote control that came with my car (kyosho inferno ve racespec rtr), the steering and throttle are just potentiometers that act as voltage dividers, and the onboard microcontroller just reads the change in voltages and sends out the data accordingly. The reason I did not use say an IOT device on the car such as an ESP8266 is because
1. Didn't really want to muck around with wifi. A previous version of project actually used an ESP8226 and used UDP and was somewhat succesful however it had no flow control and pretty much flooded our network with these packets. I didn't really feel like setting up static IP's. The wifi can quite unreliable in our house for some reason as well
2. Because all the hardware was prototyping stuff and it had to be on the car, it was very cumbersome and I didn't really like how it sat on the car
3. The original reciever is purpose built to do just one thing and so it range is superior to wifi (even though technically they are both 2.4ghz) and there is zero chance of miscommunicating with the ESC and steering servo. Don't have worry about connection issues or IP addresses.
4.Because I am using the original reciever the car itself is actually unmodified and I know if I stay within the voltages I have measured I will never blow the microcontroller on the RC controller.
The STM32L100CDISCO has two 12 bit DACs which I used to replace the potentiometers. The mappings txt file contains the voltages I measured. I just got voltages when the pots were at their max and min and just used a simple line function to get voltages inbetween, using those values to calcuate gradient and offset.
