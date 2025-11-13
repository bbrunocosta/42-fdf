#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

char *read_file_to_string(int fd)
{
    char buffer[BUFFER_SIZE];
    char *content = NULL;
    size_t total = 0;
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        char *tmp = realloc(content, total + (size_t)bytesRead + 1);
        if (!tmp) {
            free(content);
            return NULL;
        }
        content = tmp;
        memcpy(content + total, buffer, (size_t)bytesRead);
        total += (size_t)bytesRead;
        content[total] = '\0';
    }

    if (bytesRead < 0) {
        free(content);
        return NULL;
    }

    return content;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    
    char *content = read_file_to_string(fd);
    if (!content) {
        perror("read_file_to_string");
        return 1;
    }
    
    printf("Tamanho: %zu bytes\n", strlen(content));
    printf("Conteúdo:\n[%s]\n", content);
    
    // Mostrar caracteres especiais
    printf("\nHexdump dos primeiros e últimos bytes:\n");
    size_t len = strlen(content);
    for (size_t i = 0; i < 20 && i < len; i++) {
        printf("%02x ", (unsigned char)content[i]);
    }
    printf("\n...\n");
    for (size_t i = (len > 20 ? len - 20 : 0); i < len; i++) {
        printf("%02x ", (unsigned char)content[i]);
    }
    printf("\n");
    
    free(content);
    close(fd);
    return 0;
}
