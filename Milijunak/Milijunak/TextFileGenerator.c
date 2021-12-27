#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>





int main()
{
	//Just for generating files
	/*char Buffer[1024];
	for (int i = 0; i < 100; i++)
	{
		sprintf(Buffer, "question%d.txt", i);
		FILE* file = fopen(Buffer, "w");
		fclose(file);
	}*/
	//For checking what questions i already have
	char buffer[1024] = "";
	FILE* foutput = fopen("Pitanja.txt", "w");

	for (int i = 0; i < 100; i++)
	{
		sprintf(buffer, "question%d.txt", i);
		FILE* f = fopen(buffer, "r");
		fgets(buffer, 1024, f);
		fprintf(foutput, "%d: %s", i, buffer);
		fclose(f);
	}
	fclose(foutput);
	return 0;
}