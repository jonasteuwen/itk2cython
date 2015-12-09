itk2cython
==========

This repository is a Cython and ITK exercise but is not the proper way to mix Python and ITK: see the official [ITK Python wrapper](http://www.itk.org/Wiki/ITK/Python_Wrapping), as well as the excellent [SimpleITK](http://www.simpleitk.org/).

This code has been inspired by [CyITK](https://github.com/lantiga/cyitk), and the ideas here have been further developed to shape the Python wrapper that I wrote for the [IRTK library](https://github.com/BioMedIA/python-irtk) from Imperial College London.


How to compile
--------------

Written for Python 2.7.

Install the ITK dependency:

	git clone https://github.com/InsightSoftwareConsortium/ITK.git
	cd ITK
	mkdir build
	cd build
	cmake -D BUILD_DOCUMENTATION:BOOL=OFF \
	      -D BUILD_EXAMPLES:BOOL=OFF \
		  -D BUILD_TESTING:BOOL=OFF \
		  -D CMAKE_INSTALL_PREFIX:PATH=~/anaconda/ \
		  -D BUILD_SHARED_LIBS:BOOL=ON ..
	make -j 4
	make install

Then build the `_itk` module:

	cd itk2cython
	mkdir build
	cd build
	cmake -D PYTHON_LIBRARY:FILEPATH=~/anaconda/lib/libpython2.7.dylib \
	      -D PYTHON_INCLUDE_DIR:PATH=~/anaconda/include/python2.7/ \
	      -D CMAKE_INSTALL_PREFIX:PATH=~/anaconda/ ..

You can use `make VERBOSE=1` to debug compilation issues.


Run the demo
------------

Get some data:

	cd itk2cython
	mkdir data
	cd data
	wget https://raw.githubusercontent.com/kevin-keraudren/crf-denoising/master/img/lena.png
	wget https://github.com/kevin-keraudren/example-motion-correction/raw/master/output_segmentation/very_large_stack-1.nii.gz
	cd ..

Run the demo:

    cd itk2python
	python test.py
