#!/bin/sh
set -e

DESTDIR="${1:-/usr/local/bin}"
BIN="$(pwd)/utilbox"

if [ ! -x "$BIN" ]; then
    echo "install.sh: utilbox binary not found or not executable at $BIN" >&2
    echo "install.sh: run 'make' first" >&2
    exit 1
fi

mkdir -p "$DESTDIR"
cp "$BIN" "$DESTDIR/utilbox"

APPLETS=$(grep -oP '\{\s*"\K[a-z]+' include/applets.h)

for name in $APPLETS; do
    ln -sf utilbox "$DESTDIR/$name"
    echo "installed: $DESTDIR/$name -> utilbox"
done

echo "done."
