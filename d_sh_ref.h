/*
  Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.

  This software is licensed under the Apache license, version 2.0,
  available at:

  http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef D_SH_REF_H
#define D_SH_REF_H

#include <stdio.h>

#define D_SH_REF_DEBUG 1

#include <d_sh_cpp/d_sh_obj.h>

template <class T> class DSh_RefBase {
  public:
    explicit DSh_RefBase(T *p = 0): _ptr(0) { this->_acquire(p); }
    // DSh_Ref(T *p = 0): _ptr(p) { }

    DSh_RefBase(const DSh_RefBase& r) throw(): _ptr(0) {
        this->_acquire(r._ptr);
    }

    virtual ~DSh_RefBase() { this->_release(); }

    DSh_RefBase& operator=(T *p) {
        if (this->_ptr != p) {
            this->_acquire(p);
        }
        return *this;
    }

    DSh_RefBase& operator=(const DSh_RefBase& r) {
        if (this != &r) {
            this->_acquire(r._ptr);
        }
        return *this;
    }

    T& operator*() const throw() { return this->_ptr; }
    T* operator->() const {
        if (this->_ptr == 0) {
            throw "method called on non reference";
        }
        return this->_ptr;
    }

    inline bool operator==(const T& rhs) const {
        return this->get_ptr() == rhs.get_ptr();
    }

    T* get_ptr() const throw() { return this->_ptr; }
    int has_ptr() const throw() { return this->_ptr ? 1 : 0; }
    // bool unique() const throw() { return this->_ptr ? this->_ptr->ref_cnt() == 1 : true; }

    virtual operator T*() { return this->_ptr; }
    // virtual operator Pl7Obj*() { return (Pl7Obj *)this->_ptr; }

    void clear() { this->_release(); }

  protected:
    T *_ptr;

    void _release() {
        if (this->_ptr) {
            this->_release(this->_ptr);
        /*
            unsigned int cnt = this->_ptr->ref_decr();
#if D_SH_REF_DEBUG
            fprintf(stderr, "releasing %p, ref_cnt=%u\n", this->_ptr, this->_ptr->get_refcnt());
#endif

            if (cnt == 0) {
                delete this->_ptr;
            }
        */
            this->_ptr = 0;
        }
    }

    virtual void _release(T *p) {
        if (p) {
            unsigned int cnt_is_zero = p->ref_decr_and_test();
#if D_SH_REF_DEBUG
            fprintf(stderr, "releasing %p, ref_cnt=%u\n", p, p->get_refcnt());
#endif

            if (cnt_is_zero) {
                delete p;
            }
        }
    }

    virtual void _acquire(T *p) {
        T* orig_ptr = this->_ptr;

        if (p) {
            p->ref_incr();
#if D_SH_REF_DEBUG
            fprintf(stderr, "acquiring %p, ref_cnt=%u\n", p, p->get_refcnt());
#endif
        }

        this->_release(orig_ptr);

        this->_ptr = p;
    }
    
};

class DSh_Ref: public DSh_RefBase<DSh_Obj> {
  public:
  DSh_Ref(DSh_Obj *p = 0): DSh_RefBase<DSh_Obj>() { this->_acquire(p); }

    // operator DSh_Obj*() { return (DSh_Obj *)this->_ptr; }

    DSh_Ref& operator=(DSh_Obj *p) {
        if ((DSh_Obj *)this != p) {
            this->_release();
            this->_acquire(p);
        }
        return *this;
    }

};

#endif
