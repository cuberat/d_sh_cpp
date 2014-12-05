/* -*- mode: c++ -*- */
/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef D_SH_MAP_H
#define D_SH_MAP_H

#include <d_sh_cpp/d_sh_ref.h>

class DSh_Map: public DSh_Obj {
  public:
    DSh_Map(): DSh_Obj() { this->_hash = g_hash_table_new_full(_hash_func,
            _key_equal_func, _destroy_func, _destroy_func);}

    virtual int is_map() { return 1; }

  private:
    GHashTable *_hash;
    
    static gboolean _key_equal_func(gconstpointer a, gconstpointer b);
    static void _destroy_func(gpointer data);
    static guint _hash_func(gconstpointer key); // use g_str_hash()
};

class DSh_MapRef: public DSh_RefBase<DSh_Map> {
  public:
    DSh_MapRef(DSh_Map *p = 0): DSh_RefBase<DSh_Map>(p) { }
};


#endif
