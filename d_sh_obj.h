/* -*- mode: c++ -*- */
/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#include <d_sh_cpp/d_sh_ref.h>
#include <d_sh_cpp/d_sh_obj_base.h>

#ifndef D_SH_OBJ_H
#define D_SH_OBJ_H

#define DSh_OK(x) ((x) == 0)

class DSh_Obj: public DSh_ObjBase {
  public:
    DSh_Obj(): DSh_ObjBase() { }

    virtual int is_nil() { return 0; }

  private:
};

class DSh_Ref: public DSh_RefBase<DSh_Obj> {
  public:
    DSh_Ref(DSh_Obj *p = 0): DSh_RefBase<DSh_Obj>(p) { }
};

#include <d_sh_cpp/d_sh_nil.h>
#include <d_sh_cpp/d_sh_ref.h>
#include <d_sh_cpp/d_sh_str.h>
#include <d_sh_cpp/d_sh_io.h>
#include <d_sh_cpp/d_sh_map.h>

#endif
