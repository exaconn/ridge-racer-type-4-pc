# Application icon

`ridge-racer-type-4.png` (1024x1024, transparent outside the badge) is an
original icon made for this port with an image generator: a chrome-rimmed
yellow badge with a winged "R4" monogram, the title in plain black lettering
and a fictional late-90s coupe. It does not reproduce the publisher's logos,
fonts, characters or box art.

`build.sh` renders it into the derived files:

| File | Use |
| --- | --- |
| `packaging/macos/RidgeRacerType4.icns` | macOS bundle icon, 16 to 512 pt at 1x and 2x (`iconutil`) |
| `packaging/windows/ridge-racer-type-4.ico` | Windows executable icon, 16 to 256 px (`ridge-racer-type-4.rc.in`) |

Run `packaging/icon/build.sh` after replacing the PNG; it needs `sips` and
`iconutil` (macOS) and ImageMagick's `magick`.
