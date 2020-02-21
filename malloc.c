/*
 * difference between original version on the book:
 * - change the header name to block_header, instead of heap_header
 * - in free fn, next block's prev may not be updated, we fix it
 */

#include "minicrt.h"

typedef struct _block_header
{
    enum
    {
        HEAP_BLOCK_FREE = 0xabababab,
        HEAP_BLOCK_USED = 0xcdcdcdcd,
    } type;

    unsigned size;
    struct _block_header *next;
    struct _block_header *prev;
} block_header;

#define ADDR_ADD(addr, offset) ((char *)(addr) + offset)
#define HEADER_SIZE (sizeof(block_header))

static block_header *list_head = NULL;

void free(void *ptr)
{
    block_header *header = (block_header *)ADDR_ADD(ptr, -HEADER_SIZE);
    if (header->type == HEAP_BLOCK_USED)
        return;

    // if the free block can be coalesce
    block_header *prev = header->prev;
    block_header *next = header->next;
    header->type = HEAP_BLOCK_FREE;
    if (prev && prev->type == HEAP_BLOCK_FREE)
    {
        prev->size += header->size;
        prev->next = next;
        if(next)
            next->prev = prev;
        header = prev;
    }
    if(next && next->type == HEAP_BLOCK_FREE)
    {
        header->size += next->size;
        next = next->next;
        header->next = next;
        if(next)
            next->prev = header;
    }
}

void *malloc(unsigned size)
{
    block_header *header;

    if (size == 0)
        return NULL;

    // first fit
    header = list_head;
    while (header)
    {
        // if block big enough
        if (header->size < size + HEADER_SIZE)
        {
            header = header->next;
            continue;
        }

        // if block can be split
        if (header->size > size + HEADER_SIZE * 2)
        {
            block_header *next = header->next;
            block_header *curr = (block_header *)ADDR_ADD(header, size + HEADER_SIZE);

            curr->type = HEAP_BLOCK_FREE;
            curr->size = header->size - size - HEADER_SIZE;
            curr->prev = header;
            curr->next = next;

            if (next)
                next->prev = curr;
            header->next = curr;
            header->size = size + HEADER_SIZE;
        }
        header->type = HEAP_BLOCK_USED;
        return (void *)ADDR_ADD(header, HEADER_SIZE);
    }

    return NULL;
}

#ifdef WIN32
#include <Windows.h>
#else
// Linux brk system call
static int brk(void *end_data_segment)
{
    int ret = 0;
    // brk system call number: 45
    asm("movl $45, %%eax \n\t"
        "movl %1, %%ebx \n\t"
        "int $0x80 \n\t"
        "movl %%eax, %0 \n\t"
        : "=r"(ret)
        : "m"(end_data_segment));
}
#endif

int mini_crt_heap_init()
{
    void *base = NULL;
    block_header *header = NULL;
    unsigned heap_size = 32 * 1024 * 1024;

#ifdef WIN32
    base = VirtualAlloc(0, heap_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (base == NULL)
        return 0;
#else
    base = (void *)brk(0);
    void *end = ADDR_ADD(base, heap_size);
    end = (void *)brk(end);
    if (end == NULL)
        return 0;
#endif

    header = (block_header *)base;
    header->size = heap_size;
    header->type = HEAP_BLOCK_FREE;
    header->next = NULL;
    header->prev = NULL;

    list_head = header;
    return 1;
}
