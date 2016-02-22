
#include "libexif/exif-entry.h"

void main(void)
{
    ExifEntry the_entry;
    char valout[10];

    exif_entry_get_value(&the_entry,valout, 10);
}
