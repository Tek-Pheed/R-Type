# {E} R-Type

## Overview

R-TYPE is a project designed to introduce developers to networked video game
development, focusing on advanced C++ techniques and software engineering
practices. The objective is to create a multi-threaded server and a graphical
client for a reimagined version of the classic horizontal shoot-'em-up game, *
*R-Type**, using a custom-designed game engine.

The development process is divided into two phases:

Core Architecture: Build the foundational elements of the game and deliver a
functional prototype.

Enhancements: Expand and refine the prototype, exploring specialized features
and techniques from a provided list of options.

R-Type, originally released in the late 1980s, became a massive success in the
gaming community, inspiring numerous ports, spin-offs, and remakes. While not
the first of its kind, it set a high standard for horizontal shoot-'em-up games.
Other notable games in this genre include the Gradius series and Blazing Star
for Neo Geo.

## 1. Requirements

This project requires the following:

- **Development Environment**: A modern C++ compiler with C++17 support.
- **Operating System**: Linux or Windows
- **Additional Tools**: CMake for project building and management.
- **Network Requirements**: The game must support multiplayer functionality,
  allowing several players to team up against the evil Bydos.
- **Game Engine Design**: The internal architecture must reflect the structural
  complexity and modularity of a real game engine.

## 2. Libraries Used

The project is developed using the following libraries:

- **SFML (Simple and Fast Multimedia Library)**: Used for graphics, audio, and
  event handling.
    - [Official SFML Website](https://www.sfml-dev.org/)
- **Criterion**: For unit testing during development.
    - [Criterion](https://github.com/Snaipe/Criterion)

## 3. How to Launch

### Build and launch R-Type

This projet works with Dockerfile.

First build the docker image :

```bash
docker build -f Dockerfile -t r-type:latest .
```

Now run it :

```bash
docker run --rm -it -v $PWD/r-type/ r-type
```

Once inside you can run the following command for building the projet:

```bash
./tools/build-linux.sh
```

### Read the documentation

You can run the mdbook server who contains the documentation of the project :

```bash
cd docs/
docker build -f Dockerfile -t r-type-mdbook:latest .
docker run --rm -it -p 3000:3000 r-type-mdbook
```

or you can go [here](https://r-type.leafs-studio.com) to see it !

## 4. Benchmarks

Benchmarks have been made in order to choose the best way to realise this
project, one for the languages and one for the graphical libraries.
You can find the [Benchmarks](./benchmarks.md) here.

## 5. Accessibility Compliance

The game has been developed and verified using the [WebAIM Contrast Checker](https://webaim.org/resources/contrastchecker/) to ensure it adheres to color accessibility standards. 

### Compliance Details:
- **Normal Text**: 
  - WCAG AA: Pass
  - WCAG AAA: Pass
- **Large Text**: 
  - WCAG AA: Pass
  - WCAG AAA: Pass
- **Graphical Objects and User Interface Components**: 
  - WCAG AA: Pass

This ensures the game respects color norms and is accessible to individuals with color vision deficiencies, such as daltonism.

## 6. Authors

The project was developed by the following team members:

- **[RICARD Alexandre]**
- **[Loustalot Lucas]**
- **[Scandella Raphaël]**
- **[AUGAIT Arnaud]**
- **[NASSET Samy]**

