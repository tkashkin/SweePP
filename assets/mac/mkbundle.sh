#!/bin/sh

bin="$1"

rm -rf "release/macos"
mkdir -p "release/macos"

cp -rf "assets/macos/Swee++.app" "release/mac/"

cp -f "$bin/swee++" "release/macos/Swee++.app/Contents/MacOS/"
cp -f "$bin/../libswee++/libswee++.0.1.dylib" "release/macos/Swee++.app/Contents/MacOS/"

assets/mac/utils/createdmg/create-dmg --volname "Swee++" \
    --volicon "assets/macos/Swee++.app/Contents/Resources/AppIcon.icns" \
    --background "assets/macos/DMGBackground.png" \
    --window-pos 200 120 \
    --window-size 800 400 \
    --icon-size 100 \
    --icon Swee++.app 200 190 \
    --hide-extension Swee++.app \
    --app-drop-link 600 185 \
    "release/macos/Swee++.dmg" \
    "release/macos/"