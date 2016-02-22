-include mk/test/test-mem.ci.d

pp/test/test-mem.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/test/test-mem.ci.d" "-DHAVE_CONFIG_H" "-Isrc/test" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/test/test-mem.o" "-MD" "-MP" "-c" "-o" "pp/test/test-mem.ci" "src/test/test-mem.c"
	sed -i "s/^[^ ]*:/pp\/test\/test-mem.ci:/g" mk/test/test-mem.ci.d
