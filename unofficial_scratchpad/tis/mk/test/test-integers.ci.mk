-include mk/test/test-integers.ci.d

pp/test/test-integers.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/test/test-integers.ci.d" "-DHAVE_CONFIG_H" "-Isrc/test" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/test/test-integers.o" "-MD" "-MP" "-c" "-o" "pp/test/test-integers.ci" "src/test/test-integers.c"
	sed -i "s/^[^ ]*:/pp\/test\/test-integers.ci:/g" mk/test/test-integers.ci.d
