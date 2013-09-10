electrical
==========

Electrical M2M software to get an Electrical Bill approximation


1. Entity(s) related to Electrical Variables1
This JSON Objacted will be HTTP POSTed from ARM Device, will be made every 15`. 
For example at 08:00, 08:15, 08:30, 08:45, 9:00... 


{
 "\"TimeStamp\":\"%s\","
			"\"id\":%lu,"
			"\"voltage1\":%u,"
			"\"voltage2\":%u,"
			"\"voltage3\":%u,"
			"\"voltage13\":%u,"
			"\"voltage23\":%u,"
			"\"voltage31\":%u,"
			"\"current1\":%u,"
			"\"current2\":%u,"
			"\"current3\":%u,"
			"\"actPower1\":%u,"
			"\"actPower2\":%u,"
			"\"actPower3\":%u,"
			"\"reactPower1\":%u,"
			"\"reactPower2\":%u,"
			"\"reactPower3\":%u,"
			"\"currentN\":%u,"
			"\"frequency\":%u,"
			"\"appPower1\":%u,"
			"\"appPower2\":%u,"
			"\"appPower3\":%u"
			"}"
			
Example URL 
			
2. Entity(s) Related to energy.
This JSON Object will be HTTP POSTed, from ARM device every 5'. At 8:00, 8:05, 8:10...)
{
    "id: 
    "Energy": "100250",
    "TimeStamp": "Thu Sep  5 16:16:47 CLT 2013"
}

Example URL to POST http://10.10.10.10/client1/device1/energy


Perhaps it's better not to include the id on the JSON object, instead in the URL.

About the member, and device entities. There is a one to many relationship here, one member can have many devices. 
And the device entity  may have 2 Strings, like Name and Description.




Link to the docs:
https://docs.google.com/document/d/1LruXabD3js1uMHJbjtgtn_96fUXqv3CeVnnnc7lMAeA/edit?usp=sharing

