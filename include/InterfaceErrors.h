// Fabrizio Scabbia

#ifndef INTERFACE_EXCEPTIONS_H
#define INTERFACE_EXCEPTIONS_H

#include <exception>
#include <string>

// Eccezione invalidTimeFormat quando viene inserito un orario in un formato errato
class invalidTimeFormat : public std::exception {
public:
    explicit invalidTimeFormat(const std::string& timeType);
    const char* what() const noexcept override;

private:
    std::string message;
};

// Eccezione wrongCommandSyntax lanciata quando viene inserito un comando con una sintassi errata
class wrongCommandSyntax : public std::exception {
public:
    explicit wrongCommandSyntax(const std::string& command);
    const char* what() const noexcept override;

private:
    std::string message;
};

#endif // INTERFACE_EXCEPTIONS_H
