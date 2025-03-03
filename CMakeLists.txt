cmake_minimum_required(VERSION 3.10)

# Nome del progetto e versione
project(DomoticSystem VERSION 1.0 LANGUAGES CXX)

# Imposta lo standard C++ su C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Aggiungi i file header e sorgente
set(SOURCES
    src/CreaDispositivo.cpp
    src/Dispositivo.cpp
    src/Interfaccia.cpp
    src/LinkedList.cpp
    src/LinkedListOn.cpp
    src/LinkedListProg.cpp
    src/LinkedListOff.cpp
    src/mainInterfaccia.cpp
    src/RicercaDispositivo.cpp
)

set(HEADERS
    include/CreaDispositivo.h
    include/Dispositivo.h
    include/Interfaccia.h
    include/ListaDispositivi.h
    include/RicercaDispositivo.h
    include/LinkedList.h
    include/LinkedListOn.h
    include/LinkedListProg.h
    include/LinkedListOff.h
)

# Crea l'eseguibile
add_executable(DomoticSystem ${SOURCES} ${HEADERS})

# Specifica le directory dove cercare i file header
target_include_directories(DomoticSystem PRIVATE include)

# Imposta la directory di output per l'eseguibile
set_target_properties(DomoticSystem PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Aggiungi le opzioni di ottimizzazione
target_compile_options(DomoticSystem PRIVATE -O2)

