#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>

#define PAGE_SIZE 4096 

int main() {
    // Создаём общий регион памяти
    unsigned int* shared_memory = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Дочерний, запись чисел
        unsigned int i = 0;
        while (1) {
            for (int j = 0; j < PAGE_SIZE / sizeof(unsigned int); ++j) {
                shared_memory[j] = i++;
            }
        }
    } else {
        // Родительский, чтение и проверка
        unsigned int expected = 0;
        while (1) {
            for (int j = 0; j < PAGE_SIZE / sizeof(unsigned int); ++j) {
                if (shared_memory[j] != expected) {
                    printf("Ошибка: ожидалось %u, получено %u в позиции %d\n", expected, shared_memory[j], j);
                }
                expected++;
            }
        }
    }

    return 0;
}
