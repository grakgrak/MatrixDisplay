@echo off

rem compress and encode all the files in the data folder

del src\website\*.gz.h

cd data
for %%f in (*.*) do (
    ..\gzip -k %%f
    ..\luajit ..\bin2h.lua %%f.gz ..\src\website\%%f.gz.h
    rem c:\"program Files"\git\usr\bin\gzip -k %%f
    rem c:\"program Files"\git\usr\bin\xxd -i -c 20 %%f.gz ..\src\website\%%~nxf.gz.h
)
cd ..

del data\*.gz
