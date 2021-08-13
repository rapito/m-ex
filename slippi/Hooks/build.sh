export TKPATH=../../MexTK/
export FSPATH=./
mono "$TKPATH/MexTK.exe" -ff -i "main.c" -s slpFunction -t "$TKPATH/slpFunction.txt" -l "$TKPATH/melee.link" -o "$FSPATH/Slippi.dat" -v -ow