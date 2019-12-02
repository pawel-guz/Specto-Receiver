# Specto-Receiver
Source code for a DIY micro-board receiver for [Specto Music Visualizer](https://github.com/pawel-guz/Specto/blob/master/readme.md#specto-music--visualizer).
#### Note: This version hasn't been tested with the latest version of Specto.

## Board and circuit
This code is compatible with NodeMCU v1.0 boards. It should also work with other boards based on ESP8266 WiFI modules. 
Basic functionality - connecting with PC over SerialPort will work with every Arduino-like board. 

If you look for simple solution see: [simple circuit instructions](https://github.com/pawel-guz/Specto/blob/master/readme.md#example-circuit).

## To do:
```
- Implement buttons handlers (network config reset & device restart)
- Fix remote WiFi configuration (managed by Specto, in Device manager tab, 
  available upon connecting with this receiver in AP mode).
- Test WiFi communication with Specto.
```
