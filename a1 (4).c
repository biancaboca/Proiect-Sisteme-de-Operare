#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>

typedef struct section_header
{
    char SECT_NAME[21];
    int SECT_TYPE;
    int SECT_OFFSET;
    int SECT_SIZE;

} section_header;

int parse(const char *path)
{

    int fd1;
    int version = 0;
    char no_of_sections = '1';
    int header_size = 0;
    char magic[3];
    magic[2]=0;
    static int valid = 0;
    int s = 1;

    fd1 = open(path, O_RDONLY);

    if (fd1 == -1)
    {
        printf("ERROARE LA DESCHIDERE");
        return -2;
    }

    lseek(fd1, -2, SEEK_END);
    read(fd1, magic, 2);
    magic[2] = '\0';
    lseek(fd1, -4, SEEK_END);
    read(fd1, &header_size, 2);
    lseek(fd1, -header_size, SEEK_END);
    read(fd1, &version, 1);
    read(fd1, &no_of_sections, 1);

    if (strcmp(magic, "Wv") != 0)
    {
        valid = 1;
    }

    if ((version < 70 || version > 170))
    {
        valid = 2;
    }

    if ((no_of_sections < 2 || no_of_sections > 20))
    {
        valid = 3;
    }

    section_header *header;
    header = (section_header *)malloc(no_of_sections * sizeof(section_header));

    for (int parcurgere = 0; s && parcurgere < no_of_sections; parcurgere++)
    {
        read(fd1, &header[parcurgere].SECT_NAME, 20);
        read(fd1, &header[parcurgere].SECT_TYPE, 1);
        read(fd1, &header[parcurgere].SECT_OFFSET, 4);
        read(fd1, &header[parcurgere].SECT_SIZE, 4);
        header[parcurgere].SECT_NAME[20] = '\0';

        if (header[parcurgere].SECT_TYPE != 43 && header[parcurgere].SECT_TYPE != 53 && header[parcurgere].SECT_TYPE != 12 && header[parcurgere].SECT_TYPE != 72 && header[parcurgere].SECT_TYPE != 76 && header[parcurgere].SECT_TYPE != 16 && header[parcurgere].SECT_TYPE != 64)
        {
            valid = 4;
            s = 0;
        }
    }
    if (valid == 0)
    {
        // printf("%d",header->SECT_TYPE);
        printf("SUCCESS\n");
        printf("version=%d\n", version);
        printf("nr_sections=%d\n", no_of_sections);

        for (int parcurgere = 0; parcurgere < no_of_sections; parcurgere++)
        {

            printf("section%d: %s %d %d\n", parcurgere + 1, header[parcurgere].SECT_NAME, header[parcurgere].SECT_TYPE, header[parcurgere].SECT_SIZE);
        }
    }
    else
    {
        printf("ERROR\n");
        if (valid == 1)
        {
            printf("wrong magic\n");
        }
        else if (valid == 2)
        {
            printf("wrong version\n");
        }
        else if (valid == 3)
        {
            printf("wrong sect_nr\n");
        }
        else if (valid == 4)

            printf("wrong sect_types");
    }

    free(header);
    return 0;
}

int validationSF(const char *path)
{
    int fd1;
    int version = 0;
    char no_of_sections = '1';
    int header_size = 0;
    char magic[3];
    // static int valid=0;
    //  int s=1;

    fd1 = open(path, O_RDONLY);

    if (fd1 == -1)
    {
        // printf("ERROARE LA DESCHIDERE");
        return -2;
    }

    lseek(fd1, -2, SEEK_END);
    read(fd1, magic, 2);
    magic[2] = '\0';
    lseek(fd1, -4, SEEK_END);
    read(fd1, &header_size, 2);
    lseek(fd1, -1 * header_size, SEEK_END);
    read(fd1, &version, 1);
    read(fd1, &no_of_sections, 1);

    if (strcmp(magic, "Wv") != 0)
    {
        close(fd1);
        return -1;
    }
    // printf("magic:%s size:%d version:%d sections%d\n",magic,header_size,version,no_of_sections);
    if ((version < 70 || version > 170))
    {
        close(fd1);
        return -1;
    }

    if ((no_of_sections < 2 || no_of_sections > 20))
    {
        close(fd1);
        return -1;
    }

    section_header *header;
    header = (section_header *)malloc(no_of_sections * sizeof(section_header));

    for (int parcurgere = 0; parcurgere < no_of_sections; parcurgere++)
    {
        read(fd1, &header[parcurgere].SECT_NAME, 20);
        read(fd1, &header[parcurgere].SECT_TYPE, 1);
        read(fd1, &header[parcurgere].SECT_OFFSET, 4);
        read(fd1, &header[parcurgere].SECT_SIZE, 4);
        header[parcurgere].SECT_NAME[20] = '\0';

        if (header[parcurgere].SECT_TYPE != 43 && header[parcurgere].SECT_TYPE != 53 && header[parcurgere].SECT_TYPE != 12 && header[parcurgere].SECT_TYPE != 72 && header[parcurgere].SECT_TYPE != 76 && header[parcurgere].SECT_TYPE != 16 && header[parcurgere].SECT_TYPE != 64)
        {
            close(fd1);
            free(header);
            return -1;
        }
    }
    close(fd1);
    free(header);
    return 0;
}

int findall(const char *path)
{
    DIR *dirijare = NULL;
    struct dirent *intrare = NULL;
    char filePath[512];
    struct stat statbuf;
    static int aux = 0;
    int fd1;

    int version = 0;
    char no_of_sections = '1';
    int header_size = 0;
    char magic[3];
    magic[2]=0;
    // int dimension=0;
    //   char c;

    dirijare = opendir(path);

    if (dirijare == NULL)
    {
        printf("ERROR\ninvalid directory path");
        //close(fd1);
        return -4;
    }

    if (!aux)
    {
        printf("SUCCESS\n");
        aux = 1;
    }

    while ((intrare = readdir(dirijare)) != NULL)
    {
        if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0)
        {

            snprintf(filePath, 512, "%s/%s", path, intrare->d_name);
            if (lstat(filePath, &statbuf) == 0)
            {
                //    printf("dddddd\n");

                //  printf("%s\n", filePath);
                if (S_ISREG(statbuf.st_mode))
                {
                    ///  printf("AA\n");
                    if (validationSF(filePath) == 0)
                    {
                        fd1 = open(filePath, O_RDONLY);
                        lseek(fd1, -2, SEEK_END);
                        read(fd1, magic, 2);
                        magic[2] = '\0';
                        lseek(fd1, -4, SEEK_END);
                        read(fd1, &header_size, 2);
                        lseek(fd1, -header_size, SEEK_END);
                        read(fd1, &version, 1);
                        read(fd1, &no_of_sections, 1);
                        section_header *header;
                        header = (section_header *)malloc(no_of_sections * sizeof(section_header));
                        int ok = 1;
                        for (int parcurgere = 0; parcurgere < no_of_sections && ok; parcurgere++)
                        {
                            read(fd1, &header[parcurgere].SECT_NAME, 20);
                            read(fd1, &header[parcurgere].SECT_TYPE, 1);
                            read(fd1, &header[parcurgere].SECT_OFFSET, 4);
                            read(fd1, &header[parcurgere].SECT_SIZE, 4);
                            header[parcurgere].SECT_NAME[20] = '\0';
                            if (header[parcurgere].SECT_SIZE > 1168)
                            {
                                ok = 0;
                            }
                        }
                        if (ok)
                            printf("%s\n", filePath);
                        free(header);
			 close(fd1);
                    }
                }
                if (S_ISDIR(statbuf.st_mode))
                {
                    findall(filePath);
                }
            }
        }
    }

    //close(fd1);
    closedir(dirijare);
    return 0;
}

int extragere(const char *path, int section, int line)
{

    int fd1;
    int version;
    char no_of_sections = '1';
    int header_size;
    char magic[3];
    magic[2]=0;
    int s = 1;
    char charachter;
    int dimension = 0;
    int linieComparat = 1;
    // int detector=0;
    section_header *head;
    fd1 = open(path, O_RDONLY);

    static int aux = 0;

    if (fd1 == -1)
    {
        printf("ERROR\ninvalid file");
        close(fd1);
        return -7;
    }
    if (!aux)
    {
        printf("SUCCESS\n");
        aux = 1;
    }

    lseek(fd1, -2, SEEK_END);
    read(fd1, magic, 2);
    magic[2] = '\0';
    lseek(fd1, -4, SEEK_END);
    read(fd1, &header_size, 2);
    lseek(fd1, -header_size, SEEK_END);
    read(fd1, &version, 1);
    read(fd1, &no_of_sections, 1);
    head = (section_header *)malloc(no_of_sections * sizeof(section_header));
    if (section > no_of_sections)
    {
    	printf("ERROR\ninvalid section");
    	free(head);
        close(fd1);
        return -1;
    }
    if (line < 1)
    {
    	printf("ERROR\ninvalid line");
    	free(head);
        close(fd1);
    	return -1;
    }

    for (int parcurgere = 0; s && parcurgere < no_of_sections; parcurgere++)
    {
        read(fd1, &head[parcurgere].SECT_NAME, 20);
        read(fd1, &head[parcurgere].SECT_TYPE, 1);
        read(fd1, &head[parcurgere].SECT_OFFSET, 4);
        read(fd1, &head[parcurgere].SECT_SIZE, 4);
        head[parcurgere].SECT_NAME[20] = '\0';
    }

    lseek(fd1, head[section - 1].SECT_OFFSET, SEEK_SET);

    // linieComparat=line;

    // printf("\nSUCCESS\n");
    while (read(fd1, &charachter, 1) != 0)
    {
        dimension++;
        if (linieComparat == line)
        {
            // linieComparat++
            printf("%c", charachter);
            //  printf("%d\n",line);
        }
        if (dimension == head[section - 1].SECT_SIZE)
            break;
        if (charachter == 0x0A) // daca am terminat de citit linia 0x0A -codul pentru \n;
        {
            linieComparat++;
        }
    }
    //  printf("%d",linieComparat);
    //  printf("%d\n",line);
    // printf("%d\n", linieComparat);

    //        if(linieComparat!=line)
    //        {
    //            printf("ERORR\ninvalid line");
    //        }
    //
    //        else if(section!=no_of_sections)
    //        {
    //            printf("ERROR\ninvalid section");
    //        }

    // rintf("Section aaaa %d\n", section);

    //  free(head);

free(head);
        close(fd1);
    return 0;
}

int list(const char *path, bool flag, int valoare, off_t value, char *format)
{

    DIR *dirijare = NULL;
    struct dirent *intrare = NULL;
    char filePath[512];
    struct stat statbuf;
    // printf("list\n \n ");
    dirijare = opendir(path);
    if (dirijare == NULL)
    {
        perror("Nu s-a putut deschide fisierul");
        return -1;
    }

    printf("SUCCESS\n");
    while ((intrare = readdir(dirijare)) != NULL)
    {
        if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0)
        {
            if (flag == false && valoare == 0)
            {
                snprintf(filePath, 512, "%s/%s", path, intrare->d_name);
                if (lstat(filePath, &statbuf) == 0)
                {
                    // if(strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0)
                    printf("%s\n", filePath);
                }
            }
            else if (flag == true && valoare == 1)
            {
                snprintf(filePath, 512, "%s/%s", path, intrare->d_name);

                if (lstat(filePath, &statbuf) == 0)
                {
                    if ((S_ISREG(statbuf.st_mode) && (statbuf.st_size > value)))
                    {
                        printf("%s\n", filePath);
                    }
                }
            }
        }
    }
    closedir(dirijare);
    return 0;
}

void list_perm(const char *path, int wr)
{
    DIR *dirijare = NULL;
    struct dirent *intrare = NULL;
    char filePath[512];
    struct stat statbuf;
    static int aux = 0;
    dirijare = opendir(path);
    //  printf("list_perm");
    if (dirijare == NULL)
    {
        perror("Nu s-a putut deschide fisierul");
        return;
    }
    if (!aux)
    {
        printf("SUCCESS\n");
        aux = 1;
    }
    while ((intrare = readdir(dirijare)) != NULL)
    {
        if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0)
        {
            snprintf(filePath, 512, "%s/%s", path, intrare->d_name);
            if (lstat(filePath, &statbuf) == 0)
            {
                if (wr)
                {
                    if (statbuf.st_mode & 0200)
                        printf("%s\n", filePath);
                }
            }
        }
    }
    closedir(dirijare);
}

void recursiv_1(const char *path, bool flag, int valoare, off_t value, char *format)
{
    DIR *dirijare = NULL;
    struct dirent *intrare = NULL;
    char filePath[512];
    struct stat statbuf;
    static int aux = 0;

    //  printf("recursiv_l");

    dirijare = opendir(path);

    //  printf("%s\n",path);

    if (dirijare == NULL)
    {
        perror("Nu s-a putut deschide fisierul");
        return;
    }

    if (!aux)
    {
        printf("SUCCESS\n");
        aux = 1;
    }

    while ((intrare = readdir(dirijare)) != NULL)
    {

        if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0)
        {
            if (flag == false && valoare == 0)
            {
                snprintf(filePath, 512, "%s/%s", path, intrare->d_name);
                if (lstat(filePath, &statbuf) == 0)
                {
                    printf("%s\n", filePath);
                    if (S_ISDIR(statbuf.st_mode))
                        recursiv_1(filePath, flag, valoare, value, format);
                }
            }
            else if (flag == true && valoare == 1)
            {
                snprintf(filePath, 512, "%s/%s", path, intrare->d_name);
                if (lstat(filePath, &statbuf) == 0)
                {
                    if ((statbuf.st_size > value) && (!S_ISDIR(statbuf.st_mode)))
                    {
                        printf("%s\n", filePath);
                    }
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        recursiv_1(filePath, flag, valoare, value, format);
                    }
                }
            }
        }
    }
    closedir(dirijare);
}

void recursiv_perm(const char *path, int wr)
{
    DIR *dirijare = NULL;
    struct dirent *intrare = NULL;
    char filePath[512];
    struct stat statbuf;
    static int aux = 0;

    // printf("recursiv_perm");
    dirijare = opendir(path);

    //  printf("%s\n",path);

    if (dirijare == NULL)
    {
        perror("Nu s-a putut deschide fisierul");
        return;
    }

    if (!aux)
    {
        printf("SUCCESS\n");
        aux = 1;
    }

    while ((intrare = readdir(dirijare)) != NULL)
    {
        if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0)
        {
            snprintf(filePath, 512, "%s/%s", path, intrare->d_name);
            if (lstat(filePath, &statbuf) == 0)
            {
                if (wr)
                {
                    if (statbuf.st_mode & 0200)
                        printf("%s\n", filePath);
                }
                else
                    printf("%s\n", filePath);
            }

            if (S_ISDIR(statbuf.st_mode))
            {
                recursiv_perm(filePath, wr);
            }
        }
    }
    free(dirijare);
}

int main(int argc, char **argv)
{
    int wr = 0;
    off_t value = 0;
    char *path;
    char *function;
    int numberLine = 0;
    int numberSection = 0;
    int vec[7];

    if (strcmp(argv[1], "variant") == 0)
    {
        vec[0] = 1;
        printf("76809\n");
    }
    else
    {
        if (strcmp(argv[1], "parse") == 0)
        {
            path = argv[2] + 5;
            parse(path);
        }
        else if (strcmp(argv[1], "findall") == 0)
        {
            path = argv[2] + 5;
            findall(path);
        }
        else if (strcmp(argv[1], "extract") == 0)
        {
            path = argv[2] + 5;
            vec[4] = 1;
            sscanf(argv[3] + 8, "%d", &numberSection);
            sscanf(argv[4] + 5, "%d", &numberLine);
        }
        else if (strcmp(argv[1], "list") == 0)
        {
            vec[6] = 1;
            for (int i = 2; i < argc; i++)
            {
                if (strcmp(argv[i], "path=") != 0)
                {
                    path = argv[i] + 5;
                }
                if (strcmp(argv[i], "recursive") == 0)
                {
                    vec[1] = 1;
                }
                if (strstr(argv[i], "size_greater=") != 0)
                {
                    vec[2] = 1;

                    function = argv[i] + 13;
                    sscanf(function, "%ld", &value);
                }

                if (strcmp(argv[i], "has_perm_write") == 0)
                {
                    vec[3] = 1;
                    wr = 1;
                }
            }
        }
    }
    if (vec[4] == 1)
    {

        extragere(path, numberSection, numberLine);
    }
    else if (vec[1] == 1)
    {
        if (vec[3] == 1)
        {
            recursiv_perm(path, wr);
        }
        else if (vec[2] == 1)
        {
            recursiv_1(path, true, 1, value, NULL);
        }
        else
        {
            recursiv_1(path, false, 0, value, NULL);
        }
    }
    else
    {
        if (vec[3] == 1)
        {
            list_perm(path, wr);
        }
        else if (vec[2] == 1)
        {
            list(path, true, 1, value, NULL);
        }
        else if (vec[6] == 1)
        {
            list(path, false, 0, value, NULL);
        }
    }
    return 0;
}
