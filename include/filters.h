#include <string>
#include "itk2cython.h"
#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include "itkImageFileWriter.h"

template < class ImageType >
void AHE( ImageType* itk_image,
          int radius ) {
 
  typedef  itk::AdaptiveHistogramEqualizationImageFilter< ImageType > AdaptiveHistogramEqualizationImageFilterType;
  typename AdaptiveHistogramEqualizationImageFilterType::Pointer adaptiveHistogramEqualizationImageFilter = AdaptiveHistogramEqualizationImageFilterType::New();
  adaptiveHistogramEqualizationImageFilter->SetInput(itk_image);
  adaptiveHistogramEqualizationImageFilter->SetRadius(radius);
  adaptiveHistogramEqualizationImageFilter->Update();
  
  // debugging
  // typedef  itk::ImageFileWriter< ImageType  > WriterType;
  // typename WriterType::Pointer writer = WriterType::New();
  // writer->SetFileName("debug_AHE.nii");
  // writer->SetInput(adaptiveHistogramEqualizationImageFilter->GetOutput());
  // writer->Update();
 
  itk_image->Graft(adaptiveHistogramEqualizationImageFilter->GetOutput());
}

void _AHE_float_3( float* img,
                  double* pixelSize,
                  double* orientation,
                  double* origin,
                  int* dim,
                  int radius );


