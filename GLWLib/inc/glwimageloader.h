#pragma once

#include "opengl.h"

#define NULL_IMAGE "NULL_IMAGE"

namespace glw
{
  namespace util
  {
    struct ImageData
    {
      unsigned char * data;
      int w, h, n;
    };

    class ImageLoader
    {
    public:

      ImageLoader();

      ~ImageLoader();

      static GLuint loadTextureFromImage(const char *filename);
      static GLuint loadBlankTexture();
      static GLuint loadUniformColourTexture(int r, int g, int b);
      static void getData(ImageData& imageData, const char * filename);
      static void getDataPNG(ImageData& imageData, const char * filename);
      static void loadImageData(ImageData& imageData, unsigned char * data, int w, int h, int n);

    private:

      static GLuint getTexId(unsigned char * data, int w, int h);

    };
  }
}

