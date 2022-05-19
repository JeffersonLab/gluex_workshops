#!/bin/bash

# sleep 8.5 hours and then send abort signal to all hd_root processes
# so they will core dump
#
# This is to debug a problem with jobs hanging at the end of the process
# until slurm timeout kills them. That does not result in a core dump
# probably due to it being killed from outside of the container.

echo 'assassin.sh starting'
date
sleep 26950

echo 'Sending abort'
date
pkill -6 hd_root
sleep 10

echo 'Sending term'
date
pkill -15 hd_root
sleep 10

echo 'Sending 9 ints'
date
pkill -2 hd_root
sleep 1
pkill -2 hd_root
sleep 1
pkill -2 hd_root
sleep 1
pkill -2 hd_root
sleep 1
pkill -2 hd_root
sleep 1
pkill -2 hd_root
sleep 1
pkill -2 hd_root
sleep 1
pkill -2 hd_root
sleep 1
pkill -2 hd_root
sleep 1
pkill -2 hd_root
sleep 10

echo 'Sending quit'
pkill -3 hd_root
sleep 10


