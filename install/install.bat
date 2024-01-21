@echo off
IF NOT EXIST ..\bin mkdir ..\bin
xcopy /Y ..\x64\release\pc.exe ..\bin
xcopy /Y ..\x64\release\pd.exe ..\bin
xcopy /Y ..\x64\release\rp.exe ..\bin
xcopy /Y ..\x64\release\p2html.exe ..\bin
xcopy /Y ..\x64\release\ref2html.exe ..\bin
xcopy /Y ..\x64\debug\pcd.exe ..\bin
xcopy /Y ..\x64\debug\pdd.exe ..\bin
xcopy /Y ..\x64\debug\rpd.exe ..\bin
xcopy /Y ..\x64\debug\p2htmld.exe ..\bin
xcopy /Y ..\x64\debug\ref2htmld.exe ..\bin
