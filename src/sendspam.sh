#!/bin/bash

to=$1
from=$2
subject="This email is spam!"
body=`cat`

(echo helo mail.cs.ucr.edu 
sleep 1
echo mail from: $from 
sleep 1
echo rcpt to: $to 
sleep 1
echo data
sleep 1
echo Subject: $subject
sleep 1
echo "$body" 
sleep 1
echo .
sleep 1
echo quit
sleep 1) | telnet mail.cs.ucr.edu 25
sleep 1


