
#include "libexif/exif-entry.h"

#include <stdio.h>       // printf
#include <string.h>      // memset

int main(void)
{
    ExifData one_jpeg;
    one_jpeg.data = 0;
    one_jpeg.size = 0;
    one_jpeg.priv = 0;

    // ExifContent->entries[n] holds items of type ExifEntry
    ExifContent an_idf;
    an_idf.entries = 0;
    an_idf.count = 64; // not yet used in this test (on feb 20). count of entries?
    an_idf.parent = &one_jpeg;
    an_idf.priv = 0;

    unsigned char tag_data_buffer[400];  // add nondeterminism to this buffer.
    unsigned char some_char = 0xFF;
    memset(tag_data_buffer, some_char, 400);
    some_char = 0x22;
    /* tag_data_buffer[0] = 0xFF; */
    tag_data_buffer[1] = some_char;
    /* tag_data_buffer[2] = 0xFF; */

    ExifEntry the_entries[3];
    the_entries[0].tag = EXIF_TAG_ISO_SPEED_RATINGS;   // ExifTag. 0-0xa500(42240). try 0-65535
    the_entries[0].format = EXIF_FORMAT_SHORT;     // ExifFormat. valid 1-12. try 0-20
    the_entries[0].components = 1; // unsigned long (1 for non-array types). try 0-20.

    the_entries[0].size = 2;  // 1,2,4, or 8 (i think). try 0-20.
    the_entries[0].data = tag_data_buffer;

    the_entries[0].parent = &an_idf;
    the_entries[0].priv = 0;

    the_entries[1].tag = EXIF_TAG_XP_AUTHOR;   // ExifTag. 0-0xa500(42240). try 0-65535
    the_entries[1].format = EXIF_FORMAT_BYTE;     // ExifFormat. valid 1-12. try 0-20
    the_entries[1].components = 1; // unsigned long (1 for non-array types). try 0-20.

    the_entries[1].size = 1;  // 1,2,4, or 8 (i think). try 0-20.
    the_entries[1].data = tag_data_buffer;

    the_entries[1].parent = &an_idf;
    the_entries[1].priv = 0;

    the_entries[2].tag = EXIF_TAG_EXPOSURE_TIME;   // ExifTag. 0-0xa500(42240). try 0-65535
    the_entries[2].format = EXIF_FORMAT_RATIONAL;     // ExifFormat. valid 1-12. try 0-20
    the_entries[2].components = 1; // unsigned long (1 for non-array types). try 0-20.

    the_entries[2].size = 8;  // 1,2,4, or 8 (i think). try 0-20.
    the_entries[2].data = tag_data_buffer;

    the_entries[2].parent = &an_idf;
    the_entries[2].priv = 0;

    an_idf.entries = the_entries;

    one_jpeg.ifd[0] = &an_idf;
    one_jpeg.ifd[1] = &an_idf;
    one_jpeg.ifd[2] = &an_idf;
    one_jpeg.ifd[3] = &an_idf;
    one_jpeg.ifd[4] = &an_idf;

    char valout[200];

    exif_entry_get_value(&the_entries[0], valout, 200);
    printf("output of exif_entry_get_value: %s\n", valout);

    exif_entry_get_value(&the_entries[1], valout, 200);
    printf("output of exif_entry_get_value: %s\n", valout);

    exif_entry_get_value(&the_entries[2], valout, 200);
    printf("output of exif_entry_get_value: %s\n", valout);

    char ten_chars[10];
    memset(ten_chars, 0, 10);
    ten_chars[0] = 'h';
    ten_chars[1] = 'i';

    char other_chars[4] = "-o.";

    printf("ten_chars: %s\n", ten_chars);
    strncat(ten_chars, other_chars, 9-2);
    printf("ten_chars: %s\n", ten_chars);

    char ten_more_chars[10];
    memset(ten_more_chars, 0, 10);
    ten_more_chars[0] = 'h';
    ten_more_chars[1] = 'i';

    printf("ten_more_chars: %s\n", ten_more_chars);
    strncat(ten_more_chars, "=a!", 9-2);
    printf("ten_more_chars: %s\n", ten_more_chars);

    return 0;
}
