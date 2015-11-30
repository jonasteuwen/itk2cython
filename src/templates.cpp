#include "templates.h"

void _imread_float( char* filename,
                    float* img ) {
    _imread<float,3>( filename,
                      img );
}

void _imwrite_float( char* filename,
                     float* img,
                     double* pixelSize,
                     double* orientation,
                     double* origin,
                     int* dim ) {
    _imwrite<float,3>( filename,
                       img,
                       pixelSize,
                       orientation,
                       origin,
                       dim );
}

    
