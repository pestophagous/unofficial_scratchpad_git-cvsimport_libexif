-include mk/libexif/.libs/mnote-canon-tag.ci.d

pp/libexif/.libs/mnote-canon-tag.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/libexif/.libs/mnote-canon-tag.ci.d" "-DHAVE_CONFIG_H" "-Isrc/libexif" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/libexif/mnote-canon-tag.lo" "-MD" "-MP" "-c" "src/libexif/canon/mnote-canon-tag.c" "-fPIC" "-DPIC" "-o" "pp/libexif/.libs/mnote-canon-tag.ci"
	sed -i "s/^[^ ]*:/pp\/libexif\/.libs\/mnote-canon-tag.ci:/g" mk/libexif/.libs/mnote-canon-tag.ci.d
