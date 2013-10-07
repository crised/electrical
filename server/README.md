electrical
==========

Electrical M2M software to get an Electrical Bill approximation


1. Entity(s) related to Electrical Variables1
This JSON Objacted will be HTTP POSTed from ARM Device, will be made every 15`. 
For example at 08:00, 08:15, 08:30, 08:45, 9:00... 


{
 "\"TimeStamp\":\"%s\","
			"\"id\":%lu,"
			"\"voltage1\":%u,"
			"\"voltage2\":%u,"
			"\"voltage3\":%u,"
			"\"voltage13\":%u,"
			"\"voltage23\":%u,"
			"\"voltage31\":%u,"
			"\"current1\":%u,"
			"\"current2\":%u,"
			"\"current3\":%u,"
			"\"actPower1\":%u,"
			"\"actPower2\":%u,"
			"\"actPower3\":%u,"
			"\"reactPower1\":%u,"
			"\"reactPower2\":%u,"
			"\"reactPower3\":%u,"
			"\"currentN\":%u,"
			"\"frequency\":%u,"
			"\"appPower1\":%u,"
			"\"appPower2\":%u,"
			"\"appPower3\":%u"
			"}"
			
Example URL 
			
2. Entity(s) Related to energy.
This JSON Object will be HTTP POSTed, from ARM device every 5'. At 8:00, 8:05, 8:10...)
{
    "id: 
    "Energy": "100250",
    "TimeStamp": "Thu Sep  5 16:16:47 CLT 2013"
}

Example URL to POST http://10.10.10.10/client1/device1/energy


Perhaps it's better not to include the id on the JSON object, instead in the URL.

About the user, and device entities. There is a one to many relationship here, one user can have many devices.
And the device entity  may have 2 Strings, like Name and Description.




Link to the docs:
https://docs.google.com/document/d/1LruXabD3js1uMHJbjtgtn_96fUXqv3CeVnnnc7lMAeA/edit?usp=sharing

Security
---

Almost all actions require user to be authenticated.
Role based security constraints are defined only on business layer using standard JEE annotations.

Basic authentication is used and it's triggered programatically by BasicAuthenticationFilter which in turn delegates authentication to JEE container.
Container should have "Electircal" security domain configured.
For development purposes AS7 config may look like this:

    <security-domain name="Electrical" cache-type="default">
        <authentication>
            <login-module code="org.jboss.security.auth.spi.SimpleServerLoginModule" flag="required"/>
        </authentication>
    </security-domain>

With such configuration one must provide password same as username to get authenticated.

For production configuration like this would be required:

    <security-domain name="Electrical" cache-type="default">
        <authentication>
           <login-module code="Database" flag="optional">
                <module-option name="dsJndiName" value="java:jboss/datasources/ElectricalDS"/>
                <module-option name="principalsQuery" value="select password from users where email=?"/>
                <module-option name="rolesQuery" value="select user_roles.roles,'Roles' from users,user_roles where users.id=user_roles.user_id and users.email=?"/>
                <module-option name="hashAlgorithm" value="SHA-512"/>
                <module-option name="hashEncoding" value="hex"/>
                <module-option name="password-stacking" value="true"/>
            </login-module>
        </authentication>
    </security-domain>

Container does not require any requests to be authenticated. In order to authenticate user the REST method must be annotated with @Authenticated,
our custom annotation that triggers BasicAuthenticationFilter.


Old import.sql, test@test.com 'aaaaa'

INSERT INTO users (id, name, email, phone_number, password) VALUES (1, 'Test User', 'test@test.com', '2125551212', 'f368a29b71bd201a7ef78b5df88b1361fbe83f959756d33793837a5d7b2eaf660f2f6c7e2fbace01965683c4cfafded3ff28aab34e329aa79bc81e7703f68b86');
INSERT INTO users (id, name, email, phone_number, password) VALUES (2, 'Devices', 'device', '2125551212', '8145c38e632913718e335cc428c703ef370f2b5fdb7dff370c4af4a8361dfde2452f109b848e860fd420080fc35b40d7f8e1eb05fc988197aa78768030aabc8a');

INSERT INTO user_roles (user_id, roles) VALUES (1, 'admin');
INSERT INTO user_roles (user_id, roles) VALUES (1, 'user');
INSERT INTO user_roles (user_id, roles) VALUES (2, 'device');

INSERT INTO device (id, name, user_id) VALUES (1, 'abc', 1);

INSERT INTO stats_entry (stats_type, id, created_date, kwh_import, kwh_import_l1, kwh_import_l2, kwh_import_l3, device_id) VALUES ('ENG', nextval('hibernate_sequence'), now(), 1, 2, 3, 4, 1);
INSERT INTO stats_entry (stats_type, id, created_date, kwh_import, kwh_import_l1, kwh_import_l2, kwh_import_l3, device_id) VALUES ('ENG', nextval('hibernate_sequence'), now(), 4, 5, 6, 7, 1);

INSERT INTO stats_entry (stats_type, id, created_date, hour, kva_block_demand, kvar_import_block_demand, kw_import_block_demand, device_id) VALUES ('DMD', nextval('hibernate_sequence'), now(), 4, 5, 6, 7, 1);
INSERT INTO stats_entry (stats_type, id, created_date, hour, kva_block_demand, kvar_import_block_demand, kw_import_block_demand, device_id) VALUES ('DMD', nextval('hibernate_sequence'), now(), 18, 9, 10, 11, 1);
