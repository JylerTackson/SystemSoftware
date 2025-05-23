//main.c
//

#include <stdio.h>
#include <stdlib.h>

int main(){

	//enter file path as string (char array cuz C is as old as dinosaurs)
	char* filePath = "../input.txt";
	
	//create array to receive file values (i i i)
	char fileValues[5];

	//create filepointer and open file - check for NULL pointer
	FILE *fileP = fopen(filePath, "r");
	if(fileP == NULL){
		printf("File Not Found");
		return 0;
	}
	
	//Get file contents, loops until the end of the file.
	while(fgets(fileValues, 5, fileP)){
		printf("%s",&fileValues[0]);
	}

	fclose(fileP);





return 0;
}



