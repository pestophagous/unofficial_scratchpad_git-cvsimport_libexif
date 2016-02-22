-include mk/test/test-parse.ci.d

pp/test/test-parse.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/test/test-parse.ci.d" "-DHAVE_CONFIG_H" "-Isrc/test" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/test/test-parse.o" "-MD" "-MP" "-c" "-o" "pp/test/test-parse.ci" "src/test/test-parse.c"
	sed -i "s/^[^ ]*:/pp\/test\/test-parse.ci:/g" mk/test/test-parse.ci.d
