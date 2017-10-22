rovernmake: rover.c hc-sr04/hc-sr04.c l298n/l298n.c
	gcc rover.c hc-sr04/hc-sr04.c l298n/l298n.c -o rover -lwiringPi -lm