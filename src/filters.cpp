#include "filters.h"

void _AHE_float_3( float* img,
                  double* pixelSize,
                  double* orientation,
                  double* origin,
                  int* dim,
                  int radius ) {
    typedef itk::Image<float, 3> ImageType;
    typename ImageType::Pointer itk_image = ImageType::New();

    py2itk<float,3>( itk_image,
                     img,
                     pixelSize,
                     orientation,
                     origin,
                     dim );
    
    // AHE< ImageType >( itk_image,
    //                   radius );

    // itk2py<float,3>( itk_image,
    //                  img,
    //                  pixelSize,
    //                  orientation,
    //                  origin,
    //                  dim );
}
