#!/bin/bash

LOGFILE=log.txt
ERRFILE=err.txt

./configure 1>${LOGFILE} 2>${ERRFILE} && \
make        1>${LOGFILE} 2>${ERRFILE}

if [ "$?" = 0 ] ;
then
  echo "OK"
else
  tail ${ERRFILE} 
  echo "ERROR while build"
fi

