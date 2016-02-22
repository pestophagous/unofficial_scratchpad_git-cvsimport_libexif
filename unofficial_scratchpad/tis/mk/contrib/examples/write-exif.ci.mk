-include mk/contrib/examples/write-exif.ci.d

pp/contrib/examples/write-exif.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/contrib/examples/write-exif.ci.d" "-DHAVE_CONFIG_H" "-Isrc/contrib/examples" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/contrib/examples/write-exif.o" "-MD" "-MP" "-c" "-o" "pp/contrib/examples/write-exif.ci" "src/contrib/examples/write-exif.c"
	sed -i "s/^[^ ]*:/pp\/contrib\/examples\/write-exif.ci:/g" mk/contrib/examples/write-exif.ci.d
