# # distutils: language = c++

# import numpy as np
# cimport numpy as np

# from libcpp.string cimport string

# np.import_array()

# ctypedef unsigned char uchar
# ctypedef unsigned short ushort
# ctypedef unsigned int uint

cdef extern from "templates.h":
    void _imread_float( char* filename,
                        float* img )

def imread_float( bytes py_string, header ):
    cdef np.ndarray[int, ndim=1,  mode="c"] dim =  header['dim']    
    
    cdef char* c_string = py_string
    cdef np.ndarray[float, ndim=3,  mode="c"] img = np.empty( (dim[2],
                                                               dim[1],
                                                               dim[0]), dtype="float32" )
    _imread_float( c_string,
                   <float*> img.data )
    
    return img
