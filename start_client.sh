#!/bin/bash

source ./vpnrc
nohup ./client ${SERVER_REAL_IP} > ./vpn_client.log &
echo 'sleeping 5s...'
sleep 5s
ip addr add ${VPN_CLIENT_IP}/${VPN_MASK} dev tun0
ip link set tun0 up