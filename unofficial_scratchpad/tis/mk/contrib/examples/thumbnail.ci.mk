-include mk/contrib/examples/thumbnail.ci.d

pp/contrib/examples/thumbnail.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/contrib/examples/thumbnail.ci.d" "-DHAVE_CONFIG_H" "-Isrc/contrib/examples" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/contrib/examples/thumbnail.o" "-MD" "-MP" "-c" "-o" "pp/contrib/examples/thumbnail.ci" "src/contrib/examples/thumbnail.c"
	sed -i "s/^[^ ]*:/pp\/contrib\/examples\/thumbnail.ci:/g" mk/contrib/examples/thumbnail.ci.d
