#!/bin/bash

if [[ $1 == '--help' ]]; then
  echo "Usage: $0 <host> <host> <port, ports, or port-range>"
  exit 0
fi

START_IP=$1;[ -z "$START_IP" ] && START_IP=192.168.1.1
END_IP=$2;[ -z "$START_IP" ] && END_IP=192.168.1.100
PORTS=$3;[ -z "$PORTS" ] && PORTS=1-1000
TIMEOUT=$3;[ -z "$TIMEOUT" ] && TIMEOUT=100
echo Scanning $START_IP to $END_IP \(ports $PORTS\)

scan() {
  local host=$1
  local ports=()
  local timeout=$(bc -l <<< "$3/1000.0")
  case $2 in
    *-*)
      IFS=- read start end <<< "$2"
        for ((port=start; port <= end; port++)); do
          ports+=($port)
        done
      ;;
    *,*)
      IFS=, read -ra ports <<< "$2"
      ;;
    *)
      ports+=($2)
      ;;
  esac


  for port in "${ports[@]}"; do
    timeout $timeout bash -c "echo >/dev/tcp/$host/$port" >/dev/null 2>&1 &&
    echo "port $port is open" ||
    echo "port $port is closed"
  done
}

function ctrl_c() {
  echo "** Trapped CTRL-C"
  exit 1
}

nextip() {
  IP=$1
  IP_HEX=$(printf '%.2X%.2X%.2X%.2X\n' `echo $IP | sed -e 's/\./ /g'`)
  NEXT_IP_HEX=$(printf %.8X `echo $(( 0x$IP_HEX + 1 ))`)
  NEXT_IP=$(printf '%d.%d.%d.%d\n' `echo $NEXT_IP_HEX | sed -r 's/(..)/0x\1 /g'`)
  echo "$NEXT_IP"
}

trap ctrl_c INT

TARGET_NAME_OR_IP=$START_IP
while :
do
  echo $TARGET_NAME_OR_IP
  scan $TARGET_NAME_OR_IP $PORTS $TIMEOUT
  [ $TARGET_NAME_OR_IP != $END_IP ] || break
  TARGET_NAME_OR_IP=$(nextip $TARGET_NAME_OR_IP)
done
