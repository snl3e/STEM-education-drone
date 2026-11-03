#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fileDescriptor = open("/dev/ttyACM0", O_RDWR);
    struct termios tty{};

    if(fileDescriptor == -1) {
        std::cerr << "Failed to open /dev/ttyACM0\n";
        return 1;
    }

    //---Configure serial communication---
    if(tcgetattr(fileDescriptor, &tty) == -1){
        perror("tcgetattr");
        close(fileDescriptor);
        return 1;
    }

    
    //Set output & input baud rate
    if (cfsetispeed(&tty, B115200) == -1 || cfsetospeed(&tty, B115200) == -1){
        perror("cfset baud rate");
        close(fileDescriptor);
        return 1;
    }   

    cfmakeraw(&tty);                // Disable terminal processing   
    tty.c_cflag |= CREAD | CLOCAL;  // Enable the receiver             
    tty.c_cflag &= ~PARENB;         // Disable parity
    tty.c_cflag &= ~CSTOPB;         // Select one stop bit
    tty.c_cflag &= ~CRTSCTS;        // Disable hardware flow control
    tty.c_cflag &= ~CSIZE;          // Clear character-size bits
    tty.c_cflag |= CS8;             // Select 8 data bits

    if(tcsetattr(fileDescriptor, TCSANOW, &tty) == -1){
        perror("tcsetattr");
        close(fileDescriptor);
        return 1;
    }

    //---Send message to ESP32---
    const char message[] = "HELLO\n";
    write(fileDescriptor, message, sizeof(message) - 1);

    //---Read message from ESP32---
    char buffer[128];
    int bytesRead{0};
    while (bytesRead < sizeof(buffer) - 1) {
        ssize_t result = read(fileDescriptor, buffer + bytesRead, 1);

        if (result == -1) {
            perror("read");
            return 1;
        }

        if (result == 0) break;

        if (buffer[bytesRead] == '\n') break;

        bytesRead++;
    }
    
    buffer[bytesRead] = '\0';

    if (bytesRead > 0) {
        std::cout << "Message received: " << buffer;
    }

    close(fileDescriptor);
    return 0;
}