#include "minicrt.h"

int main(int argc, char *argv[])
{
    printf("in main: argc=%d %d \n", argc, argc);

    int i;
    FILE *fp;
    char **v = malloc(argc * sizeof(char *));

    for (i = 0; i < argc; i++)
    {
        v[i] = malloc(strlen(argv[i]) + 1);
        strcpy(v[i], argv[i]);
    }

    fp = fopen("test.txt", "w");
    for (i = 0; i < argc; i++)
    {
        int len = strlen(v[i]);
        fwrite(&len, sizeof(int), 1, fp);
        fwrite(v[i], 1, len, fp);
    }
    fclose(fp);

    fp = fopen("test.txt", "r");
    for(i = 0;i<argc;i++){
        int len;
        char *buf;
        buf = malloc(len + 1);
        fread(&len, sizeof(int), 1, fp);
        fread(buf, 1, len, fp);
        buf[len] = '\0';
        printf("%d %s\n", len, buf);
        free(buf);
        free(v[i]);
    }
    fclose(fp);
    
    return 0;
}