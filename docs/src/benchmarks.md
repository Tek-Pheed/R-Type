# Benchmarks

## Programming Languages

### 1. Performance

- **C++** : Excellent performance, especially for critical applications like video games, thanks to precise control over memory and resources. It is the language of choice for real-time systems requiring low latency and fast computations.

- **Rust** : Offers performance close to C++ with memory safety guarantees, avoiding common errors such as null pointer dereferences or buffer overflows.

- **Go** : Sufficient for network servers but may struggle with real-time graphics compared to C++ or Rust.

- **Python** : Not suitable for performance-critical parts like real-time rendering but useful for prototyping or scripting due to its simplicity and fast development.

### 2. Ease of Development

- **C++** : Complex due to manual memory management and advanced object-oriented design. It requires significant rigor but offers immense flexibility.

- **Rust** : Simpler than C++ for memory management, thanks to its borrowing and ownership system.

- **Go** : Less complex than C++ or Rust but offers fewer features for fine-grained resource management.

- **Python** : The easiest to use but not suited for critical game parts where performance is paramount.

### 3. Concurrency and Multithreading

- **C++** : Provides fine-grained thread management but with significant complexity to avoid race conditions and blocking locks.

- **Rust** : Features a default safe concurrency model, preventing thread and memory access errors.

- **Go** : Excellent for network applications with its lightweight goroutine model but less performant for critical tasks like graphics rendering.

- **Python** : While it supports multithreading, its Global Interpreter Lock (GIL) limits performance for CPU-bound tasks. However, it remains effective for IO-bound tasks.

### 4. Support and Ecosystem

- **C++** : Has a vast ecosystem of libraries and tools, particularly in video game development (e.g., SFML, SDL) and game engines.

- **Rust** : Its ecosystem is growing, with an increasing number of libraries, but it remains smaller than C++'s for game development.

- **Go** : Primarily used for network and backend applications, with a strong ecosystem in this domain but limited usage in game development.

- **Python** : Boasts a massive ecosystem for scripting, prototyping, and automation tools, but is very limited for real-time game development.

### Conclusion : Why C++ is the Best Choice for Our R-Type Project

After comparing various programming languages, C++ emerges as the most suitable language for developing our project:

### Optimal Performance
C++ is designed for applications requiring fine resource management and performance, essential for a real-time network game. It enables memory management and code optimization to meet the needs for low latency and fast execution, particularly for graphics rendering and multithreading.

### Total Control
C++ provides control over all system aspects, including thread management, direct memory access, and code optimization. For a project like R-Type, which requires a multithreaded server, UDP network communications, and a custom game engine, this level of control is crucial.

### Rich Ecosystem for Video Games
C++ is one of the best languages for creating game engines, with an extensive library of tools (e.g., SFML, SDL) and utilities (e.g., CMake) that facilitate game development. Leading game engines like Unreal Engine are based on C++, reinforcing its suitability for game design.

### Portability and Cross-Platform Compatibility
C++ ensures high performance across different operating systems (Linux, Windows), which is a project constraint. The use of tools like CMake and Conan for dependency management further aligns with C++ capabilities.

### Project Requirements Compliance
The project demands creating a custom game engine without using pre-existing engines like Unity. C++ is the ideal language for building a game engine while ensuring performance and flexibility.

### Final Verdict
C++ is the perfect choice for developing a real-time network game with high performance and fine-grained resource management requirements. It offers the flexibility and power necessary to successfully complete a project like ours.

## Comparison of Graphics Libraries

### 1. SFML (Simple and Fast Multimedia Library)

**Supported Language :** C++

#### Advantages :
- Simple to use and integrate, particularly well-suited for a project like R-Type.
- Provides modules for rendering, input handling, audio, and networking, making it a comprehensive solution.
- Well-documented and active, with a large community.
- Cross-platform (Windows, Linux, macOS).
- A good balance for projects requiring graphics, networking, and audio elements.
- Compatible with multithreading solutions, necessary for the multiplayer server in the R-Type project.

#### Disadvantages :
- Limited for 3D games or projects with advanced graphical needs.
- Less performant than specialized libraries for advanced rendering tasks (e.g., OpenGL, SDL).

### 2. SDL (Simple DirectMedia Layer)

**Supported Language :** C (with bindings for C++ and other languages)

#### Advantages :
- Highly flexible and powerful, especially for 2D projects.
- Supports many platforms (Windows, Linux, macOS, Android, iOS).
- Excellent for low-level rendering.
- Used in many games (e.g., *Divinity : Original Sin* and *Dead Cells*).
- Great for input handling and graphics rendering.

#### Disadvantages :
- Does not provide integrated modules for audio and networking as SFML does, requiring additional libraries.

### 3. Raylib

**Supported Language :** C (with bindings for multiple languages)

#### Advantages :
- Designed for learning projects or rapid prototyping.
- Supports OpenGL for advanced graphical rendering.
- Performs well for 2D games with simple needs.

#### Disadvantages :
- Less comprehensive than SFML in terms of integrated features (e.g., audio and networking).
- Not well-suited for projects with complex networking needs, such as multiplayer.

### 4. OpenGL (with GLFW for window management)

**Supported Language :** C/C++

#### Advantages :
- High performance for advanced graphical projects, especially 3D rendering.
- Total control over the graphics pipeline, ideal for custom effects.

#### Disadvantages :
- Very complex to use.
- Requires separate libraries for input handling and audio (e.g., GLFW).
- Overkill for 2D games with simple networking needs.

### 5. Vulkan

**Supported Language :** C/C++

#### Advantages :
- Excellent resource and performance management, especially for multi-core platforms.
- Cross-platform (Windows, Linux, Android).
- Ideal for high-performance graphical projects (e.g., 3D game engines, complex simulations).

#### Disadvantages :
- Unsuitable for simple projects or prototypes due to its complexity.

### 6. DirectX

**Supported Language :** C++

#### Advantages :
- High performance on Windows, with excellent support for AAA games.
- DirectX 12 offers Vulkan-like performance and fine-grained hardware resource management.

#### Disadvantages :
- Not cross-platform, limited to Windows and Xbox.
- Complex, requiring significant development effort.

### 7. Allegro

**Supported Language :** C/C++

#### Key Features :
Allegro is a simple, cross-platform library designed for 2D game development. It includes functionality for handling sound, input, and graphics.

#### Advantages :
- Easy to use, particularly suited for 2D projects and small graphical applications.
- Handles input (keyboard, mouse, controllers), sound, and graphics.

#### Disadvantages :
- Fewer advanced features compared to SFML or SDL.
- Less widely used than other libraries like SFML or SDL, leading to fewer community resources.

## Conclusion

For the R-Type project, we chose the SFML library because it is the ideal choice, offering simplicity, versatility, and rapid development. It meets the project's requirements for 2D rendering, networking, audio, and input handling.

### Why SFML is a Good Choice :

1. **Ease of Use** : Its intuitive API enables quick learning, allowing us to focus on game development without a complex learning curve.

2. **Integrated Multimedia** : SFML handles 2D rendering, sound, input (keyboard, mouse, controllers), and even networking, making it ideal for the R-Type project without requiring multiple libraries.

3. **Sufficient Performance** : While less optimized than low-level libraries like OpenGL or Vulkan, SFML is more than adequate for a 2D multiplayer game.

4. **Cross-Platform Compatibility** : SFML works seamlessly on Windows, Linux, and macOS, enabling multi-platform development without extra effort.

5. **Community and Documentation** : A comprehensive documentation and active community make problem-solving easier.

In summary, SFML allows us to create a functional prototype quickly. It is an excellent choice for a 2D video game project like R-Type, where the focus is on gameplay and network structure rather than complex graphical effects.
