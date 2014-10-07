#
# Copyright (c) 2014 Don Owens <don@regexguy.com>.  All rights reserved.
#
# This software is licensed under the Apache license, version 2.0,
# available at:
#
# http://www.apache.org/licenses/LICENSE-2.0


all: shlib


D_SH_LIB = libd_sh.so
D_SH_SONAME = libd_sh.so.0.1

GPP = g++

SRC_FILES = d_sh_io.cpp d_sh_obj_base.cpp d_sh_ref.cpp d_sh_str.cpp

HEADER_FILES = d_sh_io.h d_sh_obj_base.h d_sh_obj.h d_sh_ref.h d_sh_str.h

GLIB_INCLUDES = `pkg-config --cflags glib-2.0`
GLIB_LIBS = `pkg-config --libs glib-2.0`
INCLUDE = -I.. -I../include $(GLIB_INCLUDES)
CFLAGS = $(INCLUDE) -fPIC -Wall $(DEBUG)
LDFLAGS = $(GLIB_LIBS)

OBJS = $(SRC_FILES:%.cpp=%.o)
OBJS_BASE = $(SRC_FILES:%.cpp=%)

EXECS = test_prog

test: test_prog
	./test_prog

test_prog: test_prog.cpp $(OBJS)
	$(GPP) -o $@ $(CFLAGS) $< $(LDFLAGS)

echoobjs:
	@echo $(OBJS)

echoobjsbase:
	@echo $(OBJS_BASE)

define CPP_template =
$(1).o: $(1).cpp
	$$(GPP) -c -o $$@ $$(CFLAGS) $$<
endef

$(foreach obj,$(OBJS_BASE),$(eval $(call CPP_template,$(obj))))

$(D_SH_LIB): $(D_SH_SONAME)
	ln -sf $(D_SH_SONAME) $(D_SH_LIB)

$(D_SH_SONAME): $(OBJS)
	$(GPP) -shared -Wl,-soname,$(D_SH_SONAME) -o $(D_SH_SONAME) $(OBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS)
