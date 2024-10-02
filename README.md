# CTC2ESP
Esphome interface to collect data from the CTC Ecologic V3 unit and publish it to Home Assistant.

Based on info gatherd from Värmepumpsforum : https://www.varmepumpsforum.com/vpforum/index.php?topic=36385.0
And on CTC-EcoAir-log-and-control by laschrokken : https://gitlab.com/laschrokken/ctc-ecoair-log-and-control

The code is adjusted to work with CTC Ecologic V3 control unit which is conncected to CTC EcoAir 109 V3 air/water heatpump. Based on information from Värmepumps forum, after adjustments the code should work with any CTC EcoXXX unit hardware generation V3.

> [!WARNING]  
> This code was writen using "custom components" in Esphome, and those will be removed from ESPHome in a future releases as Esphome will move over to "external components". Maybe i will rewrite the custom components to external >components in some future...  maybe

# Hardware
You need:
1. ESP32 based board, as you need two harware UARTs. (ESP8266 got only one harware UART Rx) - *I used nodemcu ESP32 board.*
2. Two UART <-> RS232 adapters.- *I used ONE SparkFun Transceiver Breakout - MAX3232*
3. Ethernet cable to "slice up".

# Communication between relayboard and displayboard
The microcontroller with firmware is mounted on the displayboard(DP) and it is communicating with the microcontroller relayboard(RB) via standard ethernet(RJ45) cable using 2400bit/s RS232 8N1 protocol.

The wiring used is as follows(Parallell wires for each signal) :

1-2  +12Vdc <br/>
3-4   GND <br/>
5-6 RX (Data from DP to RB) <br/>
7-8 TX (Data from RB to DP) <br/>

# Wiring
PinOut for NodeMCU ESP32 and SparkFun Transceiver Breakout - MAX3232
![alt text](https://github.com/mercix/CTC2ESP/blob/main/PinOut.png?raw=true)



**Data from RB to DP consist of 22 hex bytes per block and looks as follows:** <br/>
Example data string: 02:27:29:80:00:26:80:80:94:94:5E:80:00:00:08:28:2D:00:00:01:00:78 <br/>

The bytes represent as follows: <br/>
*Not avalible on EcoLogic* <br/>

1 Always 0x02. <br/>
2 Temperature before heat pump.
3 Outdoor temperature from EcoLogic sensor (integer) [subtract 40 (decimal) from the value to get  ºC] <br/>
4 and 5 indoor temperature, integer and tenths of ºC respectively. <br/>
6 Incoming water temperature to the heat pump (integer) <br/>
*7 Lower tank temperature (integer)* <br/>
*8 Upper tank temperature (integer)* <br/>
9 and 10 always 0x94. <br/>
11 Heat gas temperature (integer) <br/>
12 Always 0x80. <br/>
13 Always 0x00. <br/>
*14 Power A (integer)* <br/>
15 always 0x08. <br/>
16 Outdoor temperature from the heat pump's sensor.(integer) [subtract 40 (decimal) from the value to get ºC.] <br/>
17 Temperature after the heat pump. <br/>
18 and 19 always 0x00. <br/>
20 Second counter, starts at 0x00 at switch-on and counts up to 0x3B (hex) = 59 (dec) and then starts over. <br/>
21 Always 00, occasionally disappears when the second counter starts over.<br/>
22 Unknown function.<br/>

**Data from DP to RB consist of 10 hex bytes per block and looks as follows:** <br/>
Example data string: FA:90:44:90:44:00:00:00:80:7A <br/>

1 Always 0xFA <br/>
2 Bitmaped data with information of active relays in EcoLogic and Ecoair no.1 <br/>
3 Bitmaped data with information of active relays in EcoLogic and Ecoair no.2 <br/>
4 Same as byte 2<br/>
5 Same as byte 3 <br/>
6 and 7 and 8 Always 0x00 <br/>
9 Always 0x80 (with exception for boot 0x00) <br/>
10 Always 0x7A (with exception for boot 0xFA) <br/>


**Bitmap of Byte 2&4 [DP to RB]** <br/>
The bitmap is as follows: <br/>

*Not tested* <br/>

0 Compressor <br/>
1 Fan Low <br/>
*2 Unkonwn* <br/>
3 Fan High <br/>
4 Unkonwn <br/>
*5 Three way valve* <br/>
6 Unknown <br/>
*7 Shunt closed* <br/>

**Bitmap of Byte 3&5 [DP to RB]** <br/>
The bitmap is as follows: <br/>

*Not tested* <br/>

0 Alarm LED <br/>
1 Circulation pump <br/>
2 Unknown <br/>
3 Unkonwn <br/>
4 Unkonwn <br/>
5 Supplementary heating INVERTED <br/>
*6 Shunt closening* <br/>
*7 Shunt opening* <br/>








