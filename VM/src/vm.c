// main.c
//

#include <stdio.h>
#include <stdlib.h>

#include "../include/pMachine.h"

int main()
{
	// allocate memory for 3 integers
	int *cIc = malloc(3 * sizeof(int));

	// enter file path as string (char array cuz C is as old as dinosaurs)
	char *filePath = "../input.txt";

	// create filepointer and open file - check for NULL pointer
	FILE *fileP = fopen(filePath, "r");
	if (fileP == NULL)
	{
		printf("File Not Found");
		return 0;
	}

	while (fscanf(fileP, "%d %d %d", &cIc[0], &cIc[1], &cIc[2]) != EOF)
	{
		// print the values read from the file
		printf("Read Values: %d %d %d\n", cIc[0], cIc[1], cIc[2]);
	}

	// Close the file
	fclose(fileP);

	return 0;
}
