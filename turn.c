#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

const int BIG_NUMB = 512;

void reverseString(char *str, size_t length) {
    int i = 0;
    int j = length - 1;
    char temp;

    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

size_t lastSep(char* str) {

    size_t prevPos = -1, lastPos = -1, i;

    for (i = 0; str[i] != '\0'; ++i) {
        if (str[i] == '/') {
            prevPos = lastPos;
            lastPos = i;
        }
    }

    if (lastPos == (i - 1))
        return prevPos;

    return lastPos;
}

void copyFile(const char *sourcePath, const char *targetPath) {
    FILE *source = fopen(sourcePath, "rb");
    FILE *target = fopen(targetPath, "wb");

    if (source == NULL || target == NULL) {
        perror("Error opening file");
        return;
}

    fseek(source, -2, SEEK_END);//изменяем позицию внутреннего указателя в файле на конец до \0
    long pos = ftell(source);//возвращаем значение указателя файла = отстоящей длине
    char c[BIG_NUMB];
    while (pos >= 0) {
        fseek(source, pos, SEEK_SET);//переместить указатель чтения на текущую позицию
        fread(c, sizeof(char), 1, source);
        fwrite(c, sizeof(char), 1, target);
        pos--;
    }

}

void createDir(const char *sourceStr) {

    size_t strLength = strlen(sourceStr);

    char sourceDir[strLength + 257];
    strcpy(sourceDir, sourceStr);
    if (sourceDir[strLength - 1] != '/') {
        sourceDir[strLength++] = '/';
        sourceDir[strLength] = '\0';
    }

    char targetDir[256 * 2 + 1];
    size_t necDir = 1 + lastSep(sourceDir);
    size_t necDirSize = strLength - necDir;
    strcpy(targetDir, sourceDir + necDir);
    reverseString(targetDir, necDirSize - 1);


    if (mkdir(targetDir, 0777) == -1) {
        return 1;
    }

    DIR *dir = opendir(sourceDir);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    struct stat fileStat;
    //возвращает указатель на dirent, NULL - если дойдём до последнего
    while ((entry = readdir(dir)) != NULL) {
        char filePath[256];
        sprintf(filePath, "%s/%s", sourceDir, entry->d_name);

        if (stat(filePath, &fileStat) == -1) {
            perror("Error getting file stat");
            continue;
        }

        if (S_ISREG(fileStat.st_mode) && fileStat.st_size > 0) { //S_ISREG = TRUE если файл регулярный
            reverseString(entry->d_name, strlen(entry->d_name)); //перевернули название
            char targetPath[256];
            sprintf(targetPath, "%s/%s", targetDir, entry->d_name);
            copyFile(filePath, targetPath);
        }//часть для рекурсии:
            else if(S_ISDIR(fileStat.st_mode) && fileStat.st_size > 0) {

            if (sourceStr[strlen(sourceStr) - 1] == '/') {
                char insideDir[strlen(sourceStr) + strlen(entry->d_name)];
                strcpy(insideDir, sourceStr);
                strcpy(insideDir + strlen(sourceStr), entry->d_name);
                createDir(insideDir);
            } else {
                char insideDir[strLength + strlen(entry->d_name) + 1];
                strcpy(insideDir, sourceStr);
                insideDir[strlen(sourceStr)] = '/';
                strcpy(insideDir + strlen(sourceStr) + 1, entry->d_name);
                createDir(insideDir);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong arguments count\n");
        return 1;
    }

    char *sourceStr = argv[1];
    createDir(sourceStr);

    return 0;
}
