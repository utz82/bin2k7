********************************************************************************
BIN2K7 v0.1
by utz 07'2016 * www.irrlichtproject.de
********************************************************************************


About
=====

bin2k7 is a utility that converts Z80 binaries into .k7 tape files for use on
Micronique Hector computers.

It is currently in an early beta stage, so correct output is not guaranteed.



Usage
=====

bin2k7 is invoked as follows:

bin2k7 -i <infile> -d <destination> [-p <pc> -s <sp> -o <outfile>]

The options in detail:

-i <infile>        The binary file you want to convert.
-d <destination>   The target address in RAM, in hexadecimal (base 16).
-p <pc>            The value that the program counter (PC register) will take
                   after loading has completed. Specifying this is optional.
                   This option currently does not work as intended.
-s <sp>            The value that the stack pointer (SP register) will take 
                   after loading has completed. Specifying this is optional.
-o <outfile>       The file name for the output tape file, without the .k7
                   extension. If it is not specified, the outfile will take the
                   name of the infile with the .k7 extension appended.


Compilation
===========

bin2k7.cpp should be compiled with -std=c++11.