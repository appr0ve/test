#!/bin/bash

BUILD_SYSTEM="$1"

function build() {
  if [ "$BUILD_SYSTEM" == 'gnu' ]; then
    echo 'gnu'
  elif [ "$BUILD_SYSTEM" == 'meson' ]; then
    gear --hasher -- hsh-rebuild $BS
  else echo 'Wrong build system!'
  fi
}

if !(grep -q hasher /etc/passwd); then
  su --login -c \
    'apt-get install -y gear hasher' \
    'hasher-useradd $USER'
fi

if !(test -d $HOME/hasher); then
  gear-hsh
else
  build
fi
