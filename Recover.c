#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Couldn't open %s.\n", argv[1]);
        return 1;
    }

    uint8_t buffer[512];
    FILE *img = NULL;
    char filename[8];
    int files = 0;

    while (fread(buffer, 1, 512, card) == 512)
    {
        bool jpeg = buffer[0] == 0xff &&
                    buffer[1] == 0xd8 &&
                    buffer[2] == 0xff &&
                    (buffer[3] & 0xf0) == 0xe0;

        if (jpeg)
        {
            if (img != NULL)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", files++);
            img = fopen(filename, "wb");
            if (img == NULL)
            {
                fclose(card);
                printf("Couldn't create %s.\n", filename);
                return 1;
            }
        }

        if (img != NULL)
        {
            fwrite(buffer, 1, 512, img);
        }
    }

    if (img != NULL)
    {
        fclose(img);
    }

    fclose(card);
    return 0;
}
