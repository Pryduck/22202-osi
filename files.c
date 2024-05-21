#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "files.h"
#include <string.h>

// Функция для создания каталога
void createDirectory(char *directory_name) {
    mkdir(directory_name, 0777);
}

// Функция для вывода содержимого каталога
void listDirectoryContents(char *directory_name) {
    char command[100];
    sprintf(command, "ls -l %s", directory_name);
    system(command);
}

// Функция для удаления каталога
void deleteDirectory(char *directory_name) {
    rmdir(directory_name);
}

// Функция для создания файла
void createFile(char *file_name) {
    FILE *file = fopen(file_name, "w+");
    fclose(file);
}

// Функция для вывода содержимого файла
void displayFileContents(char *file_name) {
    char command[100];
    sprintf(command, "cat %s", file_name);
    system(command);
}

// Функция для удаления файла
void deleteFile(char *file_name) {
    remove(file_name);
}

// Функция для создания символьной ссылки
void createSymbolicLink(char *target_file, char *link_name) {
    symlink(target_file, link_name);
}

// Функция для вывода содержимого символьной ссылки
void displaySymbolicLinkContents(char *link_name) {
    char command[100];
    sprintf(command, "readlink %s", link_name);
    system(command);
}

// Функция для вывода содержимого файла, на который указывает символьная ссылка
void displayLinkedFileContents(char *link_name) {
    char command[100];
    sprintf(command, "cat $(readlink %s)", link_name);
    system(command);
}

// Функция для удаления символьной ссылки
void deleteSymbolicLink(char *link_name) {
    unlink(link_name);
}

// Функция для создания жесткой ссылки
void createHardLink(char *target_file, char *link_name) {
    link(target_file, link_name);
}

// Функция для удаления жесткой ссылки
void deleteHardLink(char *link_name) {
    unlink(link_name);
}

// Функция для вывода прав доступа к файлу и количества жестких ссылок на него
void displayFilePermissionsAndLinks(char *file_name) {
    struct stat file_stat;
    stat(file_name, &file_stat);

    printf("File Permissions: %o\n", file_stat.st_mode /*& 0777*/);
    printf("Number of Hard Links: %ld\n", file_stat.st_nlink);
}

// Функция для изменения прав доступа к файлу
void changeFilePermissions(char* file_name, char* permissions) {
    int rights = 0;
    for (int i = 0; i < 9; i += 3) {
        if (permissions[i] == 'r') {
            if (i == 0) {
                rights |= S_IRUSR;
            }
            else if (i == 3) {
                rights |= S_IRGRP;
            }
            else {
                rights |= S_IROTH;
            }
        }
        else if (permissions[i] != '-') {
            int k = i + 1;
            printf("incorrect symbol %d\n", k);
        }

        if (permissions[i + 1] == 'w') {
            if (i == 0) {
                rights |= S_IWUSR;
            }
            else if (i == 3) {
                rights |= S_IWGRP;
            }
            else {
                rights |= S_IWOTH;
            }
        }
        else if (permissions[i + 1] != '-') {
            int k = i + 2;
            printf("incorrect symbol %d\n", k);
        }

        if (permissions[i + 2] == 'x') {
            if (i == 0) {
                rights |= S_IXUSR;
            }
            else if (i == 3) {
                rights |= S_IXGRP;
            }
            else {
                rights |= S_IXOTH;
            }
        }
        else if (permissions[i + 2] != '-') {
            int k = i + 3;
            printf("incorrect symbol %d\n", k);
        }
    }
    chmod(file_name, rights);
}

int choose_action(char* line) {
    int lastSeparator = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == '/') {
            lastSeparator = i;
        }
     }

    if (lastSeparator > 0) {
        char name[strlen(line) - 1 - lastSeparator];
        strcpy(line, line + lastSeparator + 1);
    } else printf("wrong command|n");
    /*char *slash_ptr = strrchr(line, '/'); //вернёт указатель на  '/'
    if (slash_ptr != NULL) {
        line = slash_ptr + 1;
    }*/
    for (int i = 0; i < 14; i++) {
        if (strcmp(line, ind[i].line) == 0) {
            return ind[i].number;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <action> <argument>\n", argv[0]);
        return 1;
    }

    int num = choose_action(argv[0]);
    char *argument = argv[1];
  
    if (num == 0) {
        createDirectory(argument);
    } else if (num == 1) {
        listDirectoryContents(argument);
    } else if (num == 2) {
        deleteDirectory(argument);
    } else if (num == 3) {
        createFile(argument);
    } else if (num == 4) {
        displayFileContents(argument);
    } else if (num == 5) {
        deleteFile(argument);
    } else if (num == 6) {
        createSymbolicLink(argument, argv[2]);
    } else if (num == 7) {
        displaySymbolicLinkContents(argument);
    } else if (num == 8) {
        displayLinkedFileContents(argument);
    } else if (num == 9) {
        deleteSymbolicLink(argument);
    } else if (num == 10) {
        createHardLink(argument, argv[2]);
    } else if (num == 11) {
        deleteHardLink(argument);
    } else if (num == 12) {
        displayFilePermissionsAndLinks(argument);
    } else if (num == 13) {
        changeFilePermissions(argument, argv[2]);
    } else {
        printf("Invalid action specified.\n");
    }
    return 0;
}
