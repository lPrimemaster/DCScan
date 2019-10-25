import sys
from PyQt5 import uic
fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')
uic.compileUi(fin,fout,execute=False)
fin.close()
fout.close()