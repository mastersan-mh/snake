#!/bin/bash

LOGFILE=log.txt
ERRFILE=err.txt

INSTALLPATH=`realpath -m ./_installdir/usr/`
rm -Rf ${INSTALLPATH}
mkdir -p ${INSTALLPATH}

./configure --prefix=${INSTALLPATH} 1>${LOGFILE} 2>${ERRFILE} && \
make                                1>${LOGFILE} 2>${ERRFILE} && \
make install                        1>${LOGFILE} 2>${ERRFILE}

if [ "$?" = 0 ] ;
then
  echo "OK"
else
  tail ${ERRFILE} 
  echo "ERROR while build"
fi
