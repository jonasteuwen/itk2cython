#!/usr/bin/python

import sys
import os
import fileinput
from string import Template

source_dir = sys.argv[1]
build_dir = sys.argv[2]

types = {
    "char" : "int8",
    "uchar" : "uint8",
    "short" : "int16",
    "ushort" : "uint16",
    "int" : "int32",
    "uint" : "uint32",
    "float" : "float32",
    "double" : "float64"
    }

compound_types = {
    "rgb" : "itk::RGBPixel< $type >",
    "rgba" : "itk::RGBAPixel< $type >"
    }

dims = [2,3,4]

header_cpp = [
    """void _imread_${type}_${dim}( char* filename,
                    ${type}* img );
""",

    """void _imread_${compoundType}_${type}_${dim}( char* filename,
                    ${type}* img );
""",

    """void _imwrite_${type}_${dim}( char* filename,
                     ${type}* img,
                     double* pixelSize,
                     double* orientation,
                     double* origin,
                     int* dim );
""" ]

templates_cpp = [
    """
void _imread_${type}_${dim}( char* filename,
                             ${type}* img ) {
    _imread<${type},${dim}>( filename,
                      img );
}
""",
    """
void _imread_${compoundType}_${type}_${dim}( char* filename,
                                             ${type}* img ) {
    _imread_compound<${compoundTypeITK},${type},${dim}>( filename,
                                                      img );
}
""",
    """
void _imwrite_${type}_${dim}( char* filename,
                     ${type}* img,
                     double* pixelSize,
                     double* orientation,
                     double* origin,
                     int* dim ) {
    _imwrite<${type},${dim}>( filename,
                       img,
                       pixelSize,
                       orientation,
                       origin,
                       dim );
}
""" ]

templates_py = [
"""
def imread_${type}_${dim}( bytes py_string, shape ):
    cdef char* c_string = py_string
    cdef np.ndarray[${type}, ndim=${dim},  mode="c"] img = np.empty( shape,
                                                              dtype="${pytype}" )
    _imread_${type}_${dim}( c_string,
                   <${type}*> img.data )
    return img
""",

"""
def imread_${compoundType}_${type}_${dim}( bytes py_string, shape ):
    cdef char* c_string = py_string
    cdef np.ndarray[${type}, ndim=${dimPlusOne},  mode="c"] img = np.empty( shape,
                                                              dtype="${pytype}" )
    _imread_${compoundType}_${type}_${dim}( c_string,
                   <${type}*> img.data )
    return img
""",

"""
def imwrite_${type}_${dim}( bytes py_string,
                            np.ndarray[${type}, ndim=${dim},  mode="c"] img,
                            header ):
    cdef np.ndarray[double, ndim=1,  mode="c"] pixelSize = header['pixelSize']
    cdef np.ndarray[double, ndim=1,  mode="c"] orientation = header['orientation']
    cdef np.ndarray[double, ndim=1,  mode="c"] origin = header['origin']
    cdef np.ndarray[int, ndim=1,  mode="c"] dim =  header['dim']

    cdef char* c_string = py_string
    _imwrite_${type}_${dim}( c_string,
                <${type}*> img.data,
                <double*> pixelSize.data,
                <double*> orientation.data,
                <double*> origin.data,
                <int*> dim.data )
""" ]

tmp_dir = build_dir + "/tmp"
if not os.path.exists( tmp_dir ):
    os.makedirs( tmp_dir )

def wrap_cpp(t,f):
    s = Template(t)
    for dim in dims:
        for type in types.keys():
            if "compound" not in t:
                f.write( s.substitute( type=type, dim=str(dim)) )
            else:
                for compound_type in compound_types.keys():
                    s2 = Template( compound_types[compound_type] )
                    f.write( s.substitute( type=type,
                                           dim=str(dim),
                                           compoundType=compound_type,
                                           compoundTypeITK=s2.substitute(type=type)))
            f.write("\n")
        f.write("\n")

f = open( tmp_dir + "/templates.h",'wb')
f.write("""#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "itk2cython.h"\n\n""")

for t in header_cpp:
    wrap_cpp(t,f)

f.write("""#endif""")
f.close()

f = open( tmp_dir + "/templates.cpp",'wb')
f.write("""#include "templates.h"\n""")

for t in templates_cpp:
    wrap_cpp(t,f)

f.close()

f = open( tmp_dir + "/_templates.pyx",'wb')
f.write(
 """########## Automatically generated templated code ##########

cdef extern from "templates.h":\n""")

for t in header_cpp:
    s = Template( "    " + t.replace( ';', '' ) )
    if "compound" not in t:
        for type in types:
            for dim in dims:
                f.write( s.substitute( type=type, dim=str(dim)) )
                f.write("\n")
        f.write("\n")
    else:
        for compound in compound_types:
            s2 = Template( compound_types[compound] )
            for type in types:
                for dim in dims:
                    f.write( s.substitute( type=type,
                                           dim=str(dim),
                                           compoundType=compound,
                                           compoundTypeITK=s2.substitute(type=type)
                                           )
                             )
                    f.write("\n")
        f.write("\n")

f.write("\n")

for t in templates_py:
    s = Template(t)
    if "compound" not in t:
        for type in types:
            for dim in dims:
                f.write( s.substitute( type=type, dim=str(dim), pytype=types[type] ) )
                f.write("\n")
        f.write("\n")
    else:
        for compound in compound_types:
            s2 = Template( compound_types[compound] )
            for type in types:
                for dim in dims:
                    f.write( s.substitute( type=type,
                                           dim=str(dim),
                                           dimPlusOne=str(dim+1),
                                           compoundType=compound,
                                           compoundTypeITK=s2.substitute(type=type),
                                           pytype=types[type]
                                           )
                             )
                    f.write("\n")
        f.write("\n")

f.close()

# Concatenate automatically generated files
with open(tmp_dir + "/_itk.pyx", 'w') as fout:
    for line in fileinput.input([ source_dir + "/src/_itk.pyx",
                                  tmp_dir + "/_templates.pyx"
                                  ]):
        fout.write(line)
    fout.close()
