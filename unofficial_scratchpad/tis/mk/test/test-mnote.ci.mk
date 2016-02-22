-include mk/test/test-mnote.ci.d

pp/test/test-mnote.ci:
	gcc $(CPP_ARGS) "-MD" "-MF" "mk/test/test-mnote.ci.d" "-DHAVE_CONFIG_H" "-Isrc/test" "-Isrc" "-DGETTEXT_PACKAGE=\"libexif-12\"" "-DLOCALEDIR=\"/home/user/m/exif/github_libexif/install/share/locale\"" "-Isrc" "-Isrc" "-g" "-Wall" "-Wmissing-declarations" "-Wmissing-prototypes" "-g" "-O0" "-MT" "src/test/test-mnote.o" "-MD" "-MP" "-c" "-o" "pp/test/test-mnote.ci" "src/test/test-mnote.c"
	sed -i "s/^[^ ]*:/pp\/test\/test-mnote.ci:/g" mk/test/test-mnote.ci.d
