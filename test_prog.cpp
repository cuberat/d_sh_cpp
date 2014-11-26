/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#include <d_sh_cpp/d_sh_obj.h>

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

    return 0;
}
