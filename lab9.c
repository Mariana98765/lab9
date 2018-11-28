#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT_TO_READING_LINES 30
#define LIMIT_TO_NUMBER_OF_QUESTIONS_PER_STUDENT 11
#define ERROR_CODE_IN_INPUT_SCANNING 'e'

void printCSVFile(char *fname, char *fname1, char *csvFile);
void printGreetingMessage();
void printSingleEntry(int id, char scores[]);
void printOneAnswerSummary(int ident, char key[], char answers[], int number_of_questions,
						   int missed_count[], double *grade);
void printIncorrectAnswersSummary(int number_of_questions, int missed_count[], int number_of_tests);
void printGradingSummary(int ident[], double grade[], int number_of_tests);

int main()
{
	printCSVFile("newomr.txt", "test_taker_names.txt", "full_data.csv");
	return EXIT_SUCCESS;
}

void printCSVFile(char *fname, char *fname1, char *csvFile)
{
	FILE *IN = fopen(fname, "r");
	FILE *IN2 = fopen(csvFile, "w+");
	fprintf(IN2, "IDENTITY,NAME,Q1,Q2,Q3,Q4,Q5,Q6,Q7,Q8,Q9,Q10,Q11,GRADE\n");
	int id;
	char scores[LIMIT_TO_NUMBER_OF_QUESTIONS_PER_STUDENT];
	double grades[LIMIT_TO_READING_LINES];
	int count = 0;
	int i;
	int number_of_questions;
	char key[LIMIT_TO_NUMBER_OF_QUESTIONS_PER_STUDENT + 1];

	/* print program information message */
	printGreetingMessage();

	if (!IN)
	{
		printf("Error opening input file %s.\n", fname);
		perror(fname);
		return;
	}

	/* Read first line with answer key */
	if (fscanf(IN, "%d", &id) == 1)
	{
		/* Read answer key */
		fscanf(IN, "%s", key);
		/* Find actual number of questions */
		number_of_questions = strlen(key);
	}

	/* Print answer key */
	printf("Read in the answer key:\n");
	for (i = 0; i < number_of_questions; i++)
		printf("\tQuestion %d: %c\n", i + 1, key[i]);
	int error = 0;
	/* Read all lines with answers */
	count = 0;
	int cc = 0;
	while (!feof(IN) && (fscanf(IN, "%d", &(id)) == 1))
	{
		for (i = 0; i < number_of_questions; i++)
			fscanf(IN, " %c", &(scores[i]));
		int grade = 0;
		for (i = 0; i < number_of_questions; i++)
		{
			if (scores[i] == key[i])
				grade++;
			else
			{
				if (scores[i] == ERROR_CODE_IN_INPUT_SCANNING)
					error = 1; /* mark this line as error in data file */
			}
		}
		int id1;
		char nm[128];
		FILE *IN1 = fopen(fname1, "r");
		if (!IN1)
		{
			printf("Error opening input file %s.\n", fname1);
			perror(fname);
			return;
		}
		while (!feof(IN1) && (fscanf(IN1, "%d", &(id1)) == 1))
		{
			fscanf(IN1, "%[^\n]s", nm);
			if (id == id1)
			{
				fprintf(IN2, "%05d,%s,", id, nm);
				for (i = 0; i < number_of_questions; i++)
					fprintf(IN2, "%c,", scores[i]);
				fprintf(IN2, "%d%%\n", 100 * (grade) / number_of_questions);
			}
		}
		close(IN1);
	}
	close(IN2);
}

/* Function just prints greeting message at the begin of the program */
void printGreetingMessage()
{
	printf("GRADING MERGE FILES PROGRAM WRITTEN IN C\n\n");
	printf("Program written by Mariana Pratsovyta.\n");
	printf("Program compiled on Oct 22, 2018 at 16:42:41.\n\n");
}
