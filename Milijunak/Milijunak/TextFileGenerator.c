#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>





int main()
{
	char Buffer[1024];
	for (int i = 0; i < 100; i++)
	{
		sprintf(Buffer, "question%d.txt", i);
		FILE* file = fopen(Buffer, "w");
		fclose(file);
	}

	return 0;
}