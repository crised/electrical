-- You can use this file to load seed data into the database using SQL statements
insert into Member (id, username, name, email, phone_number) values (1, 'abc','Test User', 'test@test.com', '2125551212');

insert into Device (id, name, member_id) values (1, 'abc', 1);