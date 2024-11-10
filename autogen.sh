#!/bin/bash

if !(rpm -qa | grep automake); then
 su --login \
   apt-get install -y \
   gcc automake autoconf gettext \
   glib2-devel libjson-glib-devel libgio-devel
fi

autoreconf -i
