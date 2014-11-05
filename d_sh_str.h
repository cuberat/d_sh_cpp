/* -*- mode: c++ -*- */
/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef D_SH_STR_H
#define D_SH_STR_H

#include <stdarg.h>
#include <stdio.h>

#include <d_sh_cpp/d_sh_ref.h>

class DSh_Str: public DSh_Obj {
  public:
    DSh_Str(): DSh_Obj() { this->_gstr = g_string_sized_new(0); }

    DSh_Str(size_t num_bytes): DSh_Obj() {
        gsize size = (gsize)num_bytes;
        this->_gstr = g_string_sized_new(size);
    }

    DSh_Str(const char *s, size_t num_bytes): DSh_Obj() {
        gsize size = (gsize)num_bytes;
        this->_gstr = g_string_new_len((const gchar *)s, size);
    }

    DSh_Str(const char *s): DSh_Obj() {
        this->_gstr = g_string_new((const gchar *)s);
    }

    ~DSh_Str() {
        if (this->_gstr) {
            g_string_free(this->_gstr, 1);
        }
    }

    size_t write_fp(FILE *fp) {
        return fwrite(_gstr->str, 1, (size_t)_gstr->len, fp);
    };

    void fmt_ap(const char *fmt, va_list ap);
    void fmt(const char *fmt, ...);
    DSh_Str *sprintf(const char *fmt, ...);

  private:
    GString *_gstr;
};

class DSh_StrRef: public DSh_RefBase<DSh_Str> {
  public:
    DSh_StrRef(DSh_Str *p = 0): DSh_RefBase<DSh_Str>(p) { }
};

#endif
