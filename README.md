#Overview

This is a simple shell which will get the login name and host and output them together 
with a dollar sign at the end.

Following this will be an input by the user which will be processed and acted on 
appropriately. 
	
#How to Install

All you need to do is to just: 
	
1.) clone the repository and type in 'make',
2.) type 'bin/rshell' to run the program,
	
#How to Use

You may type and run several types of executable commands and their parameters. They
may also be connected by a connector. It will work with any number of commands and 
parameters and connectors may be placed anywhere in between commands. Some examples 
include: 
	
* ls -a
* pwd ; ls -a
*       ls      -a;	        pwd&&exit
* ls    && pwd ;ls   -a ||ps

This shell can also run another instance of itself, which can be seen if you use 'ps'. 

When connectors are used, the commands are read from left to right and only for the 
next command. 

For example: 
'''
ls || pwd ; ps
'''
The 'ls' will run and return true. Since the connector is an '||', 'pwd' will not run,
but the next command, 'ps', will. 

###Whitespace
	
Inserting whitespace anywhere around your commands will still compile and will only 
fail if there is a bad input.

###Exit

An input of 'exit' will take you out of the terminal. This will work with any 
parameters that come after it. However, if it is passed in as a parameter, it will 
not work. 

For any bad inputs, the commands will not run and you will be prompted to try again.

###Comments

Comments may be used by inserting a '#' before your comment. It may be placed anywhere 
in the input, but any characters after it will be ignored. 

'''
ls -a #;pwd
'''

'ls -a' will run but since 'pwd' is after the '#' sign, it will be ignored.



