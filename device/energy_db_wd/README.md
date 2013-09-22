First roll for the modbus->DB daemon.


Prerequisites:

You need to have the libmodbus-devel and postgresql-devel packages installed.

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


Database name is energyMeterDB

energyMeterDB has one table as:

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

