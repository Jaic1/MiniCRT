# Source
code from [程序员的自我修养](https://book.douban.com/subject/3652388/)  
only work in x86_64-linux

# Note
I compile these 32-bit codes on 64-bit machine(x86_64-linux).  
For detail, refer *Makefile*.  
In order to do this, there is one place need to be fixed:
```
argc = *(int *)(ebp_reg + 8);
argv = (char **)(ebp_reg + 12);
```
They were `+4` and `+8` before,  
but since we are running 32-bit program on x86_64,  
the first instruction in `mini_crt_entry` would be `push %rbp`,  
then `rbp` would occupy 8 bytes in the stack,  
instead of 4 bytes when compiled and linked for 32-bit machine.

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

4. `printf` not working
solution:  
do not define `va_list`, `va_start`, `va_arg` ourself,  
use `#include <stdarg.h>` instead.

5. `fwrite` not support writing an int
It just uses size*count, so we should just use `char` to test.
```
int fwrite(const void *buffer, int size, int count, FILE *stream)
{
    return write((int)stream, buffer, size * count);
}

```
my test for `fwrite`(change in `fread` test too):
```
fp = fopen("test.txt", "w");
for (i = 0; i < argc; i++)
{
    int len = strlen(v[i]);
    char len_written = len + '0';

    printf("writing %d%s to fd %d\n", len, v[i], fp);
    fwrite(&len_written, 1, 1, fp);
    fwrite(v[i], 1, len, fp);
    fwrite(&newline, 1, 1, fp);
}
fclose(fp);
```
