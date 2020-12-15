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

    // ����� ����������� �������
    size_t step = 1;  // ��� ��������� ������������������
    char **temp = (char **) malloc(array_size * (MAX_INPUT_STRING_SIZE + 2)); // �������������� ������
    for (size_t i = 0; i < array_size; i++) {
        temp[i] = (char *) malloc(MAX_INPUT_STRING_SIZE + 2);
    }

	while (step < array_size)  { // ���� ��� ������ ����� �������
    	size_t index = 0;    // ������ ��������������� �������
    	size_t l = 0;      // ����� ������� �������
    	size_t m = l + step;  // �������� �������
    	size_t r = l + step * 2;  // ������ ������� �������
    	do	{
			m = m < array_size ? m : array_size;  // ����������� ������� �� ������� �� ������� ������������������
      		r = r < array_size ? r : array_size;
      		size_t i1 = l, i2 = m; // ������� ������������ ���������
      		for (; i1 < m && i2 < r; ) { // ���� i1 �� ����� �� �������� � i2 �� ����� �� �����
				if (comparator_func(strings_array[i1],strings_array[i2])<0) 
					temp[index++] = strings_array[i1++];  // ��������� ������� �������������� ������������������
	      		else  temp[index++] = strings_array[i2++]; 
      		}
      		// ��� i1 < m ��� i2 < r - ������ ���� �� ���������� while ����� �����������
      		while (i1 < m) temp[index++] = strings_array[i1++]; // ������� ���������� �������� ����������� ��������
      		while (i2 < r) temp[index++] = strings_array[i2++]; // � �������������� ������
      		l += step * 2; // ������������ �� ��������� ����������� �������
      		m += step * 2;
      		r += step * 2;
    	} while (l < array_size); // ���� ����� ������� ������������ ������� - � �������� ������������������
    	for (size_t i = 0; i < array_size; i++) // ��������� �������������� ������ ������� � a
      		strings_array[i] = temp[i];
    	step *= 2; // ����������� � 2 ���� ��� ���������
	}
    for (size_t i = 0; i < array_size; i++)  // ������ ��������������� ����� � ����
		free(temp[i]);
	free(temp);
}

void quick(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator_func) {

    int left = 0;     //��������� � ������ � � ����� �������
	int right = array_size - 1; 
	char *mid, *temp;

    mid = strings_array[array_size / 2];  //����������� ������� �������
    //����� ������
    do {    //��������� ��������, ���� ��, ������� ����� ���������� � ������ �����
        		//� ����� ����� ������� ����������(��������� �� �����) ��������, ������� ������ ������������
        while(comparator_func(strings_array[left],mid)<0) left++;
       			//� ������ ����� ���������� ��������, ������� ������ ������������
        while(comparator_func(strings_array[right],mid)>0) right--;

        if (left <= right) {     //������ �������� �������
            temp = strings_array[left];
            strings_array[left] = strings_array[right];
            strings_array[right] = temp;
            left++;
            right--;
        }
    } while (left <= right);
									//����������� ������, ���� ��������, ��� �����������
    if(right > 0)     //"����� �����"
		quick(strings_array, right + 1, comparator_func);
    if (left < (int)array_size)       //"������ �����"
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
        
        if (comparator_func("a", "b") < 0)  // ��������� �� �����, ������ ��� ����������� ���������� �������
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
