#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int main() {
    int fd[2]; // Файловые дескрипторы для pipe

    // Создание pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Дочерний, запись в pipe
        close(fd[0]); // Закрываем неиспользуемый конец

        for (unsigned int i = 0; i < UINT_MAX; ++i) {
            if (write(fd[1], &i, sizeof(i)) != sizeof(i)) {
                perror("write");
                close(fd[1]);
                return 1;
            }
        }

        close(fd[1]); // Закрываем дескриптор после завершения записи
    } else {
        // Родительский: чтение и проверка из pipe
        close(fd[1]); // Закрываем неиспользуемый конец

        unsigned int num, expected = 0;
        while (read(fd[0], &num, sizeof(num)) == sizeof(num)) {
            if (num != expected) {
                printf("Ошибка: ожидалось %u, получено %u\n", expected, num);
            }
            expected++;
        }

        close(fd[0]); // Закрываем дескриптор после завершения чтения
    }

    return 0;
}
