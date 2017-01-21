## Selfmade cheap telemetry solution for my quadrocopter

A project done by me ~3 years ago for learning c++ and arduino stuff.
Used this to transmit cell voltages from an arduino on my quadrocopter to an groundstation arduino with gaphical display, drawing a nice rolling average graph =)
Cheap 433MHz transmitter and receiver were used, because ive had nothníng else at this time.
Added just to archive this.

Software requirements:
* u8glib (https://github.com/olikraus/u8glib)
* VirtualWire library (https://www.pjrc.com/teensy/td_libs_VirtualWire.html)

Hardware:
* Arduino (mini Pro)
* 433 MHz Transmitter  (https://www.aliexpress.com/item/1set-RF-module-433-Mhz-superheterodyne-receiver-and-transmitter-Support-ASK-OOK-small-size-low-power/32571703475.html)
* ST7920 controller based 128X64 pixel GLCD Display
* Some resistors for voltage divider
