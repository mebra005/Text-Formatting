//Milad Ebrahimi
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define MAX_LENGTH 81
#define NOT_RIGHT_JUSTIFIED 0
#define RIGHT_JUSTIFIED 1
#define TRUE 1
#define FALSE 0

// struct contains global settings and current output line
struct Options
{
	int format;
	int pageNum;
	int putPageNum;
	int sentenceSpacing;
	int justification; 
	int lineLength;
	int curLineLength;
	int pageLength;
	int curPageLength;
	int paraIndent;
	int center;
	char outputLine[MAX_LENGTH];
};

// function to initialize default settings
void initOptions(struct Options *opt)
{
	opt->format = 1;
	opt->pageNum = 1;
	opt->putPageNum = 1;
	opt->sentenceSpacing = 1;
	opt->justification = RIGHT_JUSTIFIED;
	opt->lineLength = 40;
	opt->curLineLength = 0;
	opt->pageLength = 20;
	opt->curPageLength = 0;
	opt->paraIndent = 5;
	opt->center = 0;
	opt->outputLine[0] = '\0';
}

// function to check whether c is a space
int isSpace(char c)
{
	if (c == ' ')
		return TRUE;
	else
		return FALSE;
}

// function implements page break functionality , line break.
void pageBreak(FILE *outputFile, struct Options *opt, int end)
{
	int i = 0;
	// it breaks the line, start a new line.
	if (strlen(opt->outputLine))
	{
		fprintf(outputFile, "%s\n", opt->outputLine);
		
		//by default space between each line is 1, so the for loop
		//only execute when the sentenceSpacing is set to bigger than 1
		//example : sentenceSpacing = 2 is samething as (Double space)
		for (i = 1; i < opt->sentenceSpacing; i++)
		{
			fprintf(outputFile,"\n", opt->sentenceSpacing);
		}
		opt->curLineLength = 0; // reset the current line length to Zero
		strcpy(opt->outputLine, "");
		opt->curPageLength++; // increment the current page length
	}

	// if the current page length is equal to pageLength ( 20 by default(19)) and
	// we are at the end of page ( end = TRUE)
	if (opt->curPageLength == opt->pageLength-1 || end) 								
	{
		if (opt->putPageNum) //if true print a page number
			fprintf(outputFile, "Page=%-3d", opt->pageNum);
		fprintf(outputFile, "***********NEW PAGE ****************\n");
		opt->pageNum++; // increment the page number
		opt->curPageLength = 0; // reset the page length back to zero
	}
}

// function applies text editor formating based on global values for opt
// pp is used to declare paragraph indenting
void applyFormating(FILE *outputFile,char *line,struct Options *opt, int pp)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int count = 0;
	int index = 0;
	int space = 0;
	int leading = 0;
	int trailing = 0;

	if (opt->curLineLength > 0 && opt->outputLine[strlen(opt->outputLine)-1] != ' ')
	{
		opt->outputLine[opt->curLineLength] = ' ';
		opt->curLineLength++;
		opt->outputLine[opt->curLineLength] = '\0';

	}
	for (i = 0; i < strlen(line); i++)
	{
		if (opt->curLineLength == 0 && isSpace(line[i]) && !pp)
		{
			// do nothing
		}
		else
		{
			opt->outputLine[opt->curLineLength] = line[i];
			opt->curLineLength++;
			opt->outputLine[opt->curLineLength] = '\0';
		}

		if (opt->curLineLength >= opt->lineLength-1)
		{
			//terminate line
			opt->outputLine[opt->curLineLength] = '\0';
			// if word is being cut
			if (i + 1 < strlen(line) && isSpace(line[i + 1]) == FALSE)
			{
				// if only 1 char of word is going out of line
				if ((i + 2 < strlen(line) && isSpace(line[i + 2]) == TRUE) || (i + 2 >= strlen(line)))
				{
					i++;
					opt->outputLine[opt->curLineLength] = line[i];
					opt->curLineLength++;
					opt->outputLine[opt->curLineLength] = '\0';
				}
				else
				{
					index = opt->curLineLength-1;
					// take word to new line
					while (isSpace(opt->outputLine[index]) == FALSE)
					{
						index--;
						i--;
					}
					opt->outputLine[index] = '\0';
					opt->curLineLength = strlen(opt->outputLine);
				}
			}

			if (opt->center)
			{
				space = opt->lineLength - strlen(opt->outputLine);
				if (space)
				{
					char temp[MAX_LENGTH];
					strcpy(temp, "");
					for (j = 0; j < ((space + 1) / 2); j++)
						strcat(temp, " ");
					strcat(temp, opt->outputLine);
					strcpy(opt->outputLine, temp);
				}
				opt->center--;
			}
			else if (opt->justification == RIGHT_JUSTIFIED)
			{
				space = opt->lineLength - strlen(opt->outputLine);
				while(space)
				{
					leading = 0;
					trailing = 0;
					for (j = 0; j < strlen(opt->outputLine) && space; j++)
					{
						if (isSpace(opt->outputLine[j]) == FALSE)
						{
							leading = 1;
						}

						//only extend non leading spaces
						if (isSpace(opt->outputLine[j]) && leading)
						{
							opt->outputLine[strlen(opt->outputLine) + 1] = '\0';
							for (k = strlen(opt->outputLine); k >= j; k--)
							{
								opt->outputLine[k] = opt->outputLine[k-1];
							}
							opt->outputLine[j] = ' ';
							space--;
							while (isSpace(opt->outputLine[j]))
								j++;
							trailing++;
						}
					}
					if (trailing == 0)
					{
						strcat(opt->outputLine, " \0");
						space--;
					}
				}
			}

			pageBreak(outputFile, opt, FALSE);

		}
	
	}
	
}

void parseLine(FILE * outputFile, char * line, struct Options *opt)
{
	int i = 0;
	char command[MAX_LENGTH];
	char num[MAX_LENGTH];
	int isCommand = 1;
	int n = 0;
	if (opt->format)// if format = TRUE (.fi by defualt is set to TRUE)
	{
		sscanf(line, "%s %s", command, num); //it gets the command and the value after command
		n = atoi(num); //convert char value to integer
		if (strlen(line) > 0)//if the length of string w/o null character is bigger than Zero
		{	
			// compares the command vs. ".br"
			if (strcmp(command, ".br") == 0) //Start a new line
			{
				pageBreak(outputFile, opt, FALSE);
			}
			else if (strcmp(command, ".pp") == 0)//Start a new paragraph
			{
				char indent[MAX_LENGTH];
				strcpy(indent, "");
				pageBreak(outputFile, opt, FALSE);

				for (i = 0; i < opt->paraIndent; i++)
					strcat(indent, " ");

				applyFormating(outputFile, indent, opt, TRUE);
				isCommand = 0;
			}
			else if (strcmp(command, ".bp") == 0)//Start a new page
			{
				pageBreak(outputFile, opt, TRUE);
			}
			else if (strcmp(command, ".nf") == 0)//Stop formatting -- copy input to output
			{
				opt->format = 0;
			}
			else if (strcmp(command, ".fi") == 0)//Resume formatting
			{
				opt->format = 1;
			}
			else if (strcmp(command, ".pn") == 0)//Put page numbers on bottom of each page
			{
				opt->putPageNum = 1;
			}
			else if (strcmp(command, ".po") == 0)//Don't put page numbers
			{
				opt->putPageNum = 0;
			}
			else if (strcmp(command, ".ls") == 0)//Spacing between lines is n
			{
				opt->sentenceSpacing = n;
			}
			else if (strcmp(command, ".sp") == 0)//Leave n blank lines
			{
				pageBreak(outputFile, opt, FALSE);
				for (i = 1; i < n; i++)
				{
					fprintf(outputFile, "\n"); // leave 1 blank line 
					opt->curPageLength++; //increment the current page length
				}
			}
			else if (strcmp(command, ".rj") == 0)//Output is right justified
			{
				opt->justification = RIGHT_JUSTIFIED;
			}
			else if (strcmp(command, ".nr") == 0)//Output is not right justified
			{
				opt->justification = NOT_RIGHT_JUSTIFIED;
			}
			else if (strcmp(command, ".ce") == 0)//Center the next n lines
			{
				opt->center = n;
			}
			else if (strcmp(command, ".ll") == 0)//Output line length is n characters
			{
				opt->lineLength = n;
			}
			else if (strcmp(command, ".pl") == 0)//Page length is n lines
			{
				opt->pageLength = n;
			}
			else if (strcmp(command, ".pi") == 0)//Paragraph indentation is n spaces
			{
				opt->paraIndent = n;
			}
			else
			{
				// apply formating
				applyFormating(outputFile,line, opt, FALSE);
				isCommand = 0;
			}

			if (isCommand)
			{
				pageBreak(outputFile, opt ,FALSE);
			}
		}
	}
	else
	{
		if (strcmp(line, ".fi") == 0)
		{
			opt->format = 1;
		}
		else
			fprintf(outputFile, "%s\n", line);
	}
}

void processFile(char *fileName)
{
	FILE *inputFile;
	FILE *outputFile;
	char tempFileName[MAX_LENGTH];
	struct Options opt;
	char ch = '\0';
	int count = 0;

	char line[MAX_LENGTH];
	inputFile = fopen(fileName, "r"); // fileName is equal to argv[i]
	if (inputFile == NULL)
	{
		printf("Error: cannot open file %s\n", fileName);
		return;
	}
	strcpy(tempFileName, "");
	strcat(tempFileName, fileName);
	strcat(tempFileName, ".out");
	
	outputFile = fopen(tempFileName, "w");
	// initialize options to default settings
	initOptions(&opt);
	while (ch != EOF)
	{
		ch = getc(inputFile);
		if (ch == '\n')
		{
			line[count] = '\0';
			parseLine(outputFile,line, &opt);
			count = 0;
		}
		else
		{
			line[count] = ch;
			count++;
		}
	}
	pageBreak(outputFile, &opt, TRUE);
	fclose(inputFile);
	fclose(outputFile);
	
}

int main(int argc, char *argv[])
{
	int i = 0;
	struct Options opt;
	char ch = '\0';
	int count = 0;
	char line[MAX_LENGTH];

	for (i = 1; i < argc; i++)
	{
		processFile(argv[i]);
	}

	// if no files given
	if (argc == 1)
	{
		printf("no files in argument list, please type your sentences.")
		initOptions(&opt);
		while (ch != EOF)
		{
			ch = getc(stdin);
			if (ch == '\n')
			{
				line[count] = '\0';
				parseLine(stdout, line, &opt);
				count = 0;
			}
			else
			{
				line[count] = ch;
				count++;
			}
		}
		pageBreak(stdout, &opt, TRUE);
	}

	return 0;
}