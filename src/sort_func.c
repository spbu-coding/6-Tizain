 #include <stdlib.h>
 #include "sortings.h"


void bubble(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator_func) {

	char* tmp;
    char flag;
    do {
        flag = 0;
        for (size_t i = 1; i < array_size; i++) {
            if (comparator_func(strings_array[i-1],strings_array[i])>0) {
                tmp = strings_array[i];
                strings_array[i] = strings_array[i-1];
                strings_array[i-1] = tmp;
                flag = 1;
            }
        }
    } while (flag);
}

void insertion(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator_func) {

    char* tmp; 
	int	walk_var;
 
    for (size_t i = 1; i < array_size; i++)   {
        tmp = strings_array[i];
        walk_var = i - 1;
		while(walk_var >= 0 && comparator_func(strings_array[walk_var],tmp) > 0)
        {
            strings_array[walk_var+1] = strings_array[walk_var];
            walk_var = walk_var - 1;
        }
        strings_array[walk_var+1] = tmp;
    }
}

void merge(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator_func) {

    // ћетод восход€щего сли€ни€
    size_t step = 1;  // шаг разбиени€ последовательности
    char **temp = (char **) malloc(array_size * (MAX_INPUT_STRING_SIZE + 2)); // дополнительный массив
    for (size_t i = 0; i < array_size; i++) {
        temp[i] = (char *) malloc(MAX_INPUT_STRING_SIZE + 2);
    }

	while (step < array_size)  { // пока шаг меньше длины массива
    	size_t index = 0;    // индекс результирующего массива
    	size_t l = 0;      // лева€ граница участка
    	size_t m = l + step;  // середина участка
    	size_t r = l + step * 2;  // права€ граница участка
    	do	{
			m = m < array_size ? m : array_size;  // сортируемый участок не выходит за границы последовательности
      		r = r < array_size ? r : array_size;
      		size_t i1 = l, i2 = m; // индексы сравниваемых элементов
      		for (; i1 < m && i2 < r; ) { // пока i1 не дошЄл до середины и i2 не дошЄл до конца
				if (comparator_func(strings_array[i1],strings_array[i2])<0) 
					temp[index++] = strings_array[i1++];  // заполн€ем участок результирующей последовательности
	      		else  temp[index++] = strings_array[i2++]; 
      		}
      		// »ли i1 < m или i2 < r - только один из операторов while может выполнитьс€
      		while (i1 < m) temp[index++] = strings_array[i1++]; // заносим оставшиес€ элементы сортируемых участков
      		while (i2 < r) temp[index++] = strings_array[i2++]; // в результирующий массив
      		l += step * 2; // перемещаемс€ на следующий сортируемый участок
      		m += step * 2;
      		r += step * 2;
    	} while (l < array_size); // пока лева€ граница сортируемого участка - в пределах последовательности
    	for (size_t i = 0; i < array_size; i++) // переносим сформированный массив обратно в a
      		strings_array[i] = temp[i];
    	step *= 2; // увеличиваем в 2 раза шаг разбиени€
	}
    for (size_t i = 0; i < array_size; i++)  // запись отсортированных строк в файл
		free(temp[i]);
	free(temp);
}

void quick(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator_func) {

    int left = 0;     //”казатели в начало и в конец массива
	int right = array_size - 1; 
	char *mid, *temp;

    mid = strings_array[array_size / 2];  //÷ентральный элемент массива
    //ƒелим массив
    do {    //ѕробегаем элементы, ищем те, которые нужно перекинуть в другую часть
        		//¬ левой части массива пропускаем(оставл€ем на месте) элементы, которые меньше центрального
        while(comparator_func(strings_array[left],mid)<0) left++;
       			//¬ правой части пропускаем элементы, которые больше центрального
        while(comparator_func(strings_array[right],mid)>0) right--;

        if (left <= right) {     //ћен€ем элементы местами
            temp = strings_array[left];
            strings_array[left] = strings_array[right];
            strings_array[right] = temp;
            left++;
            right--;
        }
    } while (left <= right);
									//–екурсивные вызовы, если осталось, что сортировать
    if(right > 0)     //"Ћевый кусок"
		quick(strings_array, right + 1, comparator_func);
    if (left < (int)array_size)       //"ѕравый кусок"
		quick(&strings_array[left], array_size - left, comparator_func);
}

void radix(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator_func) {

    int addit_arr[array_size]; 
	int max_addit_arr = 0;
    for (unsigned int i = 0; i < array_size; i++) {
        addit_arr[i] = strlen(strings_array[i]) - 1;
        if (addit_arr[i] > max_addit_arr)  max_addit_arr = addit_arr[i];
	}
    for (int i = (int) max_addit_arr - 1; i >= 0; i--) {
        unsigned int pocket[256] = {0};
        for (unsigned int j = 0; j < array_size; j++) 
            if ((int) addit_arr[j] - 1 >= i) 
                pocket[(unsigned int) strings_array[j][i]]++;
            else pocket[0]++;
        
        if (comparator_func("a", "b") < 0)  // результат не нужен, служит дл€ определени€ переданной функции
            for (unsigned int j = 1; j < 256; j++) 
                pocket[j] += pocket[j - 1];
        else 
            for (int j = 256 - 2; j >= 0; j--) 
                pocket[j] += pocket[j + 1];

        char* str_res_arr[array_size];
        int int_res_arr[array_size];
        for (int j = (int) array_size - 1; j >= 0; j--) {
            if ((int) addit_arr[j] - 1 >= i) {
                str_res_arr[(pocket[(unsigned int) strings_array[j][i]]) - 1] = strings_array[j];
                int_res_arr[(pocket[(unsigned int) strings_array[j][i]]--) - 1] = addit_arr[j];
            } else {
                str_res_arr[(pocket[0]) - 1] = strings_array[j];
                int_res_arr[(pocket[0]--) - 1] = addit_arr[j];
            }
        }
        memcpy(strings_array, str_res_arr, array_size * sizeof(char*));
        memcpy(addit_arr, int_res_arr, array_size * sizeof(size_t));
    }
}
