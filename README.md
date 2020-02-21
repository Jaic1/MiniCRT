# Source
code from [程序员的自我修养](https://book.douban.com/subject/3652388/)

# Errata
1. In *malloc.c*'s `free` function, next block's prev may not be updated:
```
if(next && next->type == HEAP_BLOCK_FREE)
{
    header->size += next->size;
    next = next->next;
    header->next = next;
    // missed in the book
    if(next)
        next->prev = header;
}
```

2. In *printf.c*, the type of the first argument of `fputc` function should be `char`
```
int fputc(char c, FILE *stream)
{
    if(fwrite(&c, 1, 1, stream) != 1)
        return EOF;
    else
        return c;
}
```

3. In *tests/test.c*, `size` is not `count`
```
fp = fopen("test.txt", "w");
    for (i = 0; i < argv; i++)
    {
        int len = strlen(v[i]);
        fwrite(&len, sizeof(int), 1, fp);
        fwrite(v[i], 1, len, fp);
    }
    fclose(fp);
```
