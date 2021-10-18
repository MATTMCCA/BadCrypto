@echo off
echo ******** File Hash Befor Encryption ***********
echo.
certutil -hashfile bench.dat MD5
echo ----------------------------------------------------------
echo.
echo ******** Time Elapsed ***********
powershell -ExecutionPolicy ByPass -File bench.ps1
echo ----------------------------------------------------------
echo.
echo ******** File Hash After Encryption ***********
echo. 
certutil -hashfile bench.dat MD5
echo ----------------------------------------------------------
pause