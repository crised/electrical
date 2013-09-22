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
gcc $(pkg-config --cflags --libs libmodbus libpq) -o "energy_db_wd" ./src/energy_db_wd.c
or if pkg-config fails
gcc -lpq -lmodbus -o "energy_db_wd" ./src/energy_db_wd.c


Quick start for the database:

1. create a postgresql user - easier if you choose same username ar current linux username:
sudo su - postgres
createuser
input all prompted data then exit


2.create the database and the table:
createdb energyMeterDB
psql energyMeterDB

3 tables:

* Instant values: V1,V2,V3,Total KW, Total PF.
* Demand Values (15 minutes measured):  kw_import_block_demand bigint,
  kvar_import_block_demand bigint,
  kva_block_demand bigint.
* Energy Values:  kwh_import bigint,
  kwh_import_l1 bigint,
  kwh_import_l2 bigint,
  kwh_import_l3 bigint'


CREATE TABLE energyreadings
(
  datetime timestamp DEFAULT now(),
  sent boolean,
  v1_voltage integer,
  v2_voltage integer,
  v3_voltage integer,
  total_kw integer,
  total_pf integer,
  kw_import_block_demand bigint,
  kvar_import_block_demand bigint,
  kva_block_demand bigint,
  kwh_import bigint,
  kwh_import_l1 bigint,
  kwh_import_l2 bigint,
  kwh_import_l3 bigint
);

