@echo off

rem compress and encode all the files in the data folder

cd src
del website\*.gz.h
cd websrc

for %%f in (*.*) do (
    ..\..\gzip -k %%f
    ..\..\luajit ..\..\bin2h.lua %%f.gz ..\website\%%f.gz.h
    rem c:\"program Files"\git\usr\bin\gzip -k %%f
    rem c:\"program Files"\git\usr\bin\xxd -i -c 20 %%f.gz ..\src\website\%%~nxf.gz.h
)
del *.gz
cd ..
cd ..
