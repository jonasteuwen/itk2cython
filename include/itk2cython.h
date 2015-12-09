#ifndef ITK2CYTHON_H
#define ITK2CYTHON_H

#include <string>
#include <vector>
#include <sstream>
#include <ios>
#include <iostream>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImportImageFilter.h"
#include "itkImageRegionIterator.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

inline size_t index( size_t i, size_t j,
                     size_t shape0, size_t shape1 ) {
    return j + shape1*i;
}

inline size_t index( size_t i, size_t j, size_t k,
                     size_t shape0, size_t shape1, size_t shape2 ) {
    return k + shape2*( j + shape1*i );
}

inline size_t index( size_t i, size_t j, size_t k, size_t l,
                     size_t shape0, size_t shape1, size_t shape2, size_t shape3 ) {
    return l + shape3*( k + shape2*( j + shape1*i ) );
}

void _query_image( char* filename,
                   std::string& componentType,
                   int& numDimensions,
                   int& numComponents );

void _get_header( char* filename,
                  double* pixelSize,
                  double* orientation,
                  double* origin,
                  int* dim );

template < class ImageType >
void get_attributes( ImageType* itk_image,
                     double* pixelSize,
                     double* orientation,
                     double* origin,
                     int* dim ) {

    int numDimensions = itk_image->GetImageDimension();

    // get size
    typename ImageType::SizeType sizeITK = itk_image->GetBufferedRegion().GetSize();
    for ( int d = 0; d < numDimensions; d++ )
        dim[d] = sizeITK[d];
    
    // copy pixelSize
    typename ImageType::SpacingType spacingITK = itk_image->GetSpacing();
    for ( int d = 0; d< numDimensions; d++ )
        pixelSize[d] = spacingITK[d];

    // copy orientation
    typename ImageType::DirectionType directionITK = itk_image->GetDirection();
    for ( int i = 0; i < numDimensions; i++ )
        for ( int j = 0; j < numDimensions; j++ )
            orientation[index(i,j,numDimensions,numDimensions)] = directionITK[i][j];

    // copy origin
    typename ImageType::PointType originITK = itk_image->GetOrigin();
    for ( int d = 0; d < numDimensions; d++)
        origin[d] = originITK[d];
}




template < class ImageType >
void set_attributes( ImageType* itk_image,
                     double* pixelSize,
                     double* orientation,
                     double* origin,
                     int* dim ) {
    
    int numDimensions = itk_image->GetImageDimension();

    // set size
    typename ImageType::IndexType start;
    for ( int d = 0; d < numDimensions; d++ )
        start[d] = 0;
    
    typename ImageType::SizeType size;
    for ( int d = 0; d < numDimensions; d++ )
        size[d] = dim[d];

    typename ImageType::RegionType region;
    region.SetSize( size );
    region.SetIndex( start );
    itk_image->SetRegions( region );
    itk_image->Allocate();
        
    // set pixelSize
    typename ImageType::SpacingType spacingITK;
    for ( int d = 0; d< numDimensions; d++ )
        spacingITK[d] = pixelSize[d];
    itk_image->SetSpacing( spacingITK );

    // copy orientation
    typename ImageType::DirectionType directionITK;
    for ( int i = 0; i < numDimensions; i++ )
        for ( int j = 0; j < numDimensions; j++ )
            directionITK[i][j] = orientation[index(i,j,numDimensions,numDimensions)];
    itk_image->SetDirection( directionITK );
    
    // copy origin
    itk_image->SetOrigin( origin );
}

// Returning ITK images from functions
// http://www.itk.org/pipermail/insight-users/2004-November/011095.html
//
// Passing ITK images to functions
//http://www.itk.org/pipermail/insight-users/2010-August/037768.html

template < class PixelType, int numDimensions >
void  py2itk( itk::Image< PixelType, numDimensions >* itk_image,
              PixelType *img,
              double* pixelSize,
              double* orientation,
              double* origin,
              int* dim ) {

    typedef typename itk::Image< PixelType,numDimensions> ImageType;

    set_attributes( itk_image,
                    pixelSize,
                    orientation,
                    origin,
                    dim );

    // copy pixel data
    PixelType* data = itk_image->GetBufferPointer();
    size_t nb_pixels = 1;
    for ( int d = 0; d < numDimensions; d++ )
        nb_pixels *= dim[d];
    
    for ( size_t i = 0; i < nb_pixels; i++ )
        data[i] = img[i];
}

template < class PixelType, class ComponentType, int numDimensions >
void  py2itk_compound( itk::Image< PixelType, numDimensions >* itk_image,
              ComponentType *img,
              double* pixelSize,
              double* orientation,
              double* origin,
              int* dim ) {

    typedef typename itk::Image< PixelType,numDimensions> ImageType;

    set_attributes( itk_image,
                    pixelSize,
                    orientation,
                    origin,
                    dim );

    // copy pixel data
    PixelType* data = itk_image->GetBufferPointer();
    size_t nb_pixels = 1;
    for ( int d = 0; d < numDimensions; d++ )
        nb_pixels *= dim[d];

    int numComponents = itk_image->GetNumberOfComponentsPerPixel();
    size_t k = 0;
    for ( size_t i = 0; i < nb_pixels; i++ )
        for ( int j = 0; j < numComponents; j++ ) {
            data[i][j] = img[k];
            k++;
        }
}

template < class PixelType, int numDimensions >
void itk2py( itk::Image<PixelType, numDimensions>* itk_image,
             PixelType *img,
             double* pixelSize,
             double* orientation,
             double* origin,
             int* dim ) {

    typedef itk::Image< PixelType, numDimensions > ImageType;
    
    get_attributes( itk_image,
                    pixelSize,
                    orientation,
                    origin,
                    dim );
					
	// debugging
	// typedef  itk::ImageFileWriter< ImageType  > WriterType;
	// typename WriterType::Pointer writer = WriterType::New();
	// writer->SetFileName("debug_itk2py.nii");
	// writer->SetInput(itk_image);
	// writer->Update();

    // copy pixel data
     PixelType* data = itk_image->GetBufferPointer(); 
     size_t nb_pixels = 1; 
     for ( int d = 0; d < numDimensions; d++ )
         nb_pixels *= dim[d]; 

	 memcpy(img, data, nb_pixels*sizeof(PixelType));
}





/* template < class PixelType, */
/*     class ComponentType, */
/*     int numDimensions, */
/*     int numComponents > */
/* void _imread( char* filename, */
/*               ComponentType* img, */
/*               double* pixelSize, */
/*               double* orientation, */
/*               double* origin, */
/*               int* dim ) { */
    
/*     typedef itk::Image< PixelType, numDimensions > ImageType; */
/*     typedef itk::ImageFileReader<ImageType> ReaderType; */
 
/*     typename ReaderType::Pointer reader = ReaderType::New(); */
/*     reader->SetFileName( filename ); */
/*     reader->Update(); */

/*     itk2py< PixelType, */
/*         ComponentType, */
/*         numDimensions, */
/*         numComponents >( reader->GetOutput(), */
/*                          img, */
/*                          pixelSize, */
/*                          orientation, */
/*                          origin, */
/*                          dim ); */
/*     } */

/* void _imread_rgb_uchar( char* filename, */
/*                         uchar* img, */
/*                         double* pixelSize, */
/*                         double* orientation, */
/*                         double* origin, */
/*                         int* dim ) { */
/*     _imread< itk::RGBPixel<uchar>,uchar,2,3>( filename, */
/*                                               img, */
/*                                               pixelSize, */
/*                                               orientation, */
/*                                               origin, */
/*                                               dim ); */
/* } */

template < class PixelType,
    int numDimensions >
void _imread( char* filename,
              PixelType* img ) {
    
    typedef itk::Image< PixelType, numDimensions > ImageType;
    typedef itk::ImageFileReader<ImageType> ReaderType;

    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( filename );
        
    try {
        reader->Update();
    }
    catch ( itk::ExceptionObject & err ) {
        std::cerr << "ITK ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
    }

    typename ImageType::Pointer itk_image = reader->GetOutput();
    typename ImageType::SizeType size = itk_image->GetBufferedRegion().GetSize();

    PixelType* data = itk_image->GetBufferPointer();
    size_t nb_pixels = 1;
    for ( int d = 0; d < numDimensions; d++ )
        nb_pixels *= size[d];

    // copy pixel data
    for ( size_t i = 0; i < nb_pixels; i++ )
        img[i] = data[i];
}

template < class PixelType,
    class ComponentType,
    int numDimensions >
void _imread_compound( char* filename,
                       ComponentType* img ) {
    
    typedef itk::Image< PixelType, numDimensions > ImageType;
    typedef itk::ImageFileReader<ImageType> ReaderType;

    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( filename );
        
    try {
        reader->Update();
    }
    catch ( itk::ExceptionObject & err ) {
        std::cerr << "ITK ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
    }

    typename ImageType::Pointer itk_image = reader->GetOutput();
    typename ImageType::SizeType size = itk_image->GetBufferedRegion().GetSize();

    PixelType* data = itk_image->GetBufferPointer();
    size_t nb_pixels = 1;
    for ( int d = 0; d < numDimensions; d++ )
        nb_pixels *= size[d];

    // copy pixel data
    int numComponents = itk_image->GetNumberOfComponentsPerPixel();
    size_t k = 0;
    for ( size_t i = 0; i < nb_pixels; i++ )
        for ( int j = 0; j < numComponents; j++ ) {
            /* img[index(i,j, nb_pixels,numComponents)] */
            /*         = data[i][j]; */
                        img[k]
                            = data[i][j];k++;
        }
    
    /* for ( int i = 0; i < numDimensions; i++ ) */
    /*     for ( int j = 0; j < nb_pixels; j++ ) */
    /*         for ( int k = 0; k < numComponents; k++ ) //{             */
    /*             img[index(i,j,k,numDimensions,numDimensions,numComponents)] */
    /*                 = data[index(i,j,numDimensions,numDimensions)][k]; */
            //k++;
            //}
}

template < class PixelType,
    int numDimensions >
void _imwrite( char* filename,
              PixelType* img,
              double* pixelSize,
              double* orientation,
              double* origin,
              int* dim ) {
    typedef typename itk::Image< PixelType,numDimensions> ImageType;
    typename ImageType::Pointer itk_image = ImageType::New();

    py2itk<PixelType,numDimensions>( itk_image,
                                     img,
                                     pixelSize,
                                     orientation,
                                     origin,
                                     dim );

    typedef  itk::ImageFileWriter< ImageType  > WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( filename );
    writer->SetInput( itk_image );

    try {
        writer->Update();
    }
    catch ( itk::ExceptionObject & err ) {
        std::cerr << "ITK ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
    }
}

#endif
