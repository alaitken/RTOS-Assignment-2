#include <stdio.h>
#include "rtos-alloc.h"

typedef struct student {
	int student_num;
} student;

int main() {
    printf("Running main!");
    student* austin = rtos_malloc(sizeof(student));
    austin->student_num = 1;
    printf("Austin Aitken: %d", austin->student_num);
    return 0;
}