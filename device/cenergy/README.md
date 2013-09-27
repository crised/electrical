

Prerequisites:

install developement tools: sudo yum install gcc gcc-c++ gdb autoconf libtool postgresql-devel

Build libmodbus as static library:
Download sources from https://github.com/downloads/stephane/libmodbus/libmodbus-3.0.3.tar.gz
./configure --enable-static
make
copy libmodbus.a file
cp ./src/.libs/libmodbus.a <root of working dir>/device/energy_db_wd/lib/



Build:
gcc -I./lib/ -lpq -o "cenergy" ./src/cenergy.c ./lib/libmodbus.a


Quick start for the database:

1. create a postgresql user:
sudo su - postgres
createuser electrical -P
input all prompted data then exit (use password "electrical")

if identification fails in the connect stage the you might need to edit 
/var/lib/pgsql/data/pg_hba.conf file, change the METHOD from ident to 
password as below:

# TYPE  DATABASE    USER        CIDR-ADDRESS          METHOD

# "local" is for Unix domain socket connections only
local   all         all                               password
# IPv4 local connections:
host    all         all         127.0.0.1/32          password
# IPv6 local connections:
host    all         all         ::1/128               ident



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

CREATE SEQUENCE id_sequence;

CREATE TABLE instant
(
  id bigint DEFAULT nextval('id_sequence') NOT NULL,
  datetime timestamp DEFAULT now(),
  sent boolean DEFAULT 'false',
  v1_voltage bigint,
  v2_voltage bigint,
  v3_voltage bigint,
  total_kw integer,
  total_pf integer,
  PRIMARY KEY(id)
);

CREATE TABLE demand
(
  id bigint DEFAULT nextval('id_sequence') NOT NULL,
  datetime timestamp DEFAULT now(),
  sent boolean DEFAULT 'false',
  kw_import_block_demand bigint,
  kvar_import_block_demand bigint,
  kva_block_demand bigint,
  PRIMARY KEY(id)
);

CREATE TABLE energy
(
  id bigint DEFAULT nextval('id_sequence') NOT NULL,
  datetime timestamp DEFAULT now(),
  sent boolean DEFAULT 'false',
  kwh_import bigint,
  kwh_import_l1 bigint,
  kwh_import_l2 bigint,
  kwh_import_l3 bigint,
  PRIMARY KEY(id)
);

-------------copy to here-----------

Sample insert

``
insert into demand (id,datetime, sent, kw_import_block_demand, kvar_import_block_demand, kva_block_demand)
    values (nextval('id_sequence'),now(),false,1,2,3);
``


Installing the service:
1. copy (or simlink) cenergy binary to /usr/local/bin
2. copy energy_db_wd.conf to /etc/init

