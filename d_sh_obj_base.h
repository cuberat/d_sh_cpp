/* -*- mode: c++ -*- */
/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef D_SH_OBJ_BASE_H
#define D_SH_OBJ_BASE_H

#include <stdio.h>
#include <glib.h>

#ifdef G_ATOMIC_LOCK_FREE
/* https://developer.gnome.org/glib/2.38/glib-Atomic-Operations.html#g-atomic-int-dec-and-test */
#endif

#include <sys/types.h>
#include <inttypes.h>

typedef uint32_t d_sh_uchar_t;
typedef off_t d_sh_off_t;
/* typedef gsize d_sh_size_t; */
typedef int d_sh_bool_t;

class DSh_ObjBase {
  public:
    DSh_ObjBase() { this->_refcnt = 0; }
    virtual ~DSh_ObjBase() { }

    inline void ref_incr() { g_atomic_int_inc(&this->_refcnt); }

    inline int ref_decr_and_test() {
        return (int)g_atomic_int_dec_and_test(&this->_refcnt);
    }

    inline int get_refcnt() {
        return (int)this->_refcnt;
    }

  private:
    gint _refcnt;
    
};

#endif
