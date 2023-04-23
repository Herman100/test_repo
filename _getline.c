#include <unistd.h>
#include <stdlib.h>

#define READ_SIZE 1024

/**
 * _getline - reads an entire line from a file descriptor
 * @fd: the file descriptor to read from
 * Return: the line read as a string
 */
char *_getline(const int fd)
{
    static char *buffer;
    static size_t size;
    char *line;
    ssize_t bytes;
    size_t i, j;

    if (!buffer)
    {
        buffer = malloc(sizeof(char) * READ_SIZE);
        if (!buffer)
            return (NULL);
        size = 0;
    }
    for (i = 0; ; i += READ_SIZE)
    {
        if (i >= size)
        {
            bytes = read(fd, buffer + size, READ_SIZE);
            if (bytes <= 0)
                break;
            size += bytes;
        }
        for (j = i; j < size; j++)
        {
            if (buffer[j] == '\n')
            {
                line = malloc(sizeof(char) * (j - i + 2));
                if (!line)
                    return (NULL);
                line[j - i + 1] = '\0';
                while (j > i)
                    line[--j - i] = buffer[j];
                size -= j + 1;
                for (j = 0; j < size; j++)
                    buffer[j] = buffer[j + i + 1];
                return (line);
            }
        }
    }
    if (size == 0)
        return (NULL);
    line = malloc(sizeof(char) * (size + 1));
    if (!line)
        return (NULL);
    line[size] = '\0';
    while (size--)
        line[size] = buffer[size];
    size = 0;
    return (line);
}

