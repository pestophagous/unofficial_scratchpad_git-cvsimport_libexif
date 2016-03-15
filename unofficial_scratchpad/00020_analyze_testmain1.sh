#!/bin/bash

cd ../libexif/

(tis-analyzer-gui -64 -val \
    -cpp-extra-args="-D_= -DN_= -isystem '/home/user/m/exif/github_libexif'" \
    -slevel 1000 -val-stop-at-nth-alarm 1 *c 2>&1 ) | grep -i -E 'ERrOR|waRn|garbl|launch.the.gui'

cd ../unofficial_scratchpad/
