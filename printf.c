#include <unistd.h>
#include <stdarg.h>

int put_str(char *s)
{
    int i = 0;
    int count = 0;
    if(!s)
        s = "(null)";  
    while(s[i])
    {
        write(1, &s[i], 1);
        i++;
        count++;
    }
    return count;
}

int put_digits(int num, int base)
{
    char digits[] = "0123456789abcdef";
    int count = 0;

    if(num < 0)
    {
         num = num * -1;
         write(1, "-", 1);
         count++;
    }
    if(num >= base)
    {
        put_digits(num / base, base);
        
    }
    write(1, &digits[num % base], 1);
    count++;
    return count;
}

int u_printf(const char *format, ...)
{

    int count = 0;
    va_list ptr;
    va_start(ptr, format);

    while(*format)
    {
        if(*format == '%')
        {
            format++;
            if(*format == 's')
                count += put_str(va_arg(ptr, char *));
            else if(*format == 'd')
                count += put_digits(va_arg(ptr, int), 10);
            else if(*format == 'x')
                count+= put_digits(va_arg(ptr, unsigned int), 16);
        }
        else
        {
            write(1, format, 1);
            count++;
        }
        format++;
    }
    va_end(ptr);
    return count;
}