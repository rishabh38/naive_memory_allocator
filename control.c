#include "libmalloc.h"

#include <stdio.h>
#include <string.h>
#define SIZE 10

int main() {
    char message[] = "Caravan";
    char *str = (char *)esmalloc(sizeof(char)*SIZE);
    
    
    printf("str: %p\n", str);
    printf("assigning data to str\n");
    strncpy (str, message, strlen(message));
    
    printf("assigned data: %s\n", str);
    printf("assigned data: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%c", str[i]);
    }
    printf("\n");
    esfree(str);
}
