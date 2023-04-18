make -f Makefile.windows.mak all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)