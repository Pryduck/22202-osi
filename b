#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global_var = 5; 

int main() {
    int local_var = 10;
    
    printf("Адрес и содержимое глобальной переменной: %p = %d\n", &global_var, global_var);
    printf("Адрес и содержимое локальной переменной: %p = %d\n", &local_var, local_var);
    
    printf("PID родительского процесса: %d\n", getpid());
    
    pid_t pid = fork();
    
    if (pid == -1) {
        // Ошибка при создании процесса
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // дочерний
        printf("PID дочернего процесса: %d, Parent PID: %d\n", getpid(), getppid());
        exit(5);
    } else {
        // родительский
        printf("Родительский процесс продолжает работать...\n");
        sleep(60); // Задержка достаточно долгая, чтобы дочерний процесс завершился и стал зомби
        printf("Родительский процесс завершает работу...\n");
        // Здесь нет вызова wait(), дочерний процесс становится зомби
    }
    
    return 0;
}
