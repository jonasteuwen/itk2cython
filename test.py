import _itk
header =_itk.get_header("5066_4.nii")
img = _itk.imread_float_3("5066_4.nii",header['dim'])
header['orientation'] = header['orientation'].flatten()
_itk.imwrite_float_3("toto.nii",_itk.AHE_float_3(img,header,5),header)
_itk.imwrite_float_3("toto.nii",img,header)

import _itk
header =_itk.get_header("slice.nii")
img = _itk.imread_float_3("slice.nii",header['dim'])
header['orientation'] = header['orientation'].flatten()
_itk.imwrite_float_3("toto.nii",_itk.AHE_float_3(img,header,1),header)
_itk.imwrite_float_3("toto.nii",img,header)

import _itk
header =_itk.get_header("small.png")
img = _itk.imread_float_2("small.png",header['dim'])
header['orientation'] = header['orientation'].flatten()
_itk.imwrite_float_2("toto.png",img,header)

import _itk
header =_itk.get_header("img.nii.gz")
img = _itk.imread_float_3("img.nii.gz",header['dim'])
header['orientation'] = header['orientation'].flatten()
_itk.imwrite_float_3("toto.nii",img,header)



