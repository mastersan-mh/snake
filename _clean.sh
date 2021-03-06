#!/bin/sh

RM="rm -f"

make clean
${RM} -r ./*.cache
${RM} ./aclocal.m4
${RM} ./autoscan.log
${RM} ./compile
${RM} ./config.log
${RM} ./config.status
${RM} ./configure
${RM} ./configure.scan
${RM} ./depcomp
${RM} ./install-sh
${RM} ./missing

#clean config.h
${RM} ./src/stamp-h1
${RM} ./src/config.h
${RM} ./src/config.h.in
${RM} ./src/config.h.in~

${RM} ./Makefile
${RM} ./Makefile.in
${RM} ./src/Makefile
${RM} ./src/Makefile.in
${RM} -r ./src/.deps
${RM} -r ./_installdir
