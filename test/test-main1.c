
#include "libexif/exif-entry.h"

#include <stdio.h>       // printf
#include <string.h>      // memset

#ifdef __FRAMAC__
#    include "__fc_string_axiomatic.h"
#    include "__fc_builtin.h"
#else

unsigned char
Frama_C_unsigned_char_interval(unsigned char min, unsigned char max);

int
Frama_C_interval(int min, int max);

unsigned char Frama_C_unsigned_char_interval(unsigned char min, unsigned char max)
{
    return 0xFF;
}

int Frama_C_interval(int min, int max)
{
    return 1;
}

#endif // __FRAMAC__

// the "I" in ICHARS stands for input. ExifEntry::data is part of the input to exif_entry_get_value
#define ICHARS_QTY 400
// the "O" in OCHARS is for 'output'.
#define OCHARS_QTY 200

int main(void)
{
    ExifData one_jpeg;
    one_jpeg.data = 0;
    one_jpeg.size = 0;
    one_jpeg.priv = 0;

    // ExifContent->entries[n] holds items of type ExifEntry
    ExifContent an_idf;
    an_idf.entries = 0;
    an_idf.count = 1;
    an_idf.parent = &one_jpeg;
    an_idf.priv = 0;

    unsigned char tag_data_buffer[ICHARS_QTY];  // add nondeterminism to this buffer.

    size_t ii = 0;
    for ( ii = 0; ii < ICHARS_QTY; ii++ )
    {
        tag_data_buffer[ii] = Frama_C_unsigned_char_interval( 0, 0xFF );
    }

    ExifEntry the_entries[1];
    /*
    the_entries[0].tag = EXIF_TAG_ISO_SPEED_RATINGS;   // ExifTag. 0-0xa500(42240). try 0-65535
    the_entries[0].format = EXIF_FORMAT_SHORT;     // ExifFormat. valid 1-12. try 0-20
    the_entries[0].components = 1; // unsigned long (1 for non-array types). try 0-20.

    the_entries[0].size = 2;  // 1,2,4, or 8 (i think). try 0-20.
    the_entries[0].data = tag_data_buffer;

    the_entries[0].parent = &an_idf;
    the_entries[0].priv = 0;
    */

    the_entries[0].tag = EXIF_TAG_COPYRIGHT;//Frama_C_interval(0, 65535);
    the_entries[0].format = Frama_C_interval(0, 20);
    the_entries[0].components = Frama_C_interval(0, 20);

    the_entries[0].size = Frama_C_interval(0, ICHARS_QTY);
    the_entries[0].data = tag_data_buffer;

    the_entries[0].parent = &an_idf;
    the_entries[0].priv = 0;

    /*
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
    */
    an_idf.entries = the_entries;

    one_jpeg.ifd[0] = &an_idf;
    one_jpeg.ifd[1] = &an_idf;
    one_jpeg.ifd[2] = &an_idf;
    one_jpeg.ifd[3] = &an_idf;
    one_jpeg.ifd[4] = &an_idf;

    char valout[OCHARS_QTY];

    exif_entry_get_value(&the_entries[0], valout, OCHARS_QTY);
    printf("output of exif_entry_get_value: %s\n", valout);

    /* exif_entry_get_value(&the_entries[1], valout, OCHARS_QTY); */
    /* printf("output of exif_entry_get_value: %s\n", valout); */

    /* exif_entry_get_value(&the_entries[2], valout, OCHARS_QTY); */
    /* printf("output of exif_entry_get_value: %s\n", valout); */

    return 0;
}
