#!/bin/sh

bin="$1"

rm -rf "release/macos"
mkdir -p "release/macos"

cp -rf "assets/mac/Swee++.app" "release/macos/"

mkdir -p "release/macos/Swee++.app/Contents/MacOS"

cp -f "$bin/swee++" "release/macos/Swee++.app/Contents/MacOS/"
cp -f "$bin/../libswee++/libswee++.0.1.dylib" "release/macos/Swee++.app/Contents/MacOS/"

echo Generating locales...
mkdir -p "release/macos/Swee++.app/Contents/Resources/en.lproj"
for dir in locales/*/
do
    dir="${dir%/}"
    dir="${dir##*/}"
    echo $dir
    mkdir -p "release/macos/Swee++.app/Contents/Resources/$dir.lproj"
    msgfmt -o "release/macos/Swee++.app/Contents/Resources/$dir.lproj/swee++.mo" "locales/$dir/swee++.po"
done

assets/mac/utils/createdmg/create-dmg --volname "Swee++" \
    --volicon "release/macos/Swee++.app/Contents/Resources/AppIcon.icns" \
    --background "assets/mac/DMGBackground.png" \
    --window-pos 200 120 \
    --window-size 800 400 \
    --icon-size 100 \
    --icon Swee++.app 200 190 \
    --hide-extension Swee++.app \
    --app-drop-link 600 185 \
    "release/macos/Swee++.dmg" \
    "release/macos/"