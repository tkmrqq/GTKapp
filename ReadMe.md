# Dependencies
For backend:
- windwos.h
- powrprof.h
- setupapi.h

For frontend:
- gtk4

# Compile and run
To compile backend:
`use CMake`
>[!NOTE]
>Use Ninja to build

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

use CMake separately for two programs