Device java module
===

This is client module installed on meter devices.
It reads data from 3 tables (energy,demand,instant) and sends them to the server.
After sending to the server rows are marked as "sent".

This module depends on "server-api" module in order to generate REST API proxy (using RestEasy client library).

Security
---

AuthFilter is attached to REST API proxy with hardcoded username and password which are sent in Basic Auth style.