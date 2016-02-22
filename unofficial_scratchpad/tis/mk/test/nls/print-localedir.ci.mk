-include mk/test/nls/print-localedir.ci.d

pp/test/nls/print-localedir.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/test/nls/print-localedir.ci.d" "-DHAVE_CONFIG_H" "-Isrc/test/nls" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/test/nls/print-localedir.o" "-MD" "-MP" "-c" "-o" "pp/test/nls/print-localedir.ci" "src/test/nls/print-localedir.c"
	sed -i "s/^[^ ]*:/pp\/test\/nls\/print-localedir.ci:/g" mk/test/nls/print-localedir.ci.d
