# MSD fire simulation project

## Build instructions
- Linux
    ```bash
    mkdir build # Create a build directory
    cmake -B build
    cmake --build build -j$(nproc)
    ```
## Run the project
- Linux 

    After the project is built, you can run it using the following command
    ```bash
    ./build/msd_projekt
    ```
## Sterowanie
Sterowanie kamerą odbywa się za pomocą lewego przycisku myszy oraz klawiszów W, S, A, D, ctrl, Space.
W celu wywołania pożaru, należy chwilę przytrzymać klawisz Z.
Przysiśnięcie klawisza Q pozwala na zobaczenie rozkładu temperatury.

## Automat komórkowy
Implementacja automatu komórkowego zawarta jest w plikach lattice3d.h oraz lattice3d.cpp.
W nich zdefiniowana jest klasa Lattice3d, która agreguje obiekty typu Cell, na których wykonywane są obliczenia automatu.
Mechanizm zmiany stanu komórek zawarty został w funkcji update() w klasie Lattice3d.
