#include <stdio.h>
#include <unistd.h>
// Глобальные переменные
int globIn = 10;
int globUnin;
const int globC = 5;

void localVar() {
    // Локальные переменные
    int loc = 20;
    static int stat = 30;
    const int con = 15;

    printf("Адрес локальной переменной: %p\n", &loc);
    printf("Адрес статической переменной: %p\n", &stat);
    printf("Адрес константы: %p\n", &con);
}

int* createAndInitializeVariable() {
    int localVar = 42;
    int* ptr = &localVar;
    return ptr;
}

int main() {
    localVar();

    printf("Адрес глобальной инициализированной переменной: %p\n", &globIn);
    printf("Адрес глобальной неинициализированной переменной: %p\n", &globUnin);
    printf("Адрес глобальной константы: %p\n", &globC);
    printf("номер процесса %d\n", getpid());
    sleep(30);

    int* resultPtr = createAndInitializeVariable();
    printf("Адрес локальной переменной: %p\n", (void*)resultPtr);
    int y = *resultPtr;
    printf("%d\n", y);//всё пройдёт успешно

    return 0;
}
