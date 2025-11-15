#!/bin/sh
# Run xgettext to extract strings from all source files.
$XGETTEXT `find . -name \*.cpp -o -name \*.h -o -name \*.qml` -o $podir/kweathercore6.pot
