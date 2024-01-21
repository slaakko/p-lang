@echo off
pushd code
call build.bat
popd
ref2html -v intro.ref
