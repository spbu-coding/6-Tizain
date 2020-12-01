 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "sortings.h"

 #define  MAX_INPUT_STRING_NUMBER 100

int console_read(int argc, char* argv[], char **infile, char **outfile, char **sposob_comp, char **sposob_sort, int *str_num) {  // функция обработки команды с консоли
	char* p;    // блок проверки второго элемента командной строки на число и максимальное кол-во строк
	long converted = strtol(argv[1], &p, 10);
	if (*p)  // значит это было не число
		*str_num = -1;	
	else    
		*str_num = (int)converted; 
	if (converted > MAX_INPUT_STRING_NUMBER) *str_num = -2;
								// распознавание команд пользователя
	for (int i = 2; i < argc; i++) {
		char *last_four = "nope";
		int len = strlen(argv[i]);
		if (len > 4) last_four = &argv[i][len-4];
		if (strcmp (argv[i], "asc")==0)	*sposob_comp = "asc";
		if (strcmp (argv[i], "des")==0)	*sposob_comp = "des";
		if (strcmp (argv[i], "bubble")==0)	*sposob_sort = "bubble";
		if (strcmp (argv[i], "insertion")==0)	*sposob_sort = "insertion";
		if (strcmp (argv[i], "merge")==0)	*sposob_sort = "merge";
		if (strcmp (argv[i], "quick")==0)	*sposob_sort = "quick";
		if (strcmp (argv[i], "radix")==0)	*sposob_sort = "radix";
		if (strcmp (last_four, ".txt") == 0)	
			if (*infile == "in") *infile = argv[i];
			else *outfile = argv[i];
	}
								// и обработка возможных ошибок
	if (*str_num < -2) printf("Its a negative value in the number of strings parameter\n"); 
	switch (*str_num) {
		case -2:
			printf("There is too much strings to sort\n"); 	 
  			break;
		case -1:
			printf("There is not a number in the string number parameter\n"); 
  			break;
		case 0:
			printf("0 strings are allready done\n"); 
  			break;
		case 1:
			printf("It needs 2 or more strings\n"); 
  			break;
	}

	if (*sposob_comp == "no") printf("The comparator name is needed\n"); 
	if (*sposob_sort == "no") printf("The sorting name is needed\n"); 	
	if (*infile == "in" || *outfile == "out") printf("The names of input and output text files are needed\n"); 
	if (*infile == "in" || *outfile == "out" || *sposob_comp == "no" || *sposob_sort == "no" || *str_num < 2) return -1;

	return 0;
}

int comparator_func_asc(const char* s1, const char* s2)  { // функция сравнения элементов массива на возрастание
	return strcmp(s1, s2);
}

int comparator_func_des(const char* s1, const char* s2)  { // функция сравнения элементов массива на убывание
	return strcmp(s2, s1);
}


int main(int argc, char* argv[]) {

	if (argc != 6) {
		printf("The needed command format: strings_comparer 3 input.txt output.txt bubble des\n"); 		
		return -1;
	}

	FILE *inptr, *outptr;
	char *infile = "in";
	char *outfile = "out"; 
	char *sposob_comp = "no";
	char *sposob_sort = "no";
	int str_num = 0;
								// Чтение команды ввода с консоли и обработка ошибок в этой команде 
	if (console_read(argc, argv, &infile, &outfile, &sposob_comp, &sposob_sort, &str_num) == -1)  
			return -1;   

    inptr = fopen(infile, "r");   // Открытие 1 файла на чтение
    if (inptr == NULL) {
        printf("Could not open %s.\n", infile);
        return 0;
    }
					// выделение памяти для массива, который будет содержать строки из файла и заполнение его
    char **strings_array = (char**)malloc(str_num * (MAX_INPUT_STRING_SIZE + 2));
	for (int i = 0; i < str_num; i++) {
		strings_array[i] = (char*)malloc(MAX_INPUT_STRING_SIZE + 2);
        fgets(strings_array[i], MAX_INPUT_STRING_SIZE + 2, inptr); //+2 (fgets)символы новой и нулевой строки
	}

    fclose(inptr);
								   			// вызов нужной функции сортировки
	comparator_func_t comparator_func = comparator_func_asc;
	if (sposob_comp == "des") comparator_func = comparator_func_des;

	if (sposob_sort == "bubble") bubble(strings_array, str_num, comparator_func); 
	if (sposob_sort == "insertion") insertion(strings_array, str_num, comparator_func); 
	if (sposob_sort == "merge") merge(strings_array, str_num, comparator_func); 
	if (sposob_sort == "quick") quick(strings_array, str_num, comparator_func); 
	if (sposob_sort == "radix") radix(strings_array, str_num, comparator_func); 

	outptr = fopen(outfile, "w");   // Открытие 2 файла на запись
    if (outptr == NULL) {
        printf("Could not open %s.\n", outfile);
        return 0;
    }

	int nul_char = 0;  // проверка наличия символа новой строки в последней строке массива 
	if (strchr (strings_array[str_num-1],'\n') != 0) nul_char = 1;

    for (int i = 0; i < str_num; i++)  // запись отсортированных строк в файл
    {
        fputs(strings_array[i], outptr);
        free(strings_array[i]);
    }
	if (nul_char == 0) fputs("\n", outptr); // запись символа новой строки в файл, если нужен

    fclose(outptr);
    free(strings_array);	

	return 0;
}
