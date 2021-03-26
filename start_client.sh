#!/bin/bash

source ./vpnrc
nohup ./client ${SERVER_REAL_IP} > ./vpn_client.log 2>&1 &
echo 'sleeping 5s...'
sleep 5s
ip addr add ${VPN_CLIENT_IP}/${VPN_MASK} dev tun0
ip link set tun0 up