In Nagios Central:
 
define host{
      use linux-server
      host_name NUC
      alias NUC
      address dynamic
      active_checks_enabled 0
      passive_checks_enabled 1
 
 
}
 
define service{
      use generic-service
      host_name NUC
      service_description cenergy
      check_command passive_check_example
      active_checks_enabled 0
      passive_checks_enabled 1
}
 
define service{
      use generic-service
      host_name NUC
      service_description jenergy
      check_command passive_check_example
      active_checks_enabled 0
      passive_checks_enabled 1
}
 
In Nagios Central, /etc/nagios/objects/commands.cfg
, append to:
 
define command{
	command_name passive_check_example
	command_line /bin/echo >> /dev/null
	} 
 
Send Script:
 
#!/bin/bash
####script to check local processes using check_procs and send result to nsca server
send_nsca="/usr/sbin/send_nsca"
send_nscaconf="/etc/nagios/send_nsca.cfg"
nagios_plugin_path=/usr/lib64/nagios/plugins
nsca_server=localhost
process_name=$1
##check if check_procs plugin is installed on the plugin folder
if [ -f $nagios_plugin_path/check_procs ]
then
process_status=`$nagios_plugin_path/check_procs -w 0.0 -c 1.0 -C $process_name `
#### if process is Okay
if [ `echo $process_status|awk '{print $2}'|sed 's/://g'` == "OK" ]
then
echo -e "$HOSTNAME\t\t$process_name\t0\t$process_status\n" | $send_nsca -H $nsca_server -p 5667 -c $send_nscaconf
else
echo -e "$HOSTNAME\t\t$process_name\t2\t$process_status\n" | $send_nsca -H $nsca_server -p 5667 -c $send_nscaconf
fi
 
else
echo "check_procs is not installed under " $nagios_plugin_path
fi
 
Python Script to fake process, cenergy. (Just write this in cenergy then run ./cenergy)
 
Cron Rule, 1 for each process, every 1 minute, crontab -e:
 
***** /root/script.sh cenergy
 
5 minutes:
 
*/5**** /root/script.sh cenergy

UDEV Rules:

Make a file /etc/udev/rules.d/100-usb.rules
Contents:
*KERNEL=="ttyUSB0", MODE="0777"
*KERNEL=="ttyUSB1", MODE="0777"
