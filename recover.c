
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image");
        return 1;
    }

    // remember file names
    char *image = argv[1];

    // open image card file
    FILE *imgptr = fopen(image, "r");
    if (imgptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", image);
        return 2;
    }

    // check if we found the first jpeg
    int foundjpeg = 0; // false

    // counter for number of jpegs found on memory card
    int numjpegs = 0;

    // assign variable for jpeg filename
    char filename[8] = {0};

    // jpeg file pointer
    FILE* img;

    // create buffer
    unsigned char buffer[512];

    // repeat until end of card. I may need to divide sizeof(imgptr)
        while(fread(buffer, 512, 1, imgptr) == 1)
        {
            if (buffer[0] == 0xff &&
                buffer[1] == 0xd8 &&
                buffer[2] == 0xff &&
                (buffer[3] & 0xf0) == 0xe0)
            {
                // we found a JPEG
                foundjpeg = 1;
                // already found a JPEG?
                if (numjpegs == 0)
                {
                    // no =>
                    // start first jpeg
                    sprintf(filename, "%03i.jpg", numjpegs);
                    img = fopen(filename, "w");
                    numjpegs++;
                }
                else
                {
                    // yes =>
                    // close previous file and open our new one
                    fclose(img);
                    sprintf(filename, "%03i.jpg", numjpegs);
                    img = fopen(filename, "w");
                    numjpegs++;
                }
            }
            if (foundjpeg == 1)
            {
            // write 512 bytes to new jpeg file
            fwrite(&buffer, 512, 1, img);
            }
        }

    // exit loop and close any remaining files.
    fclose(imgptr);
    fclose(img);

    // success
    return 0;

}

