# ErrorClass

## ErrorClass.cpp

This class is used to define custom runtime errors. It inherits from the `std::runtime_error` class and provides custom constructors to throw exceptions with specific error messages.

### Functions

- `ErrorClass(const std::string &message)`: Constructor that accepts a string message to initialize the error.
- `ErrorClass(const char *message)`: Constructor that accepts a C-style string message to initialize the error.

### Example Usage

```cpp
#include "ErrorClass.hpp"

int main() {
    try {
        throw ErrorClass("Custom error occurred");

    } catch (const ErrorClass &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
