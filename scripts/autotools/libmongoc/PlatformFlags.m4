dnl Ignore OpenSSL deprecation warnings on OSX
AS_IF([test "$os_darwin" = "yes"],
      [AX_CHECK_COMPILE_FLAG([-Wno-deprecated-declarations], [STD_CFLAGS="$STD_CFLAGS -Wno-deprecated-declarations"])])

dnl We know there are some cast-align issues on OSX
AS_IF([test "$os_darwin" = "yes"],
      [AX_CHECK_COMPILE_FLAG([-Wno-cast-align], [STD_CFLAGS="$STD_CFLAGS -Wno-cast-align"])])
AS_IF([test "$os_darwin" = "yes"],
      [AX_CHECK_COMPILE_FLAG([-Wno-unneeded-internal-declaration], [STD_CFLAGS="$STD_CFLAGS -Wno-unneeded-internal-declaration"])])
AS_IF([test "$os_darwin" = "yes"],
      [AX_CHECK_COMPILE_FLAG([-Wno-error=unused-command-line-argument], [STD_CFLAGS="$STD_CFLAGS -Wno-error=unused-command-line-argument"])])

dnl Enable non-standard features on FreeBSD with __BSD_VISIBLE=1
if test "$os_freebsd" = "yes"; then
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS -D__BSD_VISIBLE=1"
fi
