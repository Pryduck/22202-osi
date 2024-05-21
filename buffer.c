#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void manipulateBuffer() {
    char* buffer = (char*)malloc(100 * sizeof(char));

    strcpy(buffer, "hello world");

    printf("Context of the buffer: %s\n", buffer);

    free(buffer);

    printf("Content of the buffer after deallocation: %s\n", buffer);

    char* newBuffer = (char*)malloc(100 * sizeof(char));

    strcpy(newBuffer, "hello world");

    printf("Content of the new buffer: %s\n", newBuffer);

//  здесь будет ошибка
/*  char* middlePointer = newBuffer + 50;
    free(middlePointer);
    printf("Content of the new buffer after middle deallocation: %s\n", newBuffer);
*/

    free(newBuffer);
}

int main() {
    manipulateBuffer();

    char *initial_value = getenv("var");
    printf("Initial value of var: %s\n", initial_value);

    // Изменяем значение переменной окружения
    setenv("var", "12", 1);//1 - перезаписано, если уже существует

    char *new_value = getenv("var");
    printf("New value of var: %s\n", new_value);
    return 0;
}
