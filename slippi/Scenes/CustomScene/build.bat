SET "TKPATH=../../../MexTK/"
SET "FSPATH=./"
SET "FNAME=CustomScene.dat"

:: Copy GUI dat file
copy ".\CustomSceneGUI.dat" ".\%FNAME%"

:: compile main code and append to copied GUI
"%TKPATH%MexTK.exe" -ff -i "major.c" -s mjFunction -t "%TKPATH%mjFunction.txt" -l "%TKPATH%melee.link" -dat "%FSPATH%%FNAME%"
"%TKPATH%MexTK.exe" -ff -i "minor.c" -s mnFunction -t "%TKPATH%mnFunction.txt" -l "%TKPATH%melee.link" -dat "%FSPATH%%FNAME%"
"%TKPATH%MexTK.exe" -trim "%FSPATH%%FNAME%"
