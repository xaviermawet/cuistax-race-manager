# Server VPS 2014 Classic 1

- **IP address**: 198.50.146.128
- **Operating system**: Debian 8 (Jessie)
- **Language**: English
- **Domain names**: 
	-  vps148995.ovh.net
	- jdm.nakim.be
	- *cuistax.jeunesse-magnee.be* (**coming soon**)

## Main configuration

```bash
ssh root@jdm.nakim.be

# Change default root password
passwd

# Update system
apt-get update
apt-get upgrade

# Select locale
vim /etc/locale.gen
    en_US.UTF-8 UTF-8
locale-gen
export LANG=en_US.UTF-8

# Change hostname
hostnamectl set-hostname vps2014CA
vim /etc/hostname # Check if well updated
vim /etc/hosts
    127.0.0.1       localhost               vps2014ca
    198.50.146.128  vps99956.vps.ovh.ca     vps2014ca
    198.50.146.128  jdm.nakim.be            vps2014ca

/etc/init.d/hostname.sh start

# Add new user and add it to sudo group
adduser nakim
passwd nakim
usermod -a -G sudo nakim
```

Now, log out and connect again with the newly created user (`nakim`).

## SSH Configuration

On a local computer :

```bash
# Generate ssh key
cd ~/.ssh
ssh-keygen -t rsa -b 2048 -f vps2014ca -C "SSH access to VPS 2014 Classic 1 for user nakim"

# Copy key on the remote server
ssh-copy-id -i vps2014ca.pub nakim@jdm.nakim.be
# or
scp -P <other_port> ~/.ssh/vps2014ca.pub nakim@jdm.nakim.be:~/.ssh/authorized_keys
```

On remote server (vps2014ca) :

```bash
# Check if key is copied
cd ~/.ssh
cat authorized_keys

# Create a group for ssh users
groupadd sshusers
usermod -a -G sshusers nakim

# Change sshd settings
vim /etc/ssh/sshd_config
    Port 5999                 # Update
    Protocol 2                # Check
    LoginGraceTime 30         # Update
    PermitRootLogin no        # Update
    AllowGroups sshusers      # Add
    MaxAuthTries 5            # Add
    PasswordAuthentication no # Uncomment and Update. WARNING: To do if ssh with key woks

# Restart SSH server
systemctl restart ssh

# Keep the current ssh session opened
```

On local computer, try logging into the machine with SSH key:

```bash
# /!\ Try to connect with the new configuration before closing current session /!\
ssh -i ~/.ssh/vps2014ca -p 5999 nakim@jdm.nakim.be

# Create a config file for ssh
vim ~/.ssh/config
    Host    jdm
        Hostname        jdm.nakim.be
        IdentityFile    ~/.ssh/vps2014ca
        Port            5999
        User            nakim

# Try to log in with
ssh jdm
```

## IPTable filtering

**WARNING** : Before flushing all rules in chaines, make sure the default policy for each chain is set to `ACCEPT` ! 

```bash

# Change default policy to ACCEPT for all chaines
iptables -P INPUT ACCEPT
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT

iptables -F           # "-t filter" no need to specify
iptables -t nat -F    # flush all rules in chaines of nat table
iptables -t mangle -F # flush all rules in chaines of mangle table
iptables -X           # Remove all chaines defined by user

# Create new chain for all the trafic comming from Internet to eth0
iptables -N bad-me
iptables -N private

# Some general rules in INPUT chain (table filter)
1 iptables -A INPUT -i lo -j ACCEPT                                # Allows all loopback (lo0) trafic
2 iptables -A INPUT ! -i lo -d 127.0.0.0/8 -j REJECT               # drop all trafic to 127/8 that doesn't use lo0
3 iptables -A INPUT -m state --state INVALID -j DROP               # Drop invalid trafic
4 iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT # Accepts all established inbound connections
5 iptables -A INPUT -i venet0 -j private                             # Will drop all trafic with private ip addresses
6 iptables -A INPUT -i venet0 -j bad-me                              # Will defines all the trafic accepted from internet
7 iptables -A INPUT -j DROP/REJECT                                 # Reject all other inbound

# Some general rules in bad-me chain (table filter)
1 iptables -A bad-me -p tcp -m state --state NEW --dport 5999 -j ACCEPT # Allows SSH connections on port 5999
2 iptables -A bad-me -p tcp -m tcp --dport 5998 -j ACCEPT               # Allows remote connection to mariadb server
3 iptables -A bad-me -p tcp --dport 80 -j ACCEPT                        # Allows HTTP connections from anywhere
4 iptables -A bad-me -p tcp --dport 443 -j ACCEPT                       # Allows HTTPS connections from anywhere
5 iptables -A bad-me -p tcp --dport 8443 -j ACCEPT                      # Allows HTTPS connections from anywhere
6 iptables -A bad-me -p icmp -m icmp --icmp-type 8 -j ACCEPT            # Allow ping

# Some general rules in private chain (table filter)
1 iptables -A private -s 10.0.0.0/8 -j DROP
2 iptables -A private -s 127.0.0.0/8 -j DROP
3 iptables -A private -s 172.16.0.0/12 -j DROP
4 iptables -A private -s 192.168.0.0/16 -j DROP

# Some general rules in OUTPUT chain (table filter)
1 iptables -A OUTPUT -o venet0 -d 10.0.0.0/8 -j REJECT
2 iptables -A OUTPUT -o venet0 -d 127.0.0.0/8 -j REJECT
3 iptables -A OUTPUT -o venet0 -d 172.16.0.0/12 -j REJECT
4 iptables -A OUTPUT -o venet0 -d 192.168.0.0/16 -j REJECT
5 iptables -A OUTPUT -j ACCEPT                                     # Allows all outbound trafic

# Some general rules in FORWARD chain (table filter)
1 iptables -A FORWARD -j DROP/REJECT

# Change default policy to DROP for all chaines
iptables -P INPUT DROP
iptables -P OUTPUT DROP
iptables -P FORWARD DROP

# See all rules
iptables -L -n --line-numbers

# Save rules in file to be able to restore all of them with only one command
mkdir /etc/iptables
iptables-save > /etc/iptables/rules.v4 

# Rules can be restored with 
iptables-restore < /etc/iptables/rules.v4

# Restore rules automatically after reboot
sudo apt-get install iptables-persistent
```


## Fail2Ban installation and configuration

- https://www.upcloud.com/support/installing-fail2ban-on-debian-8-0/

```bash
# Install fail2ban
sudo apt-get install fail2ban

# Because of the way that fail2ban updates its configuration files when the
# program has a new version, we should not edit the default configuration file.
# copy the default jail.conf file to make a local configuration
sudo cp /etc/fail2ban/jail.conf /etc/fail2ban/jail.local

# open the new local configuration file for edit
sudo vim /etc/fail2ban/jail.local

    bantime  = 3600 # determines how long an offending host will remain blocked until automatically unblocked (seconds).
    findtime = 600  # sets the time window for the max retry attempts before the host IP attempting to connect is blocked.
    maxretry = 3
    
    destemail = xavier@nakim.be # enable the email notifications if you have a sendmail service configured on your server
    action = %(action_mw)s

    [ssh]
    enabled  = true
    port     = 5999
    filter   = sshd
    logpath  = /var/log/auth.log
    maxretry = 4

    [ssh-ddos]
    enabled  = true
    port     = 5999
    filter   = sshd-ddos
    logpath  = /var/log/auth.log
    maxretry = 4

# restart the monitor 
sudo service fail2ban restart
```


## MariaDB 10.2 Installation

- https://downloads.mariadb.org/mariadb/repositories/#mirror=nucleus&distro=Debian&distro_release=jessie--jessie&version=10.2
- http://www.tecmint.com/install-mariadb-in-debian/

```bash
# Here are the commands to run to add MariaDB to your system:
sudo apt-get install software-properties-common
sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 0xcbcb082a1bb943db
sudo add-apt-repository 'deb [arch=amd64,i386,ppc64el] http://mariadb.mirror.nucleus.be/repo/10.2/debian jessie main'

# Once the key is imported and the repository added you can install MariaDB with:
sudo apt-get update
sudo apt-get install mariadb-server
    login: root
    passwd: m******X

# If installation goes smooth, check the version of Installed MariaDB.
mysql -V 
    --> mysql  Ver 15.1 Distrib 10.2.5-MariaDB, for debian-linux-gnu (x86_64) using readline 5.2

# Login to MariaDB using root (Not Recommended), followed by password.
mysql -u root -p

# Create a user in MariaDB. Use the following syntax to create a user in MariaDB.
CREATE USER 'nakim' IDENTIFIED BY 'm******X';
    --> Query OK, 0 rows affected (0.00 sec)

# Grant all privileges to newly created user (nakim)
GRANT ALL PRIVILEGES ON * . * TO 'nakim';
    --> Query OK, 0 rows affected (0.10 sec)

# Try to log in with the newly created user
mysql -u nakim -p

# Enable remote connection. Edit file and comment following lines (the order of these lines don't matter)
vim /etc/mysql/my.cnf
    [mysqld]
    ...
    #skip-networking
    ...
    #bind-address = <some ip-address>
    ...

# Restart mysql service
sudo /etc/init.d/mysql restart

```

## Nginx configuration

- https://www.digitalocean.com/community/tutorials/how-to-install-nginx-on-debian-8

```bash
# Install Nginx
sudo apt-get install nginx

```

Check if Nginx server is accessible : http://jdm.nakim.be/

```bash
# tart/stop/restart the server
sudo systemctl start/stop/restart nginx

# If you are simply making configuration changes, Nginx can often reload without dropping connections. To do this, this command can be used:
sudo systemctl reload nginx

# make sure that our web server will restart automatically when the server is rebooted:
systemctl enable nginx
```

### Server Root and Configuration

The default server root directory is `/var/www/htm`.
The default server block configuration file that ships with Nginx, is located at `/etc/nginx/sites-enabled/default`.

### Nginx Global Configuration

The main Nginx configuration file is located at `etc/nginx/nginx.conf`. This is where you can change settings like the user that runs the Nginx daemon processes, and the number of worker processes that get spawned when Nginx is running, among other things.