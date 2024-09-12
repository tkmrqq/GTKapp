#dependencies
windwos.h
powrprof.h

#compile
`g++ battery.cpp -o battery -lPowrProf`
`./battery`
`--hibernate --sleep`
`g++ src/backend/battery.cpp -o bin/battery_app -lpowrprof -Isrc/backend`