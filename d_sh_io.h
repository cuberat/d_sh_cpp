/* -*- mode: c++ -*- */
/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef D_SH_IO_H
#define D_SH_IO_H

#include <d_sh_cpp/d_sh_str.h>

class DSh_IOFileHandle: public DSh_Obj {
  public:
    ~DSh_IOFileHandle() { }

    virtual int get_byte(unsigned char *b) { return 2; }
    virtual int put_byte(unsigned char *b) { return 2; }
    virtual int get_buf(unsigned char *buf, size_t size) { return 2; }
    virtual int put_buf(unsigned char *buf, size_t size) { return 2; }
    virtual int flush_stream() { return 2; }

    DSh_IOFileHandle() { }

  protected:

};

class DSh_IOStringStream: public DSh_IOFileHandle {
  public:
    DSh_IOStringStream(DSh_StrRef str): DSh_IOFileHandle(),
                                        _pos(0), _readonly(1) {
        this->_str = str;
    }

    ~DSh_IOStringStream() { }

    virtual int get_byte(unsigned char *b) {
        int rv = this->_str->get_byte_at(b, this->_pos);
        if (rv) {
            return 1;

        }

        this->_pos++;
        return 0;
    }

    /*
    virtual int put_byte(unsigned char *b);
    virtual int get_buf(unsigned char *buf, size_t size);
    virtual int put_buf(unsigned char *buf, size_t size);
    */
    virtual int flush_stream() { return 0; }

  private:
    DSh_StrRef _str;
    size_t _pos;
    int _readonly;
};

class DSh_IOFileStream: public DSh_IOFileHandle {
  public:

};

class DSh_IOSocketStream: public DSh_IOFileHandle {
  public:

};

class DSh_IO: public DSh_Obj {
  public:
    DSh_IO(DSh_StrRef str) {
        DSh_IOStringStream *stream = new DSh_IOStringStream(str);
        this->_handle = stream;
    }

    static DSh_IO *new_string_writer() {
        DSh_StrRef str = new DSh_Str("");
        DSh_IO *writer = new DSh_IO(str);
        return writer;
    }

    virtual int get_byte(unsigned char *b) {
        return this->_handle->get_byte(b);
    }

    // FIXME: implement utf-8
    virtual int get_char(d_sh_uchar_t *c) {
        unsigned char b = 0;
        int rv = this->_handle->get_byte(&b);

        if (DSh_OK(rv)) {
            *c = (d_sh_uchar_t)b;
        }
        
        return rv;
    }    


  private:
    DSh_IO(): DSh_Obj(), _handle(0) { };

    DSh_IOFileHandle *_handle;
    // string
    // FILE *
    // bytes
    // utf-8
};

class DSh_IORef: public DSh_RefBase<DSh_IO> {
  public:
    DSh_IORef(DSh_IO *p = 0): DSh_RefBase<DSh_IO>(p) { }
};



#endif
