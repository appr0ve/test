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
apt-get install -y gettext-tools gtk-doc
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
meson setup $PREFER_BUILD_DIR
```

Compile program (within directory)[^2]:
```
meson compile
```

## Installation

You can build RPM package using gear and hasher.
```
apt-get install -y gear hasher
```

From root (admin) add regular user to group
```
# hasher-useradd $USERNAME
```

Initiate build envoriment
```
gear-hsh
```

In future you can rebuild from existense env:
```
gear --hasher -- hsh-rebuild
```

## Usage

RDB_API_URL setup url for API (default [^1])

See more on
```
rdb -help
```

## Localization

Refresh template:
```
meson compile rdb-pot
```

Generate locale file [^3]:
```
meson compile rdb-update-po
```

Assign locale directory:
```
meson configure $PREFER_BUILD_DIR --localedir=$PREFER_BUILD_DIR/po
```

[^1]: https://rdb.altlinux.org/api/
[^2]: You can specify that by `-C` option.
[^3]: Languages defined in `po/LINGUAS`
