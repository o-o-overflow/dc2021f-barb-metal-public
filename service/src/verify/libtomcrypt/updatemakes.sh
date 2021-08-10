#!/bin/bash

./helper.pl --update-makefiles || exit 1

makefiles=(makefile makefile_include.mk makefile.shared makefile.unix makefile.mingw makefile.msvc)
vcproj=(libtomcrypt_VS2008.vcproj)

if [ $# -eq 1 ] && [ "$1" == "-c" ]; then
  git add ${makefiles[@]} ${vcproj[@]} doc/Doxyfile && git commit -m 'Update makefiles'
fi

exit 0

# ref:         HEAD -> master, origin/master, origin/HEAD
# git commit:  26edbfa0164e8efed06197be0dff225945834640
# commit time: 2021-08-10 11:33:39 +0200
