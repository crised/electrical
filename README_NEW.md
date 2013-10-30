Supermarket
===========

We're extending electrical project into supermarket project.

Supermarkets need not only to monitor 1 electrical meter, typically they wan't at least 4.

Not only they want to monitor electricity, also: Temperature in Refrigerators, Illumination,
and some Digital (boolean) I/Os like check if a refrigeration door is open/close. 

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
we use 3 different types of data acquisition equipment, we use Advantech ADAM 4000 series.
A supermarket should have one of each kind, but it's not fixed. Each meter should have it's own DB table. 

* [ADAM-4015: ](http://www.advantech.com/products/ADAM-4015/mod_3DF2523A-44C4-40E7-BFB6-B44B214DF8A8.aspx) 
This device has up to 6 channels of temperature, means it can take up to 6 different temperature probes. I suggest having
one column for each channel, if only 1 probe is installed, values of others should be null.

* [ADAM-4017+: ](http://www.advantech.com/products/ADAM-4017%2B/mod_10FD9E9C-8E8A-42F2-B749-A395F8426262.aspx)

