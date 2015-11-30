#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "itk2cython.h"


    
void _imread_float( char* filename,
                    float* img );
void _imwrite_float( char* filename,
                     float* img,
                     double* pixelSize,
                     double* orientation,
                     double* origin,
                     int* dim );



    
#endif
