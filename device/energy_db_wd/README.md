First roll for the modbus->DB daemon.


Prerequisites:

install developement tools: sudo yum install gcc gcc-c++ gdb autoconf libtool postgresql-devel

Build libmodbus as static library:
Download sources from https://github.com/downloads/stephane/libmodbus/libmodbus-3.0.3.tar.gz
./configure --enable-static
make
copy libmodbus.a file
cp ./src/.libs/libmodbus.a <root of working dir>/device/energy_db_wd/lib/



Build:
gcc -I./lib/ -lpq -o "cenergy" ./src/energy_db_wd.c ./lib/libmodbus.a


Quick start for the database:

1. create a postgresql user - easier if you choose same username ar current linux username:
sudo su - postgres
createuser
input all prompted data then exit


2.create the database and the table:
createdb cenergy
psql cenergy

3 tables:

* Instant values: V1,V2,V3,Total KW, Total PF.
* Demand Values (15 minutes measured):  kw_import_block_demand bigint,
  kvar_import_block_demand bigint,
  kva_block_demand bigint.
* Energy Values:  kwh_import bigint,
  kwh_import_l1 bigint,
  kwh_import_l2 bigint,
  kwh_import_l3 bigint'


-------------copy from here-----------


DROP TABLE instant;
DROP TABLE demand;
DROP TABLE energy;

CREATE TABLE instant
(
  datetime timestamp DEFAULT now(),
  sent boolean DEFAULT 'false',
  v1_voltage bigint,
  v2_voltage bigint,
  v3_voltage bigint,
  total_kw integer,
  total_pf integer,
  PRIMARY KEY(datetime)
);

CREATE TABLE demand
(
  datetime timestamp DEFAULT now(),
  sent boolean DEFAULT 'false',
  kw_import_block_demand bigint,
  kvar_import_block_demand bigint,
  kva_block_demand bigint,
  PRIMARY KEY(datetime)
);

CREATE TABLE energy
(
  datetime timestamp DEFAULT now(),
  sent boolean DEFAULT 'false',
  kwh_import bigint,
  kwh_import_l1 bigint,
  kwh_import_l2 bigint,
  kwh_import_l3 bigint,
  PRIMARY KEY(datetime)
);

-------------copy to here-----------



Installing the service:
1. copy (or simlink) cenergy binary to /usr/local/bin
2. copy energy_db_wd.conf to /etc/init

