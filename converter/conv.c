#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Invalid usage!\n");
        return -1;
    }

    const char *src_path = argv[1];
    const char *dst_path = argv[2];

    FILE *src = fopen(src_path, "rb");
    if (src == NULL) {
        printf("Failed to open source file %s\n", src_path);
        return -1;
    }

    FILE *dst = fopen(dst_path, "w");
    if (dst == NULL) {
        printf("Failed to open output file %s\n", dst_path);
        fclose(src);
        return -1;
    }

    fseek(src, 0, SEEK_END);
    const size_t size = ftell(src);
    fseek(src, 0, SEEK_SET);

    printf("File size: %zuB\n", size);


    fprintf(dst, "const unsigned char PROGMEM hex[] = {");

    uint8_t byte;
    char buffer[16];
    for (size_t i = 0; i < size; ++i) {
        fread(&byte, sizeof(byte), 1, src);
        snprintf(buffer, sizeof(buffer), "0x%02X, ", byte);
        fprintf(dst, "%s", buffer);
    }

    fprintf(dst, "};");

    fclose(dst);
    fclose(src);

    printf("Done!\n");

    return 0;
}
