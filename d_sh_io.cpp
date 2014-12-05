/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#include <d_sh_cpp/d_sh_obj.h>

static DSh_StrRef
printf_obj_fmt_cb(d_sh_uchar_t f, DSh_StrRef pad, DSh_StrRef flags,
        va_list ap) {
    DSh_StrRef rv = "";

    return rv;
}

int
DSh_IO::printf_obj(DSh_StrRef fmt, ...) {
    DSh_StrRef specs = "s";
    DSh_StrRef flags = "";
    va_list ap;

    va_start(ap, fmt);
    DSh_StrRef out = DSh_Str::format_cb(printf_obj_fmt_cb, specs,
        flags, ap, fmt);
    va_end(ap);

    return -1;
}

#define UNLESS(stuff) if (! stuff)
#define SAFE_SET_POINTER_VAL(ptr, val) if (ptr) { *(ptr) = val; }

/* if the only set bits are in the lower 7, then the byte sequence in utf-8 is the same as ascii */
#define UTF8_BYTE_IS_INVARIANT(v) (((unsigned char)v) < 0x80)

/* a continuation byte occurs in each byte after the first in a multibyte utf-8 sequence */
#define UTF8_IS_CONTINUATION_BYTE(v) ( ((unsigned char)v) >= 0x80 && ((unsigned char)v) <= 0xbf )

/* to be the starting byte in a multi-byte utf-8 sequences, the high two bits must be set */
#define UTF8_IS_START_BYTE(v) ( ((unsigned char)v) >= 0xc2 && ((unsigned char)v) <= 0xf4 )

/* if the only set bits are in the lower 7, then the byte sequence in utf-8 is the same as ascii */
#define UNICODE_IS_INVARIANT(v) (((d_sh_uchar_t)v) < 0x80)

/* the byte order mark is the code point 0xFEFF */
/* encoded as utf-8: "\xef\xbb\xbf" */
#define UNICODE_IS_BOM(v) ((v) == 0xFEFF);

int
DSh_IOFileHandle::unicode_from_utf8_bytes(d_sh_uchar_t *code_point,
    d_sh_io_next_byte_cb_t read_cb, void *cb_data, size_t *bytes_read) {

    d_sh_uchar_t this_octet;
    size_t expected_len = 0;
    size_t len = 0;
    unsigned char the_byte = 0;
    size_t amt_read = 0;
    
    SAFE_SET_POINTER_VAL(bytes_read, 0);
    
    if (read_cb(&the_byte, cb_data)) {
        /* error or eof */
        return 1;
    }
    amt_read++;
    this_octet = the_byte;
        
    if (UTF8_BYTE_IS_INVARIANT(this_octet)) {
        *code_point = this_octet;
        SAFE_SET_POINTER_VAL(bytes_read, amt_read);
        return 0;
    }

    /* the first byte should not be a continuation byte */
    if (UTF8_IS_CONTINUATION_BYTE(this_octet)) {
        return -1;
    }

    UNLESS (UTF8_IS_START_BYTE(this_octet)) {
        return -1;
    }

    /* compute the number of expected bytes and pull out the bits
       contributing to the code point
    */
    if ((this_octet & 0xf8) == 0xf0) {
        expected_len = 4;
        this_octet &= 0x07;
    }
    else if ((this_octet & 0xf0) == 0xe0) {
        expected_len = 3;
        this_octet &= 0x0f;
    }
    else if ((this_octet & 0xe0) == 0xc0) {
        expected_len = 2;
        this_octet &= 0x1f;
    }
    else {
        return -1;
    }

    /*
    if (buf_len < expected_len) {
        SAFE_SET_POINTER_VAL(ret_len, 0);
        return 0;
    }
    */

    /* now need to grab the rest of the bytes */
    /* grab the bits we want (mask with 0x3f) and OR it with the old value left shifted by 6 */

    *code_point = this_octet;
    len = expected_len - 1;

    while (len--) {
        if (read_cb(&the_byte, cb_data)) {
            /* error or eof */
            return -1;
        }
        amt_read++;
        this_octet = the_byte;

        UNLESS (UTF8_IS_CONTINUATION_BYTE(the_byte)) {
            return -1;
        }

        this_octet = (this_octet & 0x3f) | (*code_point << 6);

        /* FIXME: should check here for illegal vals? */

        *code_point = this_octet;
    }

    SAFE_SET_POINTER_VAL(bytes_read, amt_read);

    return 0;
}
