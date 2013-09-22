This program should read from the 3 tables of the db. 
Make a JSON object then, HTTP Post. 

I made an example of this, pretty simple program. 
@blabno, please expand this program.

Console output:
1 , Monday
2 , Tuesday
3 , Wednesday

Sending 'POST' request to URL : http://requestb.in/o0cy4ko0
Response Code : 200
ok

This is what server received:

http://requestb.in
POST /o0cy4ko0  application/json
 117 bytes 4s ago  
From 190.96.21.152
FORM/POST PARAMETERS

None
HEADERS

Host: requestb.in
Accept-Language: en-US,en;q=0.5
User-Agent: Mozilla/5.0
Content-Length: 117
Accept: application/json
Content-Type: application/json
Connection: close
RAW BODY

[ {
  "name" : "Monday",
  "id" : 1
}, {
  "name" : "Tuesday",
  "id" : 2
}, {
  "name" : "Wednesday",
  "id" : 3
} ]