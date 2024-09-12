# dependencies
For backend:
- windwos.h
- powrprof.h

For frontend:
- gtk4

# compile and run
To compile backend:
`g++ src/backend/battery.cpp -o bin/battery_app -lpowrprof -Isrc/backend`

Start programm:
`./battery_app`

Cmd args:
`--hibernate`
`--sleep`
`--add-to-start-up`

To compile frontend
- `mkdir build`
- `cd build`
- `cmake ../build`
- `cmake --build`