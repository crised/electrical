Device java module
===

This is client module installed on meter devices.
It reads data from 3 tables (energy,demand,instant) and sends them to the server.
After sending to the server rows are marked as "sent".

This module depends on "server-api" module in order to generate REST API proxy (using RestEasy client library).

Security
---

AuthFilter is attached to REST API proxy with hardcoded username and password which are sent in Basic Auth style.

[rnicolau] Please check and edit below:
To install the service copy the jenergy.conf file into /etc/init folder, along with the startup.sh script and the generated jar file

Logging
---

Java module logs to syslog using logback's SyslogAppender.
Make sure to configure syslog-ng to allow UDP:

```
source src { system(); internal(); udp(ip(127.0.0.1)); };
```