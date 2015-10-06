# GRRV Raspberry Pi B+ WiFi Interface

The WiFi access point is set up using the version 1.0 of the hostapd application and the program dnsmasq version 2.62-3 that are available at the official Debian Wheezy repository.

The adapter used was a Comfast CF-WU715N that uses the Ralink RT5370 driver.

### Setting up the WiFi Interface
The file at `etc/network/interfaces` need to be changed to configure the WiFi interface.
```bash
auto wlan0
allow-hotplug wlan0
iface wlan0 inet static
address 10.0.0.01
netmask 255.255.255.0
```

Hostapd can be installed using the standard package manager.
```bash
sudo apt-get install hostapd
```


The configuration file `/etc/hostapd/hostapd.conf` must be set as follows.
```bash
interface=wlan0 
driver=nl80211
ctrl_interface=/var/run/hostapd
ctrl_interface_group=0 

ssid=CgroupRasPi
hw_mode=g
channel=8

wpa=2
wpa_psk=928519398acf811e96f5dcac68a11d6aa876140599be3dd49612e760a2aaac0e
# The line above sets the wpa passphrase to "raspiwlan", 
# is obtained via the wpa_passphrase command.

wpa_key_mgmt=WPA-PSK
wpa_pairwise=CCMP
rsn_pairwise=CCMP

beacon_int=100
auth_algs=3
wmm_enabled=1
```

Then you have to set up the program to run at boot adding a line at /etc/rc.local.
```bash
path/to/hostapd - B /etc/hostapd/hostapd.conf
```

The network now needs a DNS server, that will be done with the application dnsmasq. We used the latest version available at the standard Wheezy distribution for Raspberry Pi.
```bash
sudo apt-get install dnsmasq
```

The configuration must be changed at `/etc/dnsmasq.conf`.

```bash
interface=wlan0
dhcp-range=10.0.0.2,10.0.0.5,255.255.255.0,12h 
```

Reboot and you should have a working WiFi access point.