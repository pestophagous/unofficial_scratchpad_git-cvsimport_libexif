
//#include "libexif/exif-entry.h"

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
    return (rand() % 26) + 65;
}

int Frama_C_interval(int min, int max)
{
    return 1;
}

#endif // __FRAMAC__

#include "which_main.h"

int main_strncat_1(void)
{
    size_t ii = 0;

    unsigned char test_buff[10];
    for ( ii = 0; ii < 10; ii++ )
        test_buff[ii] = Frama_C_unsigned_char_interval( 1, 0xFF );

    test_buff[10-2] = Frama_C_unsigned_char_interval( 0,1 );
    test_buff[10-1] = 0;

    // for strncat: the size of dest must be at least strlen(dest)+n+1.

    strncat (test_buff, " ", (9) - strlen(test_buff));

    return 0;
}
