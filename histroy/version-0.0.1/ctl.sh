#!/bin/bash

SERVER_PATH=$(pwd)
server=$SERVER_PATH/bin/server
conf=$SERVER_PATH/conf/server.conf
echo "server.conf 载入成功 ···" >> $SERVER_PATH/bin/log/server.log

function usage() {
	printf "Usage: %s [-s(start) | -q(stop) | -r(restart)]\n" "$0"
}
	
function startServer() {
	pid=$(pidof $server)
	if [ $? -eq 0 ];then
        echo  "server is already runing ···"
        echo  "server is already runing ···" >> $SERVER_PATH/bin/log/server.log
	else
		ip=$(awk -F: '/ip/{print $2}' $conf)
		port=$(awk -F: '/port/{print $2}' $conf)
		$server $ip $port &
        echo "server start success ···"
        echo "server start success ···" >> $SERVER_PATH/bin/log/server.log
	fi
}

function stopServer() {
	pid=$(pidof $server)
	if [ $? -eq 0 ];then
		kill -9 $pid
        echo "server stop success ···"
        echo "server stop success ···" >> $SERVER_PATH/bin/log/server.log
	else
        echo "server is already stop ···"
        echo "server is already stop ···" >> $SERVER_PATH/bin/log/server.log
	fi
}

function restartServer() {
	stopServer
    echo "server is already stop ···"
    echo "server is already stop ···" >> $SERVER_PATH/bin/log/server.log
	startServer
    echo "server is already restart ···"
    echo "server is already restart ···" >> $SERVER_PATH/bin/log/server.log
}


if [ $# -ne 1 ] ; then
	usage
fi 

case $1 in 
	-s | 'start' )
	startServer
	;;
	-q | 'stop' )
	stopServer
	;;
	-r | 'restart')
	restartServer
	;;
esac
