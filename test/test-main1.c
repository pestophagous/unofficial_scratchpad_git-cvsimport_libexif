
#include "libexif/exif-entry.h"

#include <stdio.h>

void main(void)
{
    printf("a\n");

    ExifData one_jpeg;
    one_jpeg.data = 0;
    one_jpeg.size = 0;
    one_jpeg.priv = 0;

    // ExifContent->entries[n] holds items of type ExifEntry
    ExifContent an_idf;
    an_idf.entries = 0;
    an_idf.count = 6;
    an_idf.parent = &one_jpeg;
    an_idf.priv = 0;

    char fakebuf[12];

    ExifEntry the_entry;
    the_entry.tag = 1;        // ExifTag
    the_entry.format = 2;     // ExifFormat
    the_entry.components = 1; // unsigned long (1 for non-array types)

    the_entry.size = 12;
    the_entry.data = fakebuf;

    the_entry.parent = &an_idf;
    the_entry.priv = 0;
    char valout[10];

    exif_entry_get_value(&the_entry,valout, 10);
}
