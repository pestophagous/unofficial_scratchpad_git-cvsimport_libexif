-include mk/libexif/.libs/exif-mnote-data-pentax.ci.d

pp/libexif/.libs/exif-mnote-data-pentax.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/libexif/.libs/exif-mnote-data-pentax.ci.d" "-DHAVE_CONFIG_H" "-Isrc/libexif" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/libexif/exif-mnote-data-pentax.lo" "-MD" "-MP" "-c" "src/libexif/pentax/exif-mnote-data-pentax.c" "-fPIC" "-DPIC" "-o" "pp/libexif/.libs/exif-mnote-data-pentax.ci"
	sed -i "s/^[^ ]*:/pp\/libexif\/.libs\/exif-mnote-data-pentax.ci:/g" mk/libexif/.libs/exif-mnote-data-pentax.ci.d
