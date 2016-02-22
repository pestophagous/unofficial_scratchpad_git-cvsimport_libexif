#!/bin/bash

cd ../libexif/

(tis-analyzer-gui -64 -val \
    -cpp-extra-args="-D_= -DN_= -isystem '/home/user/m/exif/github_libexif'" \
    -slevel 1000 *c 2>&1 ) | grep -i -E 'ER.OR|w.Rn|garbl'

cd ../unofficial_scratchpad/
