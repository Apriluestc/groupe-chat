#!/bin/bash

ROOT_PATH=$(pwd)
server=$ROOT_PATH/bin/server
conf=$ROOT_PATH/conf/server.conf

function usage() {
	printf "Usage: %s [-s(start) | -t(stop) | -rt(restart)]\n" "$0"
}
	
function startServer() {
	pid=$(pidof $server)
	if [ $? -eq 0 ];then
		echo  "server is already runing"	
	else
		ip=$(awk -F: '/ip/{print $2}' $conf)
		port=$(awk -F: '/port/{print $2}' $conf)
		$server $ip $port
	fi
}

function stopServer() {
	pid=$(pidof $server)
	if [ $? -eq 0 ];then
		kill -9 $pid
	else
		echo "server is already stop"
	fi
}

function restartServer() {
	stopServer
	startServer
}


if [ $# -ne 1 ] ; then
	usage
fi 

case $1 in 
	-s | 'start' )
	startServer
	;;
	-t | 'stop' )
	stopServer
	;;
	-rt | 'restart')
	restartServer
	;;
esac
