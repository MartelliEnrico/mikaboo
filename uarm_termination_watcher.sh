#!/usr/bin/env bash

terminal=${1:-term0.uarm}
> $terminal
uarm -c machine.uarm.cfg -e &
uarm_pid=$!
trap "kill $uarm_pid 2>&1 >/dev/null" EXIT

green="\033[0;32m"
red="\033[0;31m"
reset="\033[0m"
while IFS='' read -r line || [[ -n "$line" ]]; do
	case $line in
		"SYSTEM HALTED.")
			echo -e "${green}${line}${reset}"
			exit 0 ;;
		"KERNEL PANIC!")
			echo -e "${red}${line}${reset}"
			exit 1 ;;
		"UNKNOWN SERVICE.")
			echo -e "${red}${line}" ;;
		*)
			echo $line ;;
	esac
done < <(tail -f $terminal)
