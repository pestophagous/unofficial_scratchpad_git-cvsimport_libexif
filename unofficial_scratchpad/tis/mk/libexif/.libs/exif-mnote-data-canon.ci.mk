-include mk/libexif/.libs/exif-mnote-data-canon.ci.d

pp/libexif/.libs/exif-mnote-data-canon.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/libexif/.libs/exif-mnote-data-canon.ci.d" "-DHAVE_CONFIG_H" "-Isrc/libexif" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/libexif/exif-mnote-data-canon.lo" "-MD" "-MP" "-c" "src/libexif/canon/exif-mnote-data-canon.c" "-fPIC" "-DPIC" "-o" "pp/libexif/.libs/exif-mnote-data-canon.ci"
	sed -i "s/^[^ ]*:/pp\/libexif\/.libs\/exif-mnote-data-canon.ci:/g" mk/libexif/.libs/exif-mnote-data-canon.ci.d
