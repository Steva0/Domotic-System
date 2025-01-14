// Fabrizio Scabbia

#include "../include/InterfaceErrors.h"

// Implementazione dell'eccezione invalidTimeFormat con informazioni sull'orario sbagliato
invalidTimeFormat::invalidTimeFormat(const std::string& timeType)
    : message("Formato orario [" + timeType + "] non valido! (mm:ss)") {}

const char* invalidTimeFormat::what() const noexcept {
    return message.c_str();
}

// Implementazione dell'eccezione wrongCommandSyntax con messaggio la sintassi del corretta del comando inserito 
wrongCommandSyntax::wrongCommandSyntax(const std::string& command) {
    if (command == "set time") {
        message = "Sintassi corretta: set time ${TIME}\n";
    } else if (command == "set device") {
        message = "Sintassi corretta: set $DEVICE_NAME on/off\n"
                  "                   set $DEVICE_NAME ${START} [${STOP}]\n";
    } else if (command == "rm") {
        message = "Sintassi corretta: rm $DEVICE_NAME\n";
    } else if (command == "reset") {
        message = "Sintassi corretta: reset time/timers/all\n";
    } else if (command == "set") {
        message = "Sintassi corretta: set time ${TIME}\n"
                  "Sintassi corretta: set $DEVICE_NAME on/off\n"
                  "                   set $DEVICE_NAME ${START} [${STOP}]\n";
    } else {
        message = "Comando sconosciuto.";
    }
}

const char* wrongCommandSyntax::what() const noexcept {
    return message.c_str();
}
