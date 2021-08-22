SET "TKPATH=../../../MexTK/"
SET "FSPATH=./"
:: compile main code
"%TKPATH%MexTK.exe" -ff -i "majors.c" -s mjFunction -t "%TKPATH%mjFunction.txt" -l "%TKPATH%melee.link" -o "%FSPATH%SlippiScenes.dat" -ow
"%TKPATH%MexTK.exe" -ff -i "minors.c" -s mnFunction -t "%TKPATH%mnFunction.txt" -l "%TKPATH%melee.link" -dat "%FSPATH%SlippiScenes.dat"
"%TKPATH%MexTK.exe" -trim "%FSPATH%SlippiScenes.dat"

@REM "%TKPATH%MexTK.exe" -dat -i "majors.c" -s mjFunction -t "%TKPATH%mjFunction.txt" -l "%TKPATH%melee.link" -o "%FSPATH%SlippiScenes.dat" -ow

@REM MexTK.exe -ff -i "majors.c" -s mjFunction -t "%TKPATH%mjFunction.txt" -l "%TKPATH%melee.link" -o "%FSPATH%SlippiScenes.dat" -ow
