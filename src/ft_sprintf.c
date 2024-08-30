#include "../inc/minishell.h"

int ft_sprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int count = 0;
    char c;
    const char *p = format;
    char *s;
    int d;

    while (*p) {
        if (*p == '%' && *(p + 1)) {
            p++;
            switch (*p) {
                case 'd':
                    d = va_arg(args, int);
                    count += sprintf(buffer + count, "%d", d);
                    break;
                case 's':
                    s = va_arg(args, char *);
                    count += sprintf(buffer + count, "%s", s);
                    break;
                case 'c':
                    c = (char)va_arg(args, int);
                    buffer[count++] = c;
                    break;
                default:
                    buffer[count++] = '%';
                    buffer[count++] = *p;
                    break;
            }
        } else {
            buffer[count++] = *p;
        }
        p++;
    }
    buffer[count] = '\0';
    va_end(args);
    return count;
}