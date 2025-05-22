@echo off
setlocal
:: Замените `demo` на любое другое удобное название
set main=demo
echo =======================================================
echo [1/2] Compiling main file with VM-C$ static library \*
gcc %main%.c -L. -l:VM-C$.a -o %main%.exe
echo [2/2] Project assembly completed successfully! \*
echo =======================================================
endlocal
pause