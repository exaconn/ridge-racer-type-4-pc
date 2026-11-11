# Application icon

`ridge-racer-type-4.svg` is an original icon made for this port: an abstract
night road sweeping towards a banded sunset over low ridges, with speed
streaks. It contains no artwork, logos, lettering or trade dress from the
game or its publisher, and was drawn by hand as SVG (no image generator and
no reference artwork).

`build.sh` renders the SVG into every derived file:

| File | Use |
| --- | --- |
| `packaging/icon/ridge-racer-type-4.png` | 1024x1024 source PNG (alpha outside the rounded tile) |
| `packaging/macos/RidgeRacerType4.icns` | macOS bundle icon, 16 to 512 pt at 1x and 2x (`iconutil`) |
| `packaging/windows/ridge-racer-type-4.ico` | Windows executable icon, 16 to 256 px (`ridge-racer-type-4.rc.in`) |

Run `packaging/icon/build.sh` after editing the SVG; it needs `rsvg-convert`
(librsvg), `iconutil` (macOS) and ImageMagick's `magick`.
