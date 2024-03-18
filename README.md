# CTC2ESP
Esphome interface to collect data from the CTC Ecologic V3 unit and publish it to Home Assistant.

Based on info gatherd from Värmepumpsforum : https://www.varmepumpsforum.com/vpforum/index.php?topic=36385.0
And on CTC-EcoAir-log-and-control by laschrokken : https://gitlab.com/laschrokken/ctc-ecoair-log-and-control

The code is adjusted to work with CTC Ecologic V3 control unit which is conncected to CTC EcoAir 109 V3 air/water heatpump. Based on information from Värmepumps forum, after adjustments the code should work with any CTC EcoXXX unit hardware generation V3.

> [!WARNING]  
> This code was writen using "custom components" in Esphome, and those will be removed from ESPHome in a future releases as Esphome will move over to "external components". Maybe i will rewrite the custom components to external >components in some future...  maybe

# Hardware
You need:
1. ESP32 based board, as you need two harware UARTs. (ESP8266 got only one harware UART Rx) - I used nodemcu ESP32 board.
2. Two pieces of UART <-> RS232 adapters.
3. Ethernet cable to "slice up".

# Communication between relayboard and displayboard
The microcontroller with firmware is mounted on the displayboard(DP) and it is communicating with the microcontroller relayboard(RB) via standard ethernet(RJ45) cable using 2400bit/s RS232 8N1 protocol.

The wiring used is as follows(Parallell wires for each signal) :

1-2  +12Vdc
3-4   GND
5-6 RX (Data from DP to RB)
7-8 TX (Data from RB to DP)



**Data from RB to DP consist of 22 hex bytes per block and looks as follows:**
02:27:29:80:00:26:80:80:94:94:5E:80:00:00:08:28:2D:00:00:01:00:78

The bytes represent as follows: 
#= Not avalible on EcoLogic

1 Always 0x02.
2 Temperature before heat pump.
3 Outdoor temperature from EcoLogic sensor (integer) [subtract 40 (decimal) from the value to get  ºC]
4 and 5 indoor temperature, integer and tenths of ºC respectively.
6 Incoming water temperature to the heat pump (integer)
#7 Lower tank temperature (integer)
#8 Upper tank temperature (integer)
9 and 10 always 0x94.
11 Heat gas temperature (integer)
12 Always 0x80.
13 Always 0x00.
#14 Power A (integer)
15 always 0x08.
16 Outdoor temperature from the heat pump's sensor.(integer) [subtract 40 (decimal) from the value to get ºC.]
17 Temperature after the heat pump.
18 and 19 always 0x00.
20 Second counter, starts at 0x00 at switch-on and counts up to 0x3B (hex) = 59 (dec) and then starts over.
21 Always 00, occasionally disappears when the second counter starts over.
22 Unknown function.

**Data from DP to RB consist of 10 hex bytes per block and looks as follows:**







