SET "TKPATH=../../MexTK/"
SET "FSPATH=C:\Users\rober\Documents\Dev\Github\Ishiiruka\Binary\x64\Sys\GameFiles\GALE01\"
:: compile main code
"%TKPATH%MexTK.exe" -ff -i "main.c" -s slpFunction -t "%TKPATH%slpFunction.txt" -l "%TKPATH%melee.link" -o "%FSPATH%Slippi.dat" -v -ow

pause