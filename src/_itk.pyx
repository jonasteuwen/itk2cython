# distutils: language = c++

import numpy as np
cimport numpy as np

from libcpp.string cimport string

np.import_array()

ctypedef unsigned char uchar
ctypedef unsigned short ushort
ctypedef unsigned int uint

cdef extern from "itk2cython.h":
     void _query_image( char* filename,
                        string& componentType,
                        int& numDimensions,
                        int& numComponents )
     void _get_header( char* filename,
                       double* pixelSize,
                       double* orientation,
                       double* origin,
                       int* dim )

def query_image( bytes py_string ):
    cdef char* c_string = py_string

    cdef string componentType
    cdef int numComponents
    cdef int numDimensions

    _query_image( c_string,
                  componentType,
                  numDimensions,
                  numComponents )

    return ( componentType,
             numDimensions,
             numComponents )

def get_header( bytes py_string ):
    cdef char* c_string = py_string

    cdef string componentType
    cdef int numDimensions
    cdef int numComponents

    _query_image( c_string,
                  componentType,
                  numDimensions,
                  numComponents )

    cdef np.ndarray[double, ndim=1,  mode="c"] pixelSize = np.zeros( numDimensions, dtype="float64" )
    cdef np.ndarray[double, ndim=2,  mode="c"] orientation = np.zeros( (numDimensions,numDimensions), dtype="float64" )
    cdef np.ndarray[double, ndim=1,  mode="c"] origin = np.zeros( numDimensions, dtype="float64" )
    cdef np.ndarray[int, ndim=1,  mode="c"] dim = np.zeros( numDimensions, dtype="int32" )

    _get_header( c_string,
                 <double*> pixelSize.data,
                 <double*> orientation.data,
                 <double*> origin.data,
                 <int*> dim.data )

    header = dict()
    header['pixelSize'] = pixelSize
    header['orientation'] = orientation
    header['origin'] = origin
    header['dim'] = dim

    return header

cdef extern from "filters.h":
    void _AHE_float_3( float* img,
                      double* pixelSize,
                      double* orientation,
                      double* origin,
                      int* dim,
                      int radius )

def AHE_float_3( np.ndarray[float, ndim=3,  mode="c"] img,
                 header,
                 int radius ):
    cdef np.ndarray[double, ndim=1,  mode="c"] pixelSize = header['pixelSize']
    cdef np.ndarray[double, ndim=1,  mode="c"] orientation = header['orientation']
    cdef np.ndarray[double, ndim=1,  mode="c"] origin = header['origin']
    cdef np.ndarray[int, ndim=1,  mode="c"] dim =  header['dim']

    _AHE_float_3( <float*> img.data,
                   <double*> pixelSize.data,
                   <double*> orientation.data,
                   <double*> origin.data,
                   <int*> dim.data,
                   radius )
				   
    return img, header


