#pragma once

#include <string>

class SerialPort {
    private:
        std::string devicePath{};
        int baudRate{};
        int fileDescriptor{-1};
    public: 
        SerialPort(const std::string &devicePath, int baudRate);
        ~SerialPort();
        bool open();
        bool close();
    };