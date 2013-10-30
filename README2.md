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

Probably no 2 supermarkets would have the same configuration. @radu proposes to use an xml, so C program 
could read an xml for supermarket configurations. We should not focus in makin 1 size fits all, we could
have different C program in each supermarket, but we should keep them as similar as possible.
XML also allows to modify supermarket on the fly as well.

Example XML configuration:

```xml
<configs>
   <Name: SouthWalmart />
   <Numberofelectricmeters: 3/>
   <NumberofAnalogMeters: 2/>
      <ChannelsInstalled: 2/>
      .
      .
      .
</configs>
```

Electric Network
----------------

This will consist of 1 to 10 electrical meters. Meters can be of different models. 

If they are different models, we need to find a way to have the same data in DB tables. 

Each meter should have it's own set of tables. So if 1 meter has 3 DB tables, 3 meters will have 9. 

Sensors Network
---------------

To bring reality into digital world, 
we use 3 different types of *daqs* (data acquisition equipment), we use Advantech 
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
8 Input channels can take 10-50 VDC, 8 Output channels can deliver from 5 to 40 VDC. Digital Inputs might be refrigerator 
doors status (open/closed), so for example if a door is open for more than 1 hour, we might push a button on the web
interface to trigger for example a local horn/siren.

Temperature Readings
--------------------

Temperature probe to use (could vary) [AKS 12][5]

Example Table: TEMP1

<table>
  <tr>
    <th>ID</th><th>Temp1</th><th>Temp2</th><th>Temp3</th><th>Temp4</th><th>Temp5</th><th>Temp6</th><th>TIMESTAMP</th>
  </tr>
  <tr>
    <td>1</td><td>-4</td><td>-2</td><td></td><td></td><td></td><td></td><td>2013-30-10 14:59:30.252</td>
  </tr>
  <tr>
  <td>2</td><td>-1</td><td>-3</td><td></td><td></td><td></td><td></td><td>2013-30-10 15:05:30.252</td>
  </tr>
</table>

Lux Readings
------------

[Light Sensor][6]

Example  Table: LUX1

<table>
  <tr>
    <th>ID</th><th>Lux1</th><th>Lux2</th><th>Lux3</th><th>Lux4</th><th>Lux5</th><th>Lux6</th><th>Lux7</th><th>Lux8</th><th>TIMESTAMP</th>
  </tr>
  <tr>
    <td>1</td><td>300</td><td>500</td><td>
    </td><td></td><td></td><td></td><td>
    </td><td></td><td>2013-30-10 14:59:30.252</td>
  </tr>
  <tr>
    <td>1</td><td>350</td><td>440</td><td></td><td></td><td></td><td></td>
    <td></td><td></td><td>2013-30-10 15:05:30.252</td>
  </tr>
</table>

Open/Closed Readings
--------------------

Door open closed [switch][7] example.

This logic is different from others, because we cannot only read to the device, we can write as well.
Write to the daq means controlling something. We can have the *NUC* query a specific URL like:

* /supermarket1/control2/chann2/ every 5 minutes, with a boolean variable. 
* NUC should read that value and compare it to the actual value, if it's different, write to modbus register.
* NUC should read last stat and report to the server. server should keep track of the user that did this.







Server Side
===========

We need to expand what is done now, they basically need a good dashboard.

* Graphs, we need a good library that suits AngularJS. [Google Chart][8] seems simple to implement. 
We dont' need complex graphs, I like the look of simplistic things.
* Authentication: Must drop Basic and switch to our own authentication, since we use ssl, this should be easy.
* Control Logic, a URL should be available for each PC in each supermarket, to query if new actions are to be performed.
* Keep the Calendar at all times.
* Implement mail alarms. For example temperature reading, should be within tresholds if it escapes that range, then
alarm e-mail should be sent.

It is extremely important to keep in mind **ZERO UNNEEDED LINE OF CODE** although comments are welcome.








[1]: http://support.advantech.com.tw/Support/DownloadSRDetail_New.aspx?SR_ID=1%2bGE%2b715&Doc_Source=Download "Manual"
[2]: http://www.advantech.com/products/ADAM-4015/mod_3DF2523A-44C4-40E7-BFB6-B44B214DF8A8.aspx
[3]: http://www.advantech.com/products/ADAM-4017%2B/mod_10FD9E9C-8E8A-42F2-B749-A395F8426262.aspx
[4]: http://www.advantech.com/products/ADAM-4055/mod_FD19E628-4EA5-4C16-915C-5CEEEF2FB65C.aspx
[5]: http://www.ra.danfoss.com/TechnicalInfo/Literature/Manuals/01/RK0YG502_AKS.pdf
[6]: http://www.ums-muc.de/fileadmin/produkt_downloads/Klima/Lux-1_P.jpg
[7]: http://www.eurotruck-importers.com/images/0038205010.jpg
[8]: http://bouil.github.io/angular-google-chart/








