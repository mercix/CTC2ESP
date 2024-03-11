# CTC2ESP
Esphome interface to collect data from the CTC Ecologic V3 unit and publish it to Home Assistant

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

****To be continued





