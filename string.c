#include "minicrt.h"

// only when radix 10, itoa can accept n < 0
char *itoa(int n, char *str, int radix)
{
    static char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *p = str;
    char *head = str;
    if (!p || radix < 2 || radix > 36)
        return str;
    if (radix != 10 && n < 0)
        return str;

    if (n == 0)
    {
        *p++ = '0';
        *p = 0;
        return str;
    }

    if (radix == 10 && n < 0)
    {
        *p++ = '-';
        n = -n;
        head++;
    }

    while (n)
    {
        *p++ = digit[n % radix];
        n /= radix;
    }
    *p = 0;
    for (--p; head < p; ++head, --p)
    {
        char temp = *head;
        *head = *p;
        *p = temp;
    }

    return str;
}

int strcmp(const char *src, const char *dst)
{
    int ret = 0;
    unsigned char *p1 = src;
    unsigned char *p2 = dst;
    while (!(ret = *p1 - *p2) && *p1 && *p2)
    {
        ++p1;
        ++p2;
    }

    if (ret < 0)
        ret = -1;
    else if (ret > 0)
        ret = 1;
    return ret;
}

char *strcpy(char *dst, const char *src)
{
    char *ret = dst;
    while (*src)
    {
        *dst++ = *src++;
    }
    *dst = '\0';
    return ret;
}

unsigned strlen(const char *str)
{
    int cnt = 0;
    if (!str)
        return 0;
    while (*str++ != '\0')
        cnt++;
    return cnt;
}
