#include "itk2cython.h"

void _query_image( char* filename,
                   std::string& componentType,
                   int& numDimensions,
                   int& numComponents ) {
 
    typedef itk::ImageIOBase::IOComponentType ScalarPixelType;
 
    itk::ImageIOBase::Pointer imageIO =
        itk::ImageIOFactory::CreateImageIO( filename,
                                            itk::ImageIOFactory::ReadMode);
    imageIO->SetFileName(filename);
    imageIO->ReadImageInformation();

    componentType = imageIO->GetComponentTypeAsString( imageIO->GetComponentType() );
    numDimensions =  imageIO->GetNumberOfDimensions();
    numComponents = imageIO->GetNumberOfComponents();
}

void _get_header( char* filename,
                  double* pixelSize,
                  double* orientation,
                  double* origin,
                  int* dim ) {
    typedef itk::ImageIOBase::IOComponentType ScalarPixelType;
 
    itk::ImageIOBase::Pointer imageIO =
        itk::ImageIOFactory::CreateImageIO( filename,
                                            itk::ImageIOFactory::ReadMode);
    imageIO->SetFileName(filename);
    imageIO->ReadImageInformation();

    int numDimensions =  imageIO->GetNumberOfDimensions();      
    for ( int d = 0; d < numDimensions; d++ ) {
        dim[d] = imageIO->GetDimensions(d);
        origin[d] = imageIO->GetOrigin(d);
        pixelSize[d] = imageIO->GetSpacing(d);
        std::vector<double> direction = imageIO->GetDirection(d);
        for ( int i = 0; i < numDimensions; i++ )
            orientation[index(i,d,numDimensions,numDimensions)] = direction[i];
    }
    
}
