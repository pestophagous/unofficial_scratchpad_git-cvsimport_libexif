#!/usr/bin/python

buffer_variable = 'val'
buffer_size = 14


result = ''

for nul_pos in range(0, buffer_size+1):

    inner_rslt = ''

    for pos in range(0, nul_pos):

        # this if-with-an-Exception is to document our INTENTIONAL off-by-one effect:
        if pos > (buffer_size-1):
            raise Exception('Nope')

        if (pos == nul_pos-1) and (pos == buffer_size-1):
            # remove ' && '
            pass
        elif (pos == nul_pos-1):
            inner_rslt += (buffer_variable + '[' + str(pos) + '] == 0 ||')
        else:
            inner_rslt += (buffer_variable + '[' + str(pos) + '] != 0 && ')

    result += inner_rslt + '\n'

print(result)
