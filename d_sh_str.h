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
#include <string.h>

#include <d_sh_cpp/d_sh_ref.h>

class DSh_Str;

class DSh_StrRef: public DSh_RefBase<DSh_Str> {
  public:
    DSh_StrRef(DSh_Str *p = 0): DSh_RefBase<DSh_Str>(p) { }
    DSh_StrRef(const char *s);

    /* DSh_StrRef& operator=(const char *p); */
};

typedef DSh_StrRef (*format_cb_t)(d_sh_uchar_t f, DSh_StrRef pad, DSh_StrRef flags, va_list ap);

class DSh_Str: public DSh_Obj {
  public:
    DSh_Str(): DSh_Obj(), _is_utf8(0) { this->_gstr = g_string_sized_new(0); }

    DSh_Str(size_t num_bytes): DSh_Obj() {
        this->_init();
        gsize size = (gsize)num_bytes;
        this->_gstr = g_string_sized_new(size);
    }

    DSh_Str(const char *s, size_t num_bytes): DSh_Obj() {
        this->_init();
        gsize size = (gsize)num_bytes;
        this->_gstr = g_string_new_len((const gchar *)s, size);
    }

    DSh_Str(const char *s): DSh_Obj() {
        this->_init();
        this->_gstr = g_string_new((const gchar *)s);
    }

    ~DSh_Str() {
        if (this->_gstr) {
            g_string_free(this->_gstr, 1);
        }
    }

    virtual int is_str() { return 1; }

    int equal(DSh_StrRef str) {
        if (this->_gstr->len != str->_gstr->len) {
            return 0;
        }

        if (memcmp(this->_gstr->str, str->_gstr->str, this->_gstr->len) == 0) {
            return 1;
        }

        return 0;
    }

    size_t write_fp(FILE *fp) {
        // fprintf(stdout, "len=%zu\n", (size_t)this->_gstr->len); fflush(stdout);
        return fwrite((void *)this->_gstr->str, 1, (size_t)this->_gstr->len,
            fp);
    };

    void fmt_c_ap(const char *fmt, va_list ap);
    void fmt_c(const char *fmt, ...);
    DSh_Str *sprintf(const char *fmt, ...);
    
    int append(d_sh_uchar_t ch) {
        if (this->_is_utf8) {
            g_string_append_unichar(this->_gstr, (gunichar)ch);
        }
        else {
            g_string_append_c(this->_gstr, (gchar)ch);
        }

        return 0;
    }

    int append(char ch) {
        g_string_append_c(this->_gstr, (gchar)ch);

        return 0;
    }

    int append(DSh_StrRef str) {
        g_string_append_len(this->_gstr, str->_gstr->str,
            str->_gstr->len);
        return 0;
    }

    int append_printf_c(const char *fmt, ...) {
        va_list ap;

        va_start(ap, fmt);
        g_string_append_vprintf(this->_gstr, (const gchar *)fmt, ap);
        va_end(ap);

        return 0;
    }

    /*******************/
    /* read-only stuff */
    /*******************/

    int get_byte_at(unsigned char *c, size_t pos) {
        if (pos >= this->_gstr->len) {
            return 1;
        }

        *c = (unsigned char)this->_gstr->str[pos];

        return 0;
    }

    int next_char(d_sh_uchar_t *ch, size_t *offset) {
        if (*offset >= this->_gstr->len) {
            return 1;
        }

        if (this->_is_utf8) {
            if (! this->_utf8_validated) {
                if (g_utf8_validate(this->_gstr->str, this->_gstr->len,
                        (const gchar **)0)) {
                    this->_utf8_validated = 1;
                }
                return -3;
            }
            gunichar c;
            gchar *next;
            c = g_utf8_get_char(&this->_gstr->str[*offset]);
            next = g_utf8_next_char(&this->_gstr->str[*offset]);
            *offset = next - this->_gstr->str;
            *ch = c;
            
            return 0;
        }
        else {
            *ch = (d_sh_uchar_t)this->_gstr->str[*offset];
            (*offset)++;
        }

        return 0;
    }

    int has_char(d_sh_uchar_t ch) {
        if (this->_is_utf8) {
            if (g_utf8_strchr(this->_gstr->str, this->_gstr->len,
                    (gunichar)ch)) {
                return 1;
            }
            else {
                return 0;
            }
        }
        else {
            if (memchr(this->_gstr->str, (int)ch, (size_t)this->_gstr->len)) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }


    static DSh_StrRef format_cb(format_cb_t cb, DSh_StrRef val_specs,
        DSh_StrRef val_flags, va_list ap, DSh_StrRef fmt);
    static DSh_StrRef format_cb(format_cb_t cb, DSh_StrRef val_specs,
        DSh_StrRef val_flags, DSh_StrRef fmt, ...);
    static DSh_StrRef format_cb(format_cb_t cb, const char *val_specs,
        const char *val_flags, DSh_StrRef fmt, ...);
    static DSh_StrRef format_cb(format_cb_t cb, const char *val_specs,
        const char *val_flags, DSh_StrRef fmt, va_list ap);
    static DSh_StrRef format_cb(format_cb_t cb, const char *val_specs,
        const char *val_flags, const char *fmt, ...);
    static DSh_StrRef format_cb(format_cb_t cb, const char *val_specs,
        const char *val_flags, const char *fmt, va_list ap);


  private:
    GString *_gstr;
    char _is_utf8;
    char _utf8_validated;

    void _init() {
        this->_is_utf8 = 0;
        this->_utf8_validated = 0;
    }
};


#endif
