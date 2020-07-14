# gspseven

## wiring

This library supports one display per instance of the gspSeven class.

Cascading devices on the MAX2719 driver _DOES_NOT_WORK_ consistently with the software SPI driver.

Consequently a different wiring strategy needs to be used.
Fortunately this does not require a massive number of additional pins.

All displays can share pins for:

* VCC
* GND
* CLK
* DataIn

Each display needs its own:
 
* CS/LOAD pin

Don't try and write to multiple displays at the same time, so only 
activate one CS/LOAD Pin at any one time.
