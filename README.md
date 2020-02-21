# Source
code from [程序员的自我修养](https://book.douban.com/subject/3652388/)

# Errata
In *malloc.c*'s `free` function, next block's prev may not be updated:
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