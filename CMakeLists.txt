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
    src/RicercaDispositivo.cpp
    src/mainNode.cpp
    src/LinkedList.cpp
    src/LinkedListOff.cpp
)

set(HEADERS
    include/CreaDispositivo.h
    include/Dispositivo.h
    include/ListaDispositivi.h
    include/RicercaDispositivo.h
    include/LinkedList.h
    include/LinkeListOff.h
)

# Crea l'eseguibile
add_executable(DomoticSystem ${SOURCES} ${HEADERS})

# Specifica le directory dove cercare i file header
target_include_directories(DomoticSystem PRIVATE include)

# Imposta la directory di output per l'eseguibile
set_target_properties(DomoticSystem PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Aggiungi l'eseguibile del test
add_executable(main_test ${SOURCES})

# Abilita il testing e aggiungi il test
enable_testing()
add_test(NAME main_test COMMAND main_test)
