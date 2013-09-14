electrical
==========

** Update: Modbus meter is going to be changed for Commercial Reasons.**

Electrical M2M software to get an Electrical Bill approximation


2 Projects for now, running different servers:

1. JBoss App Server, that eats the data.
2. Mock of the ARM Device, that POSTS electrical variable.


EJB, CDI, Entities, JAX-RS, Arquillian. 

AngularJS in the front of the RESTFul API.

PostgreSQL on backend.

We will move to Java EE 7 eventually.


Link to the docs:
https://docs.google.com/document/d/1LruXabD3js1uMHJbjtgtn_96fUXqv3CeVnnnc7lMAeA/edit?usp=sharing

Angular JS Front End:

Instant Voltages
-----------------
These might be refreshed every 2 minutes, not neccesary 2 store all of them in DB.
These reading are important, because customer can know if the power is "ON" on his industry.
These can be just one line:

V1 = 220 V,V2 = 220 V,V3 = 230 V

Have a calendar selector, that can be used to select range of days. default is only present day.
Need some kind of overlay or similar to show comments, in each variable.


Main Accordion
-------------
This resembles the electrical bill:
* Energy: KWh for the range, e.g. 670 KWh 
* Maximum Active Power for the Range:, e.g. 200 KW 
* Maximum Active Power in between 18:00 - 23:00
* Power Factor

Total Detail Accordion
-------------
**AVG, MIN, MAX** for:
* Active/Reactive/Apparent total Power (3 variables)

Table Accordion
----------------
Drop Down select *one* menu:**AVG, MIN, MAX** 
Drop Down select *one* phase: Phase 1, Phase 2, Phase 3. 

10 Rows:
Voltage, Current, Active Power, Reactive Power, Apparent Power, Power Factor, Frequency. 
