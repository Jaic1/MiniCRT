#include "minicrt.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *fp;
    char space = ' ';
    char newline = '\n';
    char **v = malloc(argc * sizeof(char *));

    for (i = 0; i < argc; i++)
    {
        printf("mallocing %d bytes for v[%d] to store %s\n",
               strlen(argv[i]) + 1, i, argv[i]);
        v[i] = malloc(strlen(argv[i]) + 1);
        strcpy(v[i], argv[i]);
        printf("    result: %s\n", v[i]);
    }
    printf("test malloc succeed\n\n");

    printf("sizeof(FILE) %s sizeof(int)\n",
           sizeof(FILE) == sizeof(int) ? "==" : "!=");
    fp = fopen("test.txt", "w");
    for (i = 0; i < argc; i++)
    {
        int len = strlen(v[i]);
        char len_written = len + '0';

        printf("writing %d %s to fd %d\n", len, v[i], fp);
        fwrite(&len_written, 1, 1, fp);
        fwrite(&space, 1, 1, fp);
        fwrite(v[i], 1, len, fp);
        fwrite(&newline, 1, 1, fp);
    }
    fclose(fp);
    printf("fwrite succeed\n\n");

    fp = fopen("test.txt", "r");
    for (i = 0; i < argc; i++)
    {
        int len;
        char len_read;
        char *buf;

        fread(&len_read, 1, 1, fp);
        len = len_read - '0';
        buf = malloc(len + 3);
        fread(buf, 1, len+2, fp);
        buf[len+2] = '\0';
        printf("%d%s", len, buf);
        free(buf);
        free(v[i]);
    }
    fclose(fp);
    printf("fread succeed\n");

    return 0;
}