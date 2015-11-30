cmake_minimum_required(VERSION 2.6)

project(PyITK)

find_package(ITK REQUIRED)
if (USE_ITK_FILE)
  include(${USE_ITK_FILE})
endif(USE_ITK_FILE)

find_package(PythonLibs REQUIRED)
if(USE_PYTHONLIBS_FILE)
  include(${USE_PYTHONLIBS_FILE})
endif(USE_PYTHONLIBS_FILE)

find_package(PythonInterp REQUIRED)
if(USE_PYTHONINTERP_FILE)
  include(${USE_PYTHONINTERP_FILE})
endif(USE_PYTHONINTERP_FILE)

#get numpy include path
execute_process(
  COMMAND
  ${PYTHON_EXECUTABLE} -c "import numpy; print numpy.get_include()"
  OUTPUT_VARIABLE NUMPY_INCLUDE_PATH
  RESULT_VARIABLE NUMPY_ERR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
if(NUMPY_ERR)
  message(SEND_ERROR "WARNING: NumPy header not found.")
endif(NUMPY_ERR)
set(PYTHON_INCLUDE_PATH ${PYTHON_INCLUDE_PATH} ${NUMPY_INCLUDE_PATH})
 


#TODO: Windows? Need a FindCython.cmake
find_program(CYTHON_EXECUTABLE cython /usr/bin /usr/local/bin)

#generate templates
execute_process(
  COMMAND
  ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/generate_templates.py ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
  OUTPUT_VARIABLE PYTHON_OUTPUT
  ERROR_VARIABLE PYTHON_ERROR
  RESULT_VARIABLE PYTHON_ERR
  )
if(PYTHON_OUTPUT)
  message(${PYTHON_OUTPUT})
endif(PYTHON_OUTPUT)
if(PYTHON_ERR)
  message(SEND_ERROR ${PYTHON_ERROR})
endif(PYTHON_ERR)

set( CPP_SRCS "src/itk2cython.cpp"
  "src/filters.cpp"
  "${CMAKE_BINARY_DIR}/tmp/templates.cpp"
  )

include_directories(${PYTHON_INCLUDE_PATH})
include_directories("include")
include_directories(${CMAKE_BINARY_DIR}"/tmp")

#run cython on all pyx files
execute_process(
  COMMAND
  ${CYTHON_EXECUTABLE} --cplus ${CMAKE_BINARY_DIR}/tmp/_itk.pyx -o ${CMAKE_BINARY_DIR}/tmp/_itk.cpp
  OUTPUT_VARIABLE CYTHON_OUTPUT
  ERROR_VARIABLE CYTHON_ERROR
  RESULT_VARIABLE CYTHON_ERR
  )
if(CYTHON_ERR)
  message(SEND_ERROR ${CYTHON_ERROR})
endif(CYTHON_ERR)
  
set(CPP_SRCS ${CPP_SRCS} ${CMAKE_BINARY_DIR}/tmp/_itk.cpp)
  
add_library( "_itk" MODULE ${CPP_SRCS})
target_link_libraries( "_itk" ${ITK_LIBRARIES} ${PYTHON_LIBRARY})
set_target_properties("_itk" PROPERTIES PREFIX "" LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})


