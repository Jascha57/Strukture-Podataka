#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CHAR (100)

struct _Stog;
struct _Stack;
typedef struct _Stack* StackPosition;
typedef struct _Stog* Position;
typedef struct _Stog
{

	double number;
	Position next;

}stog;

typedef struct _Stack
{
	int top;
	unsigned capacity;
	int* array;
}Stack;

//For the main task
int InsertAfter(Position position, Position newElement);
int Push(Position head, double number);
int Pop(Position head, double* result);
int MathOperation(Position head, char operation);
int GettingPostFixFromFile(double* result, char* filename);
void FreeMemory(Position head);
int DeleteAfter(Position position);

Position CreateStogElement(double number);


//Code I found that turns an infix into a postfix
//As i thought need to modify it for numbers because this is only for char strings. Oh well, gonna try it later.
int IsEmpty(StackPosition stack);
char Peek(StackPosition stack);
char Pop2(StackPosition stack);
void Push2(StackPosition stack, char operation);
int IsOperand(char ch);
int Prec(char ch);
int INtoPOST(char* fix);

StackPosition CreateStack(unsigned capacity);


int main()
{
	int status = 0;
	double result = 0;
	char fileName[MAX_CHAR] = { "" };
	char fix[] = "23 * 3 + 23 - 8 * 2";

	printf("Upisite ime datoteke\n");
	scanf(" %s", fileName);

	status = GettingPostFixFromFile(&result, fileName);
	if (status == EXIT_SUCCESS)
	{
		printf("Result is:%lf\n", result);
		INtoPOST(fix);
		return 0;
	}

	return 0;
}



int InsertAfter(Position position, Position newElement)
{
	newElement->next = position->next;
	position->next = newElement;

	return EXIT_SUCCESS;
}

int Push(Position head, double number)
{
	Position newElement = NULL;
	newElement = CreateStogElement(number);

	if (!newElement)
	{
		return EXIT_FAILURE;
	}

	InsertAfter(head, newElement);

	return EXIT_SUCCESS;
}
int Pop(Position head, double* result)
{
	Position first = head->next;
	if (!first)
	{
		perror("Greska u Pop\n");
		return EXIT_FAILURE;
	}

	*result = first->number;
	DeleteAfter(head);

	return EXIT_SUCCESS;
}
int MathOperation(Position head, char operation)
{
	double operand1 = 0;
	double operand2 = 0;
	int status1 = 0;
	int status2 = 0;
	double result = 0;

	status2 = Pop(head, &operand2);
	if (status2 != EXIT_SUCCESS)
	{
		perror("Status2 failed\n");
		return EXIT_FAILURE;
	}

	status1 = Pop(head, &operand1);
	if (status1 != EXIT_SUCCESS)
	{
		perror("Status2 failed\n");
		return EXIT_FAILURE;
	}

	switch (operation)
	{
	case'+':
		result = operand1 + operand2;
		break;

	case'-':
		result = operand1 - operand2;
		break;

	case'*':
		result = operand1 * operand2;
		break;

	case'/':
		if (operand2 == 0)
		{
			printf("Look up DividingByZero/SlimPickensDoesTheRightThingAndRidesTheBombToHell\n");
			return EXIT_FAILURE;
		}
		else
		{
			result = operand1 / operand2;
		}
		break;

	default:
		printf("Operation not avaliable. Please contact customer support at Microsoft\n");
		return EXIT_FAILURE;
		break;
	}

	return Push(head, result);
}
int GettingPostFixFromFile(double* result, char* filename)
{
	stog head = { .number = 0, .next = NULL };
	FILE* f = NULL;
	char* buffer = NULL;
	char* pBuffer = NULL;
	char operation = 0;

	int fileSize = 0;
	int status = 0;
	int bytes = 0;
	double number = 0;

	f = fopen(filename, "rb");
	if (!f)
	{
		perror("Ne postoji datoteka\n");
		return EXIT_SUCCESS;
	}

	fseek(f, 0, SEEK_END);
	fileSize = ftell(f);

	buffer = (char*)calloc(fileSize + 1, sizeof(char));
	if (!buffer)
	{
		perror("Buffer nije alociran\n");
		fclose(f);
		return EXIT_FAILURE;
	}

	rewind(f);
	fread(buffer, sizeof(char), fileSize, f);
	printf("|%s|\n", buffer);
	fclose(f);

	INtoPOST(buffer);
	pBuffer = buffer;
	while (strlen(pBuffer) != 0)
	{
		status = sscanf(pBuffer, " %lf %n", &number, &bytes);
		if (status == 1)
		{
			Push(&head, number);
			pBuffer += bytes;
		}
		else
		{
			sscanf(pBuffer, " %c %n", &operation, &bytes);
			status = MathOperation(&head, operation);
			if (status != EXIT_SUCCESS)
			{
				perror("Postfix is garbage\n");
				free(buffer);
				FreeMemory(&head);
				return EXIT_FAILURE;
			}
			pBuffer += bytes;
		}
	}

	free(buffer);

	status = Pop(&head, result);
	if (status != EXIT_SUCCESS)
	{
		perror("Garbage\n");
		return EXIT_FAILURE;
	}
	
	FreeMemory(&head);
	
	return EXIT_SUCCESS;
}
void FreeMemory(Position head)
{
	Position temp = NULL;

	if (head->next != NULL)
	{
		temp = head->next;
		if (NULL == temp->next) {
			free(temp);
		}
		else {
			while (head->next != NULL) {
				temp = head->next;
				head->next = temp->next;
				free(temp);
			}
		}
	}
}
int DeleteAfter(Position position)
{
	Position temp = position->next;
	if (!temp)
	{
		perror("Greska u deleteafter\n");
		return -1;
	}

	position->next = temp->next;
	free(temp);
	return 0;
}

Position CreateStogElement(double number)
{
	Position newElement = NULL;
	newElement = (Position)malloc(sizeof(stog));

	if (!newElement)
	{
		perror("Greska u alokaciji memorije\n");
		return NULL;
	}

	newElement->number = number;
	newElement->next = NULL;

	return newElement;
}

//Here starts plagirism


int IsEmpty(StackPosition stack)
{
	//Cheking if the stack is empty, pretty self explanitory
	return stack->top == -1;
}
char Peek(StackPosition stack)
{
	return stack->array[stack->top];
}
char Pop2(StackPosition stack)
{
	//From the name, obviously does the Pop function
	if (!IsEmpty(stack))
	{
		return stack->array[stack->top--];
	}
	return '$';
}
void Push2(StackPosition stack, char operation)
{
	//From the name, obviously does the Push function
	stack->array[++stack->top] = operation;
}
int IsOperand(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}
int Prec(char ch)
{
	//Function that gives operations priority
	switch (ch)
	{
	case'+':
	case'-':
		return 1;
	case'*':
	case'/':
		return 2;
	case'^':
		return 3;
	}
	return 1;
}
int INtoPOST(char* fix)
{
	int i = 0;
	int j = 0;

	StackPosition stack = CreateStack(strlen(fix));
	if (!stack)
	{
		printf("Allocation of stack failed\n");
		return EXIT_FAILURE;
	}

	for (i = 0, j = -1; fix[i]; ++i)
	{
		//If it is an operand add it to output
		if (IsOperand(fix[i]))
		{
			fix[++j] = fix[i];
		}
		//Here if it is a ( then push it to stack and it is goint to await a ) to pop it
		else if (fix[i] == '(')
		{
			Push2(stack, fix[i]);
		}
		else if (fix[i] == ')')
		{
			while (!IsEmpty(stack) && Peek(stack) != '(')
			{
				fix[++j] = Pop2(stack);
			}
			if (!IsEmpty(stack) && Peek(stack) != '(')
			{
				printf("Invalid fixture\n");
				return EXIT_FAILURE;
			}
			else
			{
				Pop2(stack);
			}
		}
		//Final else when we encounter an operation
		else
		{
			while (!IsEmpty(stack) && Prec(fix[i] <= Prec(Peek(stack))))
			{
				fix[++j] = Pop2(stack);
			}
			Push2(stack, fix[i]);
		}
	}
	while (!IsEmpty(stack))
	{
		fix[++j] = Pop2(stack);
	}

	fix[++j] = '\0';
	printf("%s\n", fix);
	return EXIT_SUCCESS;
}


//Creating a new stack for sorting just like the original for the task where it is already postfix written
StackPosition CreateStack(unsigned capacity)
{
	StackPosition stack = (StackPosition)malloc(sizeof(Stack));

	if (!stack)
	{
		printf("Cheking stack failed\n");
		return NULL;
	}

	stack->top = -1;
	stack->capacity = capacity;
	stack->array = (int*)malloc(stack->capacity * sizeof(int));

	return stack;
};
