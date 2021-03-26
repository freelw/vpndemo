#!/bin/bash

source ./vpnrc
nohup ./server > ./vpn_server.log &
ip addr add ${VPN_SERVER_IP}/${VPN_MASK} dev tun0
ip link set tun0 up