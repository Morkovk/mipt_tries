#include <stdio.h>
#include <stdlib.h>

#define stdout 1
#define stdin 0

void StringRealloc(char** str, int *ReservedSize, int UsedSize){
	if (*ReservedSize == UsedSize)
		*str = (char*)realloc((void*)(*str), sizeof(char) * *ReservedSize * 2);
}

void ArrayRealloc(int** arr, int *ReservedSize, int UsedSize){
	if (*ReservedSize == UsedSize)
		*arr = (int*)realloc((void*)(*arr), sizeof(char)* *ReservedSize * 2);
	*ReservedSize *= 2;
}


int CheckForTemplate(char * str, char* Template){
	int iT = 0;
	int iS = 0;
	//int LeftBracketFound;

	int FirstLetterOfTheRangeRead = 0;
	int DefRead = 0;
	int starRead = 0;
	int AskRead = 0;
	int subTemplateFound = 0;

	int* arrayOfRangeBegins;
	int Reserved1 = 0;
	int Used1 = 0;
	int* arrayOfRangeEnds;
	int Reserved2 = 0;
	int Used2 = 0;

	while (Template[iT] != '\0'){

		if (Template[iT] == '.'){
			if (str[iS] == '\0')
				return 0;
			iT++;
			iS++;
		}

		else if (Template[iT] == '['){

			FirstLetterOfTheRangeRead = 0;
			DefRead = 0;
			starRead = 0;
			AskRead = 0;
			subTemplateFound = 0;

			Reserved1 = 0;
			Used1 = 0;
			Reserved2 = 0;
			Used2 = 0;

			iT++;
			while (Template[iT] != ']'){
				if (!FirstLetterOfTheRangeRead){
					FirstLetterOfTheRangeRead = 1;
					ArrayRealloc(&arrayOfRangeBegins, &Reserved1, Used1);
					arrayOfRangeBegins[Used1] = (int)Template[iT];
					Used1++;
				}
				if (FirstLetterOfTheRangeRead && !DefRead && Template[iT] == '-'){
					iT++;
					DefRead = 1;
				}
				else if (FirstLetterOfTheRangeRead && !DefRead && Template[iT] != '-'){
					ArrayRealloc(&arrayOfRangeEnds, &Reserved2, Used2);
					arrayOfRangeBegins[Used2] = (int)Template[iT - 1];
					Used2++;

					ArrayRealloc(&arrayOfRangeBegins, &Reserved1, Used1);
					arrayOfRangeBegins[Used1] = (int)Template[iT];
					Used1++;
				}
				else if (FirstLetterOfTheRangeRead && DefRead){
					ArrayRealloc(&arrayOfRangeEnds, &Reserved2, Used2);
					arrayOfRangeBegins[Used2] = (int)Template[iT - 1];
					Used2++;
					FirstLetterOfTheRangeRead == 0;
				}
				else {
					printf("Wrong Template");
					break;
				}
			}
			if (FirstLetterOfTheRangeRead)
				printf("Wrong Template");
			arrayOfRangeBegins[Used1] = '\0';
			arrayOfRangeEnds[Used2] = '\0';
			iT++;
			if (Template[iT] == '*'){
				starRead = 1;
				iT++;
			}
			if (Template[iT] == '?'){
				AskRead = 1;
				iT++;
			}

			for (int i = 0; i < Used1; i++){
				if ((int)str[iS] >= arrayOfRangeBegins[i] && (int)str[iS] <= arrayOfRangeEnds[i]){
					subTemplateFound = 1;
					break;
				}
			}
			if (!AskRead && !starRead && !subTemplateFound)
				return 0;
		}

		else{
			if (str[iS] != Template[iT] && !starRead){
				return 0;
			}
			else if (str[iS] == Template[iT] && !starRead){
				iS++;
				iT++;
			}
			else if (str[iS] != Template[iT] && starRead){
				for (int i = 0; i < Used1; i++){
					if ((int)str[iS] >= arrayOfRangeBegins[i] && (int)str[iS] <= arrayOfRangeEnds[i]){
						subTemplateFound = 1;
						break;
					}
				}
				if (!subTemplateFound)
					return 0;
			}
		}
	}
	return 1;
}


void ReadFile(FILE* f, char* Template){
	char c;
	char *str = (char*)malloc(sizeof(char));
	int ReservedSize;
	int UsedSize;

	fread((void*)c, sizeof(char), 1, f);

	while (c != EOF){
		while (c != '\n'){
			str = (char*)malloc(sizeof(char));
			ReservedSize = 1;
			UsedSize = 0;
			StringRealloc(&str, &ReservedSize, UsedSize);
			str[UsedSize] = c;
			UsedSize++;
		}
		str[UsedSize] = '\0';
		if (CheckForTemplate(str, Template))
			printf("%s\n", str);
	}
}

int main(int argc, char **argv){
	for (int i = 2; i < argc; i++){
		if (argc <= 2){
			FILE *f = stdin;
			ReadFile(f, argv[1]);
			break;
		}
		else{
			FILE *f = fopen(argv[i], "r");
			if (!f)
				printf("file ¹ %d can not be opened", i);
			else
				ReadFile(f, argv[1]);
		}
	}
}