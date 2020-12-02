#!/bin/bash

THIS="${0##*/}"
THIS_PATH="${0%/*}"
THIS_VERSION="0.1"

YEAR=$(date +%Y)
DAY=$(date +%-d)

show_help() {
    cat <<EOT
Usage: $THIS [-h] [-d|--day num] [-y|--year num]

Gets the AoC input for the specified day/year.

-d, --day    Specifies the day. Defaults to now.
-y, --year   Specifies the year. Defaults to now.
EOT
}

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)
            show_help
            exit 0
            ;;
        -d|--day)
            shift
            DAY="$1"
            shift
            ;;
        -y|--year)
            shift
            YEAR="$1"
            shift
            ;;
    esac
done

PADDAY=$(printf "%02d" $DAY)

# Get this curl command by open Chrome developer tools on the input page, reload
# to capture the network request, right click on the request,
# Copy -> Copy As cURL
# Thanks to https://askubuntu.com/a/1099876/562735

curl "https://adventofcode.com/${YEAR}/day/${DAY}/input"  \
    -o "${PADDAY}a-input"  \
    --cookie aoc-cookies  \
    --compressed
