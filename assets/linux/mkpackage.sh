#!/bin/sh

bin="$1"

rm -rf "release/linux"
mkdir -p "release/linux"

cp -rf "assets/linux/debpackage" "release/linux/swee++-0.1"

cp -f "$bin/swee++" "release/linux/swee++-0.1/usr/bin/"
cp -f "$bin/../libswee++/libswee++.so.0.1" "release/linux/swee++-0.1/usr/lib/"
ln -s "libswee++.so.0.1" "release/linux/swee++-0.1/usr/lib/libswee++.so"

strip -s "release/linux/swee++-0.1/usr/bin/swee++"
strip -s "release/linux/swee++-0.1/usr/lib/libswee++.so.0.1"

rm -rf "/tmp/dpkg-deb-swee++"
mkdir -p "/tmp/dpkg-deb-swee++"
cp -rf "release/linux/swee++-0.1" "/tmp/dpkg-deb-swee++"
chmod -R 755 "/tmp/dpkg-deb-swee++"

dpkg-deb --build "/tmp/dpkg-deb-swee++/swee++-0.1"
cp -f "/tmp/dpkg-deb-swee++/swee++-0.1.deb" "release/linux"

rm -rf "/tmp/dpkg-deb-swee++"