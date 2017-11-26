@echo off

chcp 437

set bin=%1

xcopy "%bin%\..\libswee++\libswee++.dll" "%bin%" /i /h /y /c /r /s

if exist "release\windows" del /f /s /q "release\windows" 1>nul
if exist "release\windows" rmdir /s /q "release\windows"

mkdir "release\windows"

xcopy "assets\win\swee++" "release\windows\swee++\" /i /h /y /c /r /s

xcopy "%bin%\swee++.exe" "release\windows\swee++\" /i /h /y /c /r /s
xcopy "%bin%\..\libswee++\libswee++.dll" "release\windows\swee++\" /i /h /y /c /r /s

echo Generating locales...
for /d %%i in (locales\*) do (
    echo     %%i
    mkdir "release\windows\swee++\%%i"
    "assets\win\xgettext\bin\msgfmt" -o "release\windows\swee++\%%i\swee++.mo" "%%i\swee++.po"
)

"assets\win\7za.exe" a -tzip "release\windows\swee++.zip" ".\release\windows\swee++\*"