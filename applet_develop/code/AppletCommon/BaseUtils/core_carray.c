/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

const unsigned char g_hydra_letter_digit[256] = 
{
    [0 ... 255] = 0,
    ['a' ... 'z' ] = 1,
    ['A' ... 'Z' ] = 1,
    ['0' ... '9' ] = 1,
};

const unsigned char g_hydra_space[256] = 
{
    [0 ... 255] = 0,
    [' ']  = 1,
    ['\t'] = 1,
    ['\n'] = 1,
    ['\r'] = 1,
    ['\f'] = 1,
    ['\b'] = 1,
};

const char g_hydra_hexadecimal[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f'
};
