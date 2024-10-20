# User defined
EXE=rdb
LIB=librdb.so
LIB_H=include/rdb
VER=1

# Trash
CLN=$(EXE) $(LIB).$(VER)

# Only Simply Linux have broken symlink for `cc`?
# x86_64-alt-linux-gcc: No such file or directory 
CC=gcc-10
H=`pkg-config --cflags glib-2.0`
CFLAGS=-Wall -g -O3 -std=gnu17 $(H)
LDLIBS=`pkg-config --libs glib-2.0`

all: $(LIB) $(EXE)

# Targets
$(EXE): $(O)
$(LIB): CFLAGS+=-shared -Wl,-soname,$(LIB).$(VER) -fPIC
$(LIB): $(LIB).$(VER)
$(LIB).$(VER): $(O)
	$(CC) $(CFLAGS) -o $@ $(LDLIBS)

ifndef PREFIX
PREFIX=/opt/$(EXE)
endif

install:
	install -d $(PREFIX)/lib/
	install -m 644 $(LIB).$(VER) $(PREFIX)/lib/
	install -d $(PREFIX)/include/
	install -m 644 $(LIB_H) $(PREFIX)/include/
clean:
	rm -f $(CLN)
