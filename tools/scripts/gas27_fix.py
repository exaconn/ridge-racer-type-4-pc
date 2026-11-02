import re,sys
p=sys.argv[1]; s=open(p).read()
s=re.sub(r'(\tlw\t\$31,\d+\(\$sp\)\n)\t#nop\n\t\.set\tnoreorder\n\t\.set\tnomacro\n\tj\t\$31\n(\taddu\t\$sp,\$sp,\d+\n)\t\.set\tmacro\n\t\.set\treorder\n', r'\1\2\tj\t$31\n\tnop\n', s)
open(p,'w').write(s)
