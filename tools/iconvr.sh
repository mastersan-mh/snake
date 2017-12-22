#!/bin/sh

SELF=$0

if [ -n "$1" ] ; then

  for filename in $* ; do
    echo "filename = $filename"
    iconv -f CP866 -t UTF-8 ${filename} -o ./out866/${filename}
    iconv -f WINDOWS-1251 -t UTF-8 ${filename} -o ./out1251/${filename}
  done
  exit 0
fi

mkdir out866 out1251
find -name "*.c" -exec ${SELF} {} +
find -name "*.h" -exec ${SELF} {} +
find -name "*.C" -exec ${SELF} {} +
find -name "*.H" -exec ${SELF} {} +
find -name "*.CPP" -exec ${SELF} {} +
