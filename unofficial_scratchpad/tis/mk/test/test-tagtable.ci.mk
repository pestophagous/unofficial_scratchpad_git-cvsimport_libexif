-include mk/test/test-tagtable.ci.d

pp/test/test-tagtable.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/test/test-tagtable.ci.d" "-DHAVE_CONFIG_H" "-Isrc/test" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/test/test-tagtable.o" "-MD" "-MP" "-c" "-o" "pp/test/test-tagtable.ci" "src/test/test-tagtable.c"
	sed -i "s/^[^ ]*:/pp\/test\/test-tagtable.ci:/g" mk/test/test-tagtable.ci.d
