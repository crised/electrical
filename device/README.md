Device
======

The sample code actually made a communication with meter. 

We have 2 devices here, meter and x86_64 Intel computer. Running CentOS 6.4

These were the compile instruction to compile:
gcc $(pkg-config --cflags --libs -lmodbus) -o "hello" hello.c

As we spoke, an expansion of this file will be a C daemon, that INSERTS data to postgresql.

@Radu, please create this daemon, values to be read are in the other README.md
Please include which database tables did you made.

Later on a Java daemon, will extract the values of the postgresql and HTTP POST to the server, with JSON formatting.