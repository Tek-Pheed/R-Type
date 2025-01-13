# Render

## Render.cpp

This class is used to render a window with SFML. It manages the window's size,
title, and frame rate, as well as events like closing the window.

### Functions

- `getWindow()`: Returns the SFML window object.
- `getTitle()`: Returns the current title of the window.
- `setTitle(newTitle)`: Sets a new title for the window.
- `getFrameRate()`: Returns the current frame rate of the window.
- `setFrameRate(newFrameRate)`: Sets a new frame rate for the window.
- `renderWindow()`: Starts the rendering loop and continuously updates the
  window until it is closed.
- `playEvent()`: Catches and handles events, such as closing the window.

#### Errors are managed thanks to the [Error Class](./errorclass.md)

### Variables

- `window`: The SFML window object used to display the content.
- `title`: The title of the window.
- `videoMode`: Holds the window's width and height (using SFML's
  `sf::VideoMode`).
- `frameRate`: The maximum frame rate of the window.
- `width`: The width of the window.
- `height`: The height of the window.

### Example Usage

```cpp
#include "RenderClass.hpp"

int main() {
    try {
        RenderClass renderWindow(800, 600, "My Game", 60);
        renderWindow.renderWindow();
    } catch (const ErrorClass &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
