import _itk

header =_itk.get_header("data/very_large_stack-1.nii.gz")
img = _itk.imread_float_3("data/very_large_stack-1.nii.gz",header['dim'])
header['orientation'] = header['orientation'].flatten()
img, header = _itk.AHE_float_3(img,header,10)
_itk.imwrite_float_3("toto.nii",img,header)

header =_itk.get_header("data/lena.png")
img = _itk.imread_uchar_2("data/lena.png",header['dim'])
header['orientation'] = header['orientation'].flatten()
_itk.imwrite_uchar_2("toto.png",img,header)




