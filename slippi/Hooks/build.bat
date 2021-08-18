SET "TKPATH=../../MexTK/"
SET "FSPATH=./"
:: compile main code
"%TKPATH%MexTK.exe" -ff -i "main.c" -s slpFunction -t "%TKPATH%slpFunction.txt" -l "%TKPATH%melee.link" -o "%FSPATH%Slippi.dat" -v -ow
