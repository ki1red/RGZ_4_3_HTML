#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


long GetSize(const char nameFile[])
{
    FILE* file = fopen(nameFile, "r+");
   
    if (file == NULL) return -1;

    long size;
    fseek(file, 1, SEEK_END);
    size = ftell(file);
    
    fclose(file);
    return size;
}

const char* EditNameFile(const char nameFile [])
{
    // Узнаем длину имени фала
    int size = strlen(nameFile);
    // Создаём и массив и копируем в него строку
    char* name = (char*)malloc((size+1)*sizeof(char)); 
    strcpy(name,nameFile);
    
    // Ищем, где заканчивается имя
    int i;
    for (i = size - 1; i >= 0; i--)
        if (name[i] == '/')
        {
            i++;
            break;
        }
    // Если названия файла содeржит только его имя (без пути к файлу)
    if (i == -1)i = 0;
        //return nameFile;

    // Создаём массив только под имя (без пути к файлу)
    char *newName = (char*)malloc((size - i +5+1)); // 5 - "html/", size-i - "имя.html", 1 - '\0'
    int j;
    for (j = 5; i < size; i++, j++)
        newName[j] = name[i];
    
    free(name);

    newName[0] = 'h'; newName[1] = 't'; newName[2] = 'm'; newName[3] = 'l'; newName[4] = '/';
    newName[j] = '\0';

    
    const char* newNameFile = newName;
    //free(newName);

    return newNameFile;
}

void rec(const char nameFile[])
{
    // узнаем размер файла
    long size = GetSize(nameFile);
    if (size <=1) return;
    // создаем массив со страницы
    char* page = (char*)malloc(size);
    // открываем файл
    FILE* file = fopen(nameFile, "r+");
    // Копируем весь текст с файла
    int i=0;
    while ((page[i] = fgetc(file)) != EOF)
        i++;
    page[i] = '\0';
    // закрываем файл
    fclose(file);
    // сохраняем текст в файл в каталог
    FILE* fileCreate = fopen(EditNameFile(nameFile), "w+");
    fwrite(page,sizeof(char),i,fileCreate);
    fclose(fileCreate);
    
    // Ищем гиперссылки
    for (int j = 0; j < i; j++)
    {
        // Если наткнулись на первую букву
        if (page[j] == 'h' && (i - j >= 8))
            // Если обнаружили слово href
            if (page[j + 1] == 'r' && page[j + 2] == 'e' && page[j + 3] == 'f')
            {
                j += 3;
                // Начинаем искать начало гиперссылки
                while (page[j] != '"' && j<i)
                    j++;

                // Резко кончился файл
                if (j == i)break;

                j++;
                int g=j;
                
                // Считаем кол-во символов в гиперссылке
                while (page[g] != '"') 
                { 
                    g++; 
                }
                
                // Если в гиперссылке символ #, то пропускаем её
                if (g - j == 1 && page[g - 1] == '#')
                {
                    j = g + 1;
                    continue;
                }

                // Создаём массив под рамзер гиперссылки
                char* hyperlink = (char*)malloc(g-j+1);
                
                // Копируем гиперссылку
                g = 0;
                for (; page[j] != '"'; j++,g++)
                    hyperlink[g] = page[j];
                hyperlink[g] = '\0';

                // Делаем копию гиперссылки в константный массив char
                // для корректного очищения памяти из под hyperlink
                //const char* hl = hyperlink;

                // Запускаем гиперссылку
                rec(hyperlink);
                free(hyperlink);

                // продумать названия для файлов
                // очистить hyperlink
                // найти ошибку с \0 при копировании страницы
            }
    }

    // удаляем массив
    free(page);
}

int main()
{
    printf("Hello World!\n");
    
    //GetSize("s.html");
    rec("s.html");
    return 0;
}
