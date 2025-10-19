Router> enable
Router# configure terminal

# Configure LAN 1 Interface (connects to Switch1)
Router(config)# interface fastEthernet0/0
Router(config-if)# ip address 192.168.1.1 255.255.255.0
Router(config-if)# no shutdown
Router(config-if)# description "LAN 1 Gateway"
Router(config-if)# exit

# Configure LAN 2 Interface (connects to Switch2)
Router(config)# interface fastEthernet0/1
Router(config-if)# ip address 192.168.2.1 255.255.255.0
Router(config-if)# no shutdown
Router(config-if)# description "LAN 2 Gateway"
Router(config-if)# exit

# Enable IP routing (should be on by default)
Router(config)# ip routing

# Save configuration
Router(config)# exit
Router# copy running-config startup-config