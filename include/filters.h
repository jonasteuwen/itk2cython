#include "itk2cython.h"
#include "itkAdaptiveHistogramEqualizationImageFilter.h"

template < class ImageType >
void AHE( ImageType* itk_image,
          int radius ) {
 
  typedef  itk::AdaptiveHistogramEqualizationImageFilter< ImageType > AdaptiveHistogramEqualizationImageFilterType;
  typename AdaptiveHistogramEqualizationImageFilterType::Pointer adaptiveHistogramEqualizationImageFilter = AdaptiveHistogramEqualizationImageFilterType::New();
  adaptiveHistogramEqualizationImageFilter->SetInput(itk_image);
  adaptiveHistogramEqualizationImageFilter->SetRadius(radius);
  adaptiveHistogramEqualizationImageFilter->Update();
 
  itk_image = adaptiveHistogramEqualizationImageFilter->GetOutput();
}

void _AHE_float_3( float* img,
                  double* pixelSize,
                  double* orientation,
                  double* origin,
                  int* dim,
                  int radius );


