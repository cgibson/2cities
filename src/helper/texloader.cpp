#include "texloader.h"

namespace texldr
{
    GLuint loadBMP(const char *filename)
    {
        // generate texture id
        GLuint id;
        glGenTextures(1, &id);
        
        // open the file
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL)
        {
            fprintf(stderr, "ERROR: %s not found!\n", filename);
            exit(EXIT_FAILURE);
        }

        // jump to the width/height
        fseek(fp, 18, SEEK_CUR);

        // read the width and height
        unsigned int width = getuint(fp);
        unsigned int height = getuint(fp);

        // sanity check - make sure the # of planes is 1
        unsigned short planes = getushort(fp);
        if (planes != 1)
        {
            fprintf(stderr, "ERROR: Cannot load %s, too many planes!\n", filename);
            exit(EXIT_FAILURE);
        }

        // sanity check - make sure the color depth is 24-bit
        unsigned short bpp = getushort(fp);
        if (bpp != 24)
        {
            fprintf(stderr, "ERROR: Cannot load %s, bit depth is not 24-bit!\n", filename);
            exit(EXIT_FAILURE);
        }

        // jump to the image data
        fseek(fp, 24, SEEK_CUR);

        // calculate the image data size and allocate memory for it
        unsigned int size = width * height * 3; // 3 bytes per pixel (RGB)
        unsigned char *data = (unsigned char *)malloc(size);
        if (data == NULL)
        {
            fprintf(stderr, "ERROR: Couldn't allocate memory to load %s!\n", filename);
            exit(EXIT_FAILURE);
        }

        // read in the image data
        if (fread(data, size, 1, fp) != 1)
        {
            fprintf(stderr, "ERROR: Problem reading image data from %s!\n", filename);
            exit(EXIT_FAILURE);
        }

        // swap the color byte order (BGR -> RGB)
        for (int i = 0; i < size; i += 3)
        {
            unsigned char temp = data[i];
            data[i] = data[i + 2];
            data[i + 2] = temp;
        }

        // close the file
        fclose(fp);

        // bind the texture and load the image data
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // hand back the new texture id
        return id;
    }

    static unsigned int getuint(FILE *fp)
    {
        // assumes little-endian!
        int b1 = getc(fp);
        int b2 = getc(fp);
        int b3 = getc(fp);
        int b4 = getc(fp);

        return ((unsigned int) b1) +
               (((unsigned int) b2) << 8) +
               (((unsigned int) b3) << 16) +
               (((unsigned int) b4) << 24);
    }

    static unsigned short getushort(FILE *fp)
    {
        int b1 = getc(fp);
        int b2 = getc(fp);

        return ((unsigned int) b1) +
               (((unsigned int) b2) << 8);
    }
}
