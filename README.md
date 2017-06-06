# Text-Formatting
In this program we will write a simple text formatter that reads a text file  with embedded formatting commands and writes a text file that contains the  input file formatted according to the embedded formatting commands. The  embedded commands begin with a . (period) and appear on a line by themselves.

**.br** Start a new line

**.pp** Start a new paragraph 

**.bp** Start a new page 

**.nf** Stop formatting -- copy input to output 

**.fi** Resume formatting 

**.pn** Put page numbers on bottom of each page 

**.po** Don't put page numbers 

**.ls** n Spacing between lines is n 

**.sp** n Leave n blank lines 

**.rj** Output is right justified 

**.nr** Output is not right justified 

**.ce** n Center the next n lines 

**.ll** n Output line length is n characters 

**.pl** n Page length is n lines 

**.pi** n Paragraph indentation is n spaces


Assume the following commands are at the beginning of every input file (i.e. default settings): 
.ll 40 

.pn 

.ls 1 

.pl 20 

.pi 5 

.fi 

The program may have multiple input files that will all be given on the command line (i.e. formatter file1 
file2 file3). The output file names are the input file names with extension out. (i.e. formatter file1 file2 
file3 will produce files file1.out file2.out file3.out) If a given input file does not exist in the current 
directory, the program must give an error message but continue processing the other files (if any). 
If no input file is given in the command line, the program must read from the standard input and write 
to standard output. 
Assume that all input lines are less than 80 characters long. 
