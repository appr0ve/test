#!/bin/bash
function prepare () {
  su --login \
    apt-get install -y \
    gcc meson glib2-devel libjson-glib-devel libgio-devel
}
function get () {
  wget 'https://savannah.gnu.org/cgi-bin/viewcvs/*checkout*/config/config/config.{guess,sub}'
}
autoreconf -i
