//-*-c++-*-
#ifndef _cs1300bmp_h_
#define _cs1300bmp_h_

//
// Maximum image size
//
#define MAX_DIM 1024 //changed from 8096

//
// The specific colors
//
#define COLOR_RED 0
#define COLOR_GREEN 1
#define COLOR_BLUE 2
#define MAX_COLORS 3

#include <cstdint>

struct cs1300bmp {
  //
  // Actual width used by this image
  //
  short width; //changed from int
  //
  // Actual height used by this image
  //
  short height; //changed from int (largest image dimension can fit in a short)
  //
  // R/G/B fields
  // 
  short color[MAX_COLORS][MAX_DIM][MAX_DIM]; //changed from int. Max is 255 (color)
};

//
// routines to read and write BMP images
//

#ifdef __cplusplus
extern "C" {
#endif

int cs1300bmp_readfile(char *filename, struct cs1300bmp *image);
int cs1300bmp_writefile(char *filename, struct cs1300bmp *image);

#ifdef __cplusplus
}
#endif


#endif
