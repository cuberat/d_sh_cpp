/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#include <d_sh_cpp/d_sh_obj.h>

DSh_StrRef
format_callback(d_sh_uchar_t f, DSh_StrRef pad, DSh_StrRef flags,
        va_list ap) {

    fprintf(stdout, "format_callback: fmt_char=%u ('%c')\n", f, (int)f);
    fflush(stdout);
    fprintf(stdout, ", got pad="); fflush(stdout);
    pad->write_fp(stdout); fflush(stdout);
    fprintf(stdout, ", flags="); fflush(stdout);
    flags->write_fp(stdout); fflush(stdout);
    fprintf(stdout, "\n"); fflush(stdout);

    /* s = va_arg(ap, char *); */

    DSh_StrRef str = new DSh_Str("");
    return str;
}

int
main(int argc, char **argv) {
    DSh_Ref obj;
    DSh_StrRef str;
    DSh_StrRef str2;
    DSh_IORef io;
    unsigned char c = 0;

    obj = new DSh_Obj();
    str = new DSh_Str("foo\n");
    io = new DSh_IO(str);

    printf("DSh_IORef size = %zu\n", sizeof(io));
    printf("pointer size = %zu\n", sizeof(char *));

    str->write_fp(stdout);

    while (DSh_OK( io->get_byte(&c) )) {
        printf("byte -> '%c'\n", c);
    }

    str2 = new DSh_Str("foo2\n");
    if (str2->equal(str)) {
        printf("1. equal\n");
    }
    else { 
        printf("1. NOT equal\n");
    }

    str2 = new DSh_Str("foo\n");
    if (str2->equal(str)) {
        printf("2. equal\n");
    }
    else { 
        printf("2. NOT equal\n");
    }

    DSh_StrRef val_specs = "sd";
    DSh_StrRef val_flags = "l";
    DSh_StrRef fmt = "foo='%ld', bar='% s\n";

    /*
    const char *val_specs_c = "sd";
    const char *val_flags_c = "l";
    const char *fmt_c = "foo='%ld', bar='%s\n";
    */

    DSh_StrRef out_str = DSh_Str::format_cb(format_callback, val_specs, val_flags,
        fmt, 1, "stuff");

    return 0;
}
