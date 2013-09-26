electrical
==========

Intel NUC working as expected, CentOS 6.4 x86_64

3 Instances are up & running:

23.21.60.18 -> JBoss 
23.21.96.77 -> Httpd
107.21.233.48 -> Syslog-ng

Electrical M2M software to get an Electrical Bill approximation

EJB, CDI, Entities, JAX-RS, Arquillian. 

AngularJS in the front of the RESTFul API.

PostgreSQL on backend.

We will move to Java EE 7 eventually.

Link to the docs:
https://docs.google.com/document/d/1LruXabD3js1uMHJbjtgtn_96fUXqv3CeVnnnc7lMAeA/edit?usp=sharing

Read Variables from Meter
========================

Modbus RTU Frame Format. Not Modbus TCP.
32 bit values (2 Registers, form one float/double) ByteBuffer to be used here.
(Java uses two Complement... We might have an issue here?)

Cache Values
------------
 
1 Second Phase Values
Starting register: 13.952
* +0,1 V1 Voltage
* +2,3 V2 Voltage
* +4,5 V3 Voltage

1 Second Total Values
Starting register: 14.336
* +0,1 Total kW
* +6,7 Total PF

Persistent Values
-----------------

Power Demands 
Integrated in 15`
Starting Register: 14.592
* +12,13 kW Import Block Demand
* +14,15 kvar Import Block Demand
* +16,17 kVA block Demand

Total Energy
Starting Register 14.720
* +0,1 kWh import

Phase Energies
Starting Register 14848
* +0,1 kWh import L1
* +2,3 kWh import L2
* +4,5 kWh import L3

Angular JS Front End
====================
Instant Values

These might be refreshed every 2 minutes, not neccesary 2 store all of them in DB.
These reading are important, because customer can know if the power is "ON" on his industry.
These can be just one line:

V1 = 220 V,V2 = 220 V,V3 = 230 V
1 Second Total Power
1 Second Total PF



Have a calendar selector, that can be used to select range of days. default is only present day.
Need some kind of overlay or similar to show comments, in each variable.


Main Accordion
-------------
This resembles the electrical bill:
* Energy: KWh for the range, e.g. 670 KWh 
* Maximum Total Active Power for the Range:, e.g. 200 KW 
* Maximum Total Active Power in between 18:00 - 23:00
* Power Factor: This is Calculated at the maximum power divided by the "matching" apparent power.
P/S In the same time interval.

Total Detail Accordion
-------------
**AVG, MIN, MAX** for:
* Active/Reactive/Apparent total Power (3 variables)

Table Accordion
----------------
* Energy Phase 1 (Last sample - Initial Sample)
* Energy Phase 2
* Energy Phase 3

Drop Down select *one* menu:**AVG, MIN, MAX** 
Drop Down select *one* phase: Phase 1, Phase 2, Phase 3. 
3 Rows:
Voltage, Current, Apparent Power (multiplication of the past and divive by 1000 to get kVA).  
(Had to delete active and reactive PowerS because meter is not giving 15' Power Factor for each phase,
we can calculate this in the future so I would keep the select buttons.)
