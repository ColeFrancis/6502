#include "stdlib.h"
#include "stdio.h"
#include "file_handler.h"

void load_ram (char *file_name, uint8_t *ram)
{
    FILE *file;
    long file_size;
    size_t read_result;
    int read_size;

    file = fopen(file_name, "rb");
    if (file == NULL)
    {
            fprintf(stderr, "Error, unable to open %s\n", file_name);
            exit(1);
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    read_size = (file_size > MEM_SIZE) ? MEM_SIZE : (int) file_size;

    read_result = fread(ram, 1, read_size, file);
    if (read_result != read_size)
    {
            fprintf(stderr, "Error in loading memory");
            exit(1);
    }

    fclose(file);
}

void dump_ram (char* file_name, uint8_t *ram)
{
        FILE *file;

        file = fopen(file_name, "wb");
        if (file == NULL)
        {
                fprintf(stderr, "Error, unable to open %s\n", file_name);
                exit(1);
        }

        fwrite(ram, 1, MEM_SIZE, file);

        fclose(file);
}