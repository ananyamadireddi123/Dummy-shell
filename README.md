# Description
This project implements a shell with aleady existing commands and the creating of new version of the old commands.
This is being divided into various c files representing various functions implemented.
There is are warnings being generated when we run the make file(ignore them).

HOW TO RUN
prompt> make
prompt> ./a.out

There you will have respective prompt ready to accept commands.

# Assumptions

1) Working of echo is slightly different. echo function assumes that wht ever the text may be after the command is a command to be printed, and it takes everything after echo to be a joint string, since handling echo is not explicitely mentioned, i am going to keep the functionaity as it is.
2) I am considering even the commands which are wrong under the pastevents list
3) "seek -e -f proc" will give the complete content of file
4) For "warp -" whenever we do it twice, without any change in the directory it will stay in the same directory.
5) For the seek function, even the words in which the entered search word is a sub-string will also be displayed.
6) I assumed that -e flag cannot be used alone and has to be accompanied by -d or -f flag.
.
