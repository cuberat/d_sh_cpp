/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#include <d_sh_cpp/d_sh_obj.h>

DSh_StrRef
DSh_Str::format_cb(format_cb_t cb, DSh_StrRef val_specs, DSh_StrRef val_flags,
    DSh_StrRef fmt, va_list ap) {
    DSh_StrRef str = new DSh_Str("");
    d_sh_uchar_t ch = 0;
    d_sh_uchar_t fmt_start = (d_sh_uchar_t)'%';
    int in_fmt = 0;
    DSh_IORef f = new DSh_IO(fmt);
    DSh_StrRef flags_found;
    DSh_StrRef padding_found;
    DSh_StrRef this_chunk;
    
    while (DSh_OK(f->get_char(&ch))) {
        if (in_fmt) {
            if (ch == fmt_start) {
                /* escaped format start, so output literal */
                str->append(fmt_start);
                in_fmt = 0;
                continue;
            }

            if (val_specs->has_char(ch)) {
                this_chunk = cb(ch, padding_found, flags_found, ap);
                str->append(this_chunk);
                in_fmt = 0;
                continue;
            }

            if (val_flags->has_char(ch)) {
                flags_found->append(ch);
                continue;
            }
            
            padding_found->append(ch);
        }
        else if (ch == fmt_start) {
            in_fmt = 1;
            flags_found = "";
            padding_found = "";
            continue;
        }
        
        str->append(ch);        
    }
    

    return str;
}

DSh_StrRef
DSh_Str::format_cb(format_cb_t cb, DSh_StrRef val_specs, DSh_StrRef val_flags,
    DSh_StrRef fmt, ...) {
    DSh_StrRef rv;
    va_list ap;

    va_start(ap, fmt);
    rv = format_cb(cb, val_specs, val_flags, fmt, ap);
    va_end(ap);

    return rv;
}

DSh_StrRef
DSh_Str::format_cb(format_cb_t cb, const char *val_specs, const char *val_flags,
    DSh_StrRef fmt, va_list ap) {
    DSh_StrRef val_specs_ref = new DSh_Str(val_specs);
    DSh_StrRef val_flags_ref = new DSh_Str(val_flags);

    return format_cb(cb, val_specs_ref, val_flags_ref, fmt, ap);
}

DSh_StrRef
DSh_Str::format_cb(format_cb_t cb, const char *val_specs, const char *val_flags,
    DSh_StrRef fmt, ...) {
    va_list ap;
    DSh_StrRef rv;
    
    va_start(ap, fmt);
    rv = format_cb(cb, val_specs, val_flags, fmt, ap);
    va_end(ap);

    return rv;
}

DSh_StrRef
DSh_Str::format_cb(format_cb_t cb, const char *val_specs, const char *val_flags,
    const char *fmt, va_list ap) {
    DSh_StrRef str_fmt = new DSh_Str(fmt);

    return format_cb(cb, val_specs, val_flags, str_fmt, ap);
}

DSh_StrRef
DSh_Str::format_cb(format_cb_t cb, const char *val_specs, const char *val_flags,
    const char *fmt, ...) {
    va_list ap;
    DSh_StrRef rv;
    
    va_start(ap, fmt);
    rv = format_cb(cb, val_specs, val_flags, fmt, ap);
    va_end(ap);

    return rv;
}



/*
DSh_StrRef&
DSh_StrRef::operator=(const char *p) {
        DSh_Str *str = new DSh_Str(p);
        this->_acquire(str);
        return *this;
}
*/

DSh_StrRef::DSh_StrRef(const char *s) {
    DSh_RefBase<DSh_Str>(0);
    DSh_Str *str = new DSh_Str(s);
    this->_acquire(str);
}


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
