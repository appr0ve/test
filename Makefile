# User defined
EXE=rdb
LIB=librdb
LIB_H=include/rdb
VER=1

# Trash
CLN=$(EXE) $(LIB).so

CC=gcc
H=`pkg-config --cflags glib-2.0 json-glib-1.0`
S=-I$(PWD)/include
CFLAGS=-Wall -g -O3 -std=gnu17 $(H) $(S)
LDLIBS=`pkg-config --libs glib-2.0 json-glib-1.0`

all: $(EXE)

# Targets
$(EXE): LDLIBS+=-L$(PWD) -l$(EXE)
$(EXE): $(O) $(LIB).so
$(LIB).so: CFLAGS+=-shared -Wl,-soname,$(LIB).so -fPIC
$(LIB).so: $(LIB).c
	$(CC) $(LIB).c $(CFLAGS) -o $@ $(LDLIBS)

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
po_init:
	mkdir -p po/$(lang)
	msginit --input $(EXE).pot --output po/$(lang)/rdb.po --locale $(lang)
po_update:
	msgmerge --update po/$(lang)/$(EXE).po $(EXE).pot
po_template: $(EXE).c
	xgettext --keyword=_ --language=c --sort-output -o $(EXE).pot $(EXE).c
po_mo: LC_MSG_DIR=po/$(lang)/LC_MESSAGES
po_mo:
	mkdir -p $(LC_MSG_DIR)
	msgfmt --output $(LC_MSG_DIR)/$(EXE).mo po/$(lang)/$(EXE).po
