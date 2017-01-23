#!/usr/bin/env bash

TERM=${1:-term0.uarm}
> $TERM
uarm -c machine.uarm.cfg -e &
UARM_PID=$!
trap "kill $UARM_PID 2>&1 >/dev/null" EXIT

GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"
while IFS='' read -r LINE || [[ -n "$LINE" ]]; do
	case $LINE in
		"SYSTEM HALTED.")
			echo -e "${GREEN}${LINE}${RESET}"
			exit 0 ;;
		"KERNEL PANIC!")
			echo -e "${RED}${LINE}${RESET}"
			exit 1 ;;
		"UNKNOWN SERVICE.")
			echo -e "${RED}${LINE}" ;;
		*)
			echo $LINE ;;
	esac
done < <(tail -f $TERM)
