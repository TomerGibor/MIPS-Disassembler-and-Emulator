#include <stdio.h>

int add(int a, int b){
    int temp = a;
    a = b;
    b = temp;
    printf("a=%d, b=%d, a+b=%d\n", a, b, a+b);
    return a+b;
}

int main(){
    int i = 0;
    char* strs[] = {"hello", "world", "!"};

    for (i = 0; i < 3; i++)
        printf("i: %d, str: %s, 0x%p\n", i, strs[i], &strs[i]);

    i = add(10, 5);
    printf("Sum: %d\n", i);

	return 0;
}
