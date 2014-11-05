/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#include <d_sh_cpp/d_sh_obj.h>

void
DSh_Str::fmt_ap(const char *fmt, va_list ap) {
    g_string_vprintf(this->_gstr, (const gchar *)fmt, ap);
}

void
DSh_Str::fmt(const char *fmt, ...) {
    va_list ap;
    
    va_start(ap, fmt);
    this->fmt_ap(fmt, ap);
    va_end(ap);
}

DSh_Str *
DSh_Str::sprintf(const char *fmt, ...) {
    va_list ap;
    DSh_Str *str = new DSh_Str(1);

    va_start(ap, fmt);
    str->fmt_ap(fmt, ap);
    va_end(ap);

    return str;
}
