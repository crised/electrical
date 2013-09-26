INSERT INTO users (id, name, email, role, phone_number, password) VALUES (1, 'Test User', 'test@test.com', 'ADMIN', '2125551212', 'f368a29b71bd201a7ef78b5df88b1361fbe83f959756d33793837a5d7b2eaf660f2f6c7e2fbace01965683c4cfafded3ff28aab34e329aa79bc81e7703f68b86');

INSERT INTO device (id, name, user_id) VALUES (1, 'abc', 1);

INSERT INTO stats_entry (stats_type, id, created_date, kwh_import, kwh_import_l1, kwh_import_l2, kwh_import_l3, device_id) VALUES ('ENG', nextval('hibernate_sequence'), now(), 1, 2, 3, 4, 1);
INSERT INTO stats_entry (stats_type, id, created_date, kwh_import, kwh_import_l1, kwh_import_l2, kwh_import_l3, device_id) VALUES ('ENG', nextval('hibernate_sequence'), now(), 4, 5, 6, 7, 1);

INSERT INTO stats_entry (stats_type, id, created_date, hour, kva_block_demand, kvar_import_block_demand, kw_import_block_demand, device_id) VALUES ('DMD', nextval('hibernate_sequence'), now(), 4, 5, 6, 7, 1);
INSERT INTO stats_entry (stats_type, id, created_date, hour, kva_block_demand, kvar_import_block_demand, kw_import_block_demand, device_id) VALUES ('DMD', nextval('hibernate_sequence'), now(), 18, 9, 10, 11, 1);