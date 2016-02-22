-include mk/test/test-value.ci.d

pp/test/test-value.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/test/test-value.ci.d" "-DHAVE_CONFIG_H" "-Isrc/test" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/test/test-value.o" "-MD" "-MP" "-c" "-o" "pp/test/test-value.ci" "src/test/test-value.c"
	sed -i "s/^[^ ]*:/pp\/test\/test-value.ci:/g" mk/test/test-value.ci.d
