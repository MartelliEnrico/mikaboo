#!/usr/bin/env bash -eu
set -o pipefail

terminal=${1:-term0.uarm}
> $terminal
echo "Starting uArm..."
uarm -c machine.uarm.cfg -e -x --no-gui &
uarm_pid=$!
kill-uarm() {
	kill $uarm_pid 2>&1 >/dev/null
}
trap kill-uarm EXIT

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
