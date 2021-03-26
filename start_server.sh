#!/bin/bash

source ./vpnrc
nohup ./server > ./vpn_server.log 2>&1 &
echo 'sleeping 5s...'
sleep 5s
ip addr add ${VPN_SERVER_IP}/${VPN_MASK} dev tun0
ip link set tun0 up