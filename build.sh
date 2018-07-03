gcc main.c -Wl,-rpath,'$ORIGIN'/lib -o ./roguelike ./lib/libBearLibTerminal.so ./lib/libwren.so -lm &&
# LD_DEBUG=all ./roguelike 2> out.txt
./roguelike