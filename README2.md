Supermarket
===========

We're extending electrical project into supermarket project.

Supermarkets need not only to monitor 1 electrical meter, typically they wan't at least 4.

Not only they want to monitor electricity, also: Temperature in Refrigerators, Illumination,
and some Digital (boolean) I/Os like check if a refrigeration door is open/close.

Our goal: Energy monitoring/controling **HIGH QUALITY** software. 

Supermarket Side
================

Inside the supermarket, 2 RS-485/Modbus networks are proposed. 

2 RS-485 to 1 USB physical port, leads to two: /dev/ttyUSB0 and /dev/ttyUSB1

Electric Network
----------------

This will consist of 1 to 10 electrical meters. Meters can be of different models. 

If they are different models, we need to find a way to have the same data in DB tables. 

Each meter should have it's own set of tables. So if 1 meter has 3 DB tables, 3 meters will have 9. 

Sensors Network
---------------

To bring reality into digital world, 
we use 3 different types of data acquisition equipment, we use Advantech 
[ADAM 4000][1]
series. A supermarket should have one of each kind, but it's not fixed. Each meter should have it's own DB table. 

* [ADAM-4015: ][2] 
This device has up to 6 channels of temperature, means it can take up to 6 different temperature probes. I suggest having
one column for each channel, if only 1 probe is installed, values of others should be null. Temperature should be in 
Celsius degrees, with 16-bit resolution.

* [ADAM-4017+: ][3]
Analog to digital converter has up to 8 analog channels that measure either voltage or current, with 16-bit resolution. 
These a/d converter will be used to measure illumination. Supermarkets internal illumination range from 0 to 1000 lux.
Luxmeter can output both 0 to 100 mv or 4 to 20 ma. It's better to pick only one for the long term.

* [ADAM-4055: ][4]
This module provide our only controlling capability since it has outputs. Digital Input / Output module. 
8 Input channels can take 10-50 VDC, 8 Output channels can deliver from 5 to 40 VDC. Digital Inputs might be refrigator 
doors status (open/closed), so for example if a door is open for more than 1 hour, we might push a button on the web
interface to trigger for example a local horn/siren.

Server Side
===========

We need to expand what is done now, they basically need a dashboard with graphs. 

It is extremely important to keep in mind **ZERO UNNEEDED LINE OF CODE** although comments are welcome.








[1]: http://support.advantech.com.tw/Support/DownloadSRDetail_New.aspx?SR_ID=1%2bGE%2b715&Doc_Source=Download "Manual"
[2]: http://www.advantech.com/products/ADAM-4015/mod_3DF2523A-44C4-40E7-BFB6-B44B214DF8A8.aspx
[3]: http://www.advantech.com/products/ADAM-4017%2B/mod_10FD9E9C-8E8A-42F2-B749-A395F8426262.aspx
[4]: http://www.advantech.com/products/ADAM-4055/mod_FD19E628-4EA5-4C16-915C-5CEEEF2FB65C.aspx









