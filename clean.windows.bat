make -f Makefile.windows.mak clean
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)