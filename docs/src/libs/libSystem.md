# LibSystem

LibSystem is an encapsulation of OS system calls and primitives.
It is designed to be built on Windows and Linux for use with the R-Type project.

This library provides encapsulation for network sockets (TCP and UDP), and filesystem operations.

## Building the library
Simply run the cmake at the root of the `libs/system` folder.
This library will be built as a shared library (.so on Linux and .dll on Windows)

## Using the Library
To use the library you will need to link your project either at compile time (in cmake) or at runtime
(with dlopen for example on Linux).

The header files for developpement are located in the `libs/system` folder.

# Network
Headers:
- `libs/system/system_network.hpp`
  - This header is required for accessing network functions.
- `libs/system/system_tcp.hpp`
  - This header is required for accessing functions related to TCP sockets.
- `libs/system/system.hpp`
  - General library header

## TCP Sockets
LibSystem provides a class for cross platform TCP sockets.
This class contains the following functions:

- `TCPSocket(uint16_t port, TCPMode mode, const std::string &address = "0.0.0.0")`: Constructor of the a TCPSocket object.
  <br>You should a least provide a port and a mode.
The mode can be either:
    - `SERVE` to accept incomming connections
    - `CONNECT` to connect to another socket.

- `ssize_t sendData(const byteArray &byteSequence)`: Send data through the socket:
  - You should provide a byteArray object containing the data to send.

- `byteArray receive(void)`: Receive data through the socket. This call could block if the socket is invalid or if there is no data available when called.
  - This function returns a byteArray object containing the received data.

- `void closeSocket(void)`: Closes the socket.
  - The socket should not be used after calling this function.

- `bool isOpen(void)`: Check if the socket is connected and opened.

- `uint64_t getUID(void) const`: Get the unique identitifer of this socket

Additionnal syscall encapsulation are also provided:
- `void select(socketSetTCP *readfds = nullptr, socketSetTCP *writefds = nullptr, socketSetTCP *exceptfds = nullptr, timeoutStruct timeout = std::nullopt)`: The Linux select function applied to TCPSocket objects. This function modifies the sets passed as argument.

- `TCPSocket accept(const TCPSocket &src)`: The Linux accept function applied to TCPSocket objects.

## UDP Sockets
TODO

# Filesystem