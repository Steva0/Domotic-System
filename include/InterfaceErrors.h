#include <exception>
#include <string>

class invalidTimeFormat : public std::exception {
public:
    explicit invalidTimeFormat(const std::string& timeType)
        : message("Formato orario [" + timeType + "] non valido! (mm:ss)") {}

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

class wrongCommandSyntax : public std::exception {
public:
    explicit wrongCommandSyntax(const std::string& command) {
        std::string exception;
        if(command == "set time") {
            exception = "Sintassi corretta: set time ${TIME}\n";
        }else if(command == "set device") {
            exception = "Sintassi corretta: set $DEVICE_NAME on/off\n";
            exception += "                   set $DEVICE_NAME ${START} [${STOP}]\n";
        }else if(command == "rm") {
            exception = "Sintassi corretta: rm $DEVICE_NAME\n";
        }else if(command == "reset") {
            exception = "Sintassi corretta: reset time/timers/all\n";
        }else if(command == "set") {
            exception = "Sintassi corretta: set time ${TIME}\n";
            exception += "Sintassi corretta: set $DEVICE_NAME on/off\n";
            exception += "                   set $DEVICE_NAME ${START} [${STOP}]\n";
        }
        message = exception;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};