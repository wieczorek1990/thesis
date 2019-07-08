#!/usr/bin/python
import sys
from gamera.core import *
init_gamera()
from gamera import knn
if len(sys.argv) != 3:
    print 'error'
    sys.exit(1)
img = sys.argv[1]
xml = sys.argv[2]
classifier = knn.kNNNonInteractive(xml, ['moments', 'nholes', 'nholes_extended', 'volume', 'zernike_moments'])
image = load_image(img)
onebit = image.to_onebit()
ccs = onebit.cc_analysis()
classifier.classify_list_automatic(ccs)
if len(ccs) == 1 and len(ccs[0].id_name) == 1:
    print ccs[0].id_name[0][1]
else:
    print 'error'
