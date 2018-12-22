#!/usr/bin/env bash

[[ $# < 1 ]] && echo "usage: $0 /path/to/pulseaudio-source" && exit 1

cd $(dirname "$0")
for a in pulse/* ; do
    file="$1"/src/"$a"
    [[ -e "$file" ]] && cp "$file" pulse
    [[ ! -e "$file" ]] && echo "missing $file"
done

exit 0
