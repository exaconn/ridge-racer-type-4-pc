#!/bin/sh
# Regenerate the application icons from ridge-racer-type-4.svg:
#   packaging/icon/ridge-racer-type-4.png        1024x1024 source PNG
#   packaging/macos/RidgeRacerType4.icns         macOS bundle icon (iconutil)
#   packaging/windows/ridge-racer-type-4.ico     Windows executable icon
# Requires rsvg-convert (librsvg), iconutil (macOS) and ImageMagick (magick).
set -eu
cd "$(dirname "$0")/../.."
svg=packaging/icon/ridge-racer-type-4.svg
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

rsvg-convert -w 1024 -h 1024 "$svg" -o packaging/icon/ridge-racer-type-4.png

iconset="$tmp/RidgeRacerType4.iconset"
mkdir "$iconset"
for size in 16 32 128 256 512; do
    rsvg-convert -w $size -h $size "$svg" -o "$iconset/icon_${size}x${size}.png"
    rsvg-convert -w $((size * 2)) -h $((size * 2)) "$svg" -o "$iconset/icon_${size}x${size}@2x.png"
done
iconutil -c icns "$iconset" -o packaging/macos/RidgeRacerType4.icns

for size in 16 24 32 48 64 128 256; do
    rsvg-convert -w $size -h $size "$svg" -o "$tmp/ico_$size.png"
done
magick "$tmp"/ico_16.png "$tmp"/ico_24.png "$tmp"/ico_32.png "$tmp"/ico_48.png \
    "$tmp"/ico_64.png "$tmp"/ico_128.png "$tmp"/ico_256.png \
    packaging/windows/ridge-racer-type-4.ico
