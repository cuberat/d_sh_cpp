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

    obj = new DSh_Obj();
    str = new DSh_Str("foo\n");

    str->write_fp(stdout);

    return 0;
}
