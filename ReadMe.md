# dependencies
windwos.h
powrprof.h

# compile and run
To compile backend:
`g++ src/backend/battery.cpp -o bin/battery_app -lpowrprof -Isrc/backend`

Start programm:
`./battery_app`

Cmd args:
`--hibernate`
`--sleep`
`--add-to-start-up`