-include mk/libexif/.libs/exif-mnote-data-fuji.ci.d

pp/libexif/.libs/exif-mnote-data-fuji.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/libexif/.libs/exif-mnote-data-fuji.ci.d" "-DHAVE_CONFIG_H" "-Isrc/libexif" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/libexif/exif-mnote-data-fuji.lo" "-MD" "-MP" "-c" "src/libexif/fuji/exif-mnote-data-fuji.c" "-fPIC" "-DPIC" "-o" "pp/libexif/.libs/exif-mnote-data-fuji.ci"
	sed -i "s/^[^ ]*:/pp\/libexif\/.libs\/exif-mnote-data-fuji.ci:/g" mk/libexif/.libs/exif-mnote-data-fuji.ci.d
