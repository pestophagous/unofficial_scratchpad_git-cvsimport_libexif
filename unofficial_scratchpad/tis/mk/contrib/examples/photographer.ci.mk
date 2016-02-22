-include mk/contrib/examples/photographer.ci.d

pp/contrib/examples/photographer.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/contrib/examples/photographer.ci.d" "-DHAVE_CONFIG_H" "-Isrc/contrib/examples" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/contrib/examples/photographer.o" "-MD" "-MP" "-c" "-o" "pp/contrib/examples/photographer.ci" "src/contrib/examples/photographer.c"
	sed -i "s/^[^ ]*:/pp\/contrib\/examples\/photographer.ci:/g" mk/contrib/examples/photographer.ci.d
