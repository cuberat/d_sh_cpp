/* -*- mode: c++ -*- */
/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef D_SH_NIL_H
#define D_SH_NIL_H

class DSh_Nil: public DSh_Obj {
  public:
    DSh_Nil(): DSh_Obj() { }
    
    virtual int is_nil() { return 1; }

  private:
};

class DSh_NilRef: public DSh_RefBase<DSh_Nil> {
  public:
    DSh_NilRef(DSh_Nil *p = 0): DSh_RefBase<DSh_Nil>(p) { }
};

#endif
