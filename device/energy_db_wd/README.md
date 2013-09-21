First roll for the modbus->DB daemon.

Build:

gcc $(pkg-config --cflags --libs libmodbus libpq) -o "energy_db_wd" ./src/energy_db_wd.c
or if pkg-config fails
gcc -lpq -lmodbus -o "energy_db_wd" ./src/energy_db_wd.c

You need to have the libmodbus-devel and postgresql-devel packages installed.

Database has one table as:

CREATE TABLE energyreadings
(
  datetime varchar(40),
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

database name is energyMeterDB
