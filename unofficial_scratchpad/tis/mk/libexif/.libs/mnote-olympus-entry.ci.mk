-include mk/libexif/.libs/mnote-olympus-entry.ci.d

pp/libexif/.libs/mnote-olympus-entry.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/libexif/.libs/mnote-olympus-entry.ci.d" "-DHAVE_CONFIG_H" "-Isrc/libexif" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/libexif/mnote-olympus-entry.lo" "-MD" "-MP" "-c" "src/libexif/olympus/mnote-olympus-entry.c" "-fPIC" "-DPIC" "-o" "pp/libexif/.libs/mnote-olympus-entry.ci"
	sed -i "s/^[^ ]*:/pp\/libexif\/.libs\/mnote-olympus-entry.ci:/g" mk/libexif/.libs/mnote-olympus-entry.ci.d
