-include mk/libexif/.libs/exif-log.ci.d

pp/libexif/.libs/exif-log.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/libexif/.libs/exif-log.ci.d" "-DHAVE_CONFIG_H" "-Isrc/libexif" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/libexif/exif-log.lo" "-MD" "-MP" "-c" "src/libexif/exif-log.c" "-fPIC" "-DPIC" "-o" "pp/libexif/.libs/exif-log.ci"
	sed -i "s/^[^ ]*:/pp\/libexif\/.libs\/exif-log.ci:/g" mk/libexif/.libs/exif-log.ci.d
