make -f Makefile.linux.mak buildir
make -f Makefile.linux.mak objdir
make -f Makefile.linux.mak savedir
make -f Makefile.linux.mak all
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "Error:"$ERRORLEVEL && exit
fi