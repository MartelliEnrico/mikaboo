#!/usr/bin/env bash

TERM=${1:-term0.uarm}
> $TERM
uarm -c machine.uarm.cfg -e &
UARM_PID=$!
trap "kill $UARM_PID" EXIT

while IFS='' read -r LINE || [[ -n "$LINE" ]]; do
	echo $LINE
	case $LINE in
		"SYSTEM HALTED.") exit 0 ;;
		"KERNEL PANIC!") exit 1 ;;
	esac
done < <(tail -f $TERM)
