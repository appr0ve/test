# ALTRepo API client and shared library

Developed for demonstrative purpose.

rdb client interact with ALTRepo API[^1].

## Prerequisites

Before start building install followed dependencies:
```
apt-get install -y gcc meson glib2-devel libjson-glib-devel libgio-devel
```

Additional dependencies:
```
apt-get install -y gettext-tools
```

After that get source code for program from github repo:
```
git clone https://github.com/appr0ve/test
```
or (you can change wget on your prefer choice)
```
wget https://github.com/appr0ve/test/archive/refs/heads/main.tar.gz
tar xf main.tar.gz
```

## Building

First create build directory by invoke this command:
```
meson setup [builddir]
```

Compile program (within directory)[^2]:
```
meson compile
```

## Installation


## Usage

LC_MESSAGES setup language for program

RDB_API_URL setup url for API (default [^1])

## Localization

Refresh template:
```
meson compile rdb-pot
```

Generate locale file [^3]:
```
meson compile rdb-update-po
```

[^1]: https://rdb.altlinux.org/api/
[^2]: You can specify that by `-C` option.
[^2]: Languages defined in `po/LINGUAS`
