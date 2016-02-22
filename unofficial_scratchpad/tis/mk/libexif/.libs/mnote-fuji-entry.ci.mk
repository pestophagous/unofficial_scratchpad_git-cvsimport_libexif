-include mk/libexif/.libs/mnote-fuji-entry.ci.d

pp/libexif/.libs/mnote-fuji-entry.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/libexif/.libs/mnote-fuji-entry.ci.d" "-DHAVE_CONFIG_H" "-Isrc/libexif" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/libexif/mnote-fuji-entry.lo" "-MD" "-MP" "-c" "src/libexif/fuji/mnote-fuji-entry.c" "-fPIC" "-DPIC" "-o" "pp/libexif/.libs/mnote-fuji-entry.ci"
	sed -i "s/^[^ ]*:/pp\/libexif\/.libs\/mnote-fuji-entry.ci:/g" mk/libexif/.libs/mnote-fuji-entry.ci.d
