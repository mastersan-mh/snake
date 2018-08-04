AC_PREREQ(2.69)
# AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_INIT([snake], [0.1b], [mastersan_mh@list.ru])
AM_CONFIG_HEADER(src/config.h)
AC_CONFIG_SRCDIR(src/main.c)

AM_INIT_AUTOMAKE([1.13 foreign no-define subdir-objects])

# Checks compiler
AC_PROG_CC

#
# ncursesw
#

AC_CHECK_HEADER([curses.h])

AC_CHECK_LIB([ncursesw], [initscr], [], [ AC_MSG_ERROR([Unable to find ncursesw]) ] )

if test "x$prefix" != xNONE; then
  SNAKEBASEDIR="$prefix/share/games/snake"
else
  SNAKEBASEDIR="$ac_default_prefix/share/games/snake"
fi

AC_DEFINE_UNQUOTED(SNAKEBASEDIR,"$SNAKEBASEDIR",[Path to install base directory])

#AC_CONFIG_HEADER(src/config.h)


AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT
