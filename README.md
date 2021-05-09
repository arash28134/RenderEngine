# RenderEngine

RenderEngine is a small rasterization render framework that aims to ease the development of any 3D related application.

It uses OpenGL as graphics driver, although the architecture isolates the interaction with the driver in a set of classes that makes easier the replacement of the former.

## Architecture

The main components of the framework are defined and implemented in **rendercomp**. A set of sample applications which uses the framework can be found in **apps**. This project tries to be as sefl-contained as possible to easy the building process. For this purpose, the **deps** folder contains almost all of the framework dependencies as git submodules. Finally, the **CMake** folder contains some tools to easy the building process, given that both Windows and Linux based systems are supported.

## Building

### Requirements

To build the engine, the following tools and libraries must be present on the system as hard dependencies:

* **git**
* **cmake** 3.15 or above
* **OpenGL** 4.6 or above

If running in a Linux based system, **FreeImage** must be installed (it is typically available in the common package managers, such as **apt** and **yum**). For Windows, a precompiled DLL is available within this project.

### Soft dependencies

The project also uses the following dependencies:

* **GLFW** 3.3.2 for window management.
* **GLM** 0.9.9.8 for linear algebra math.
* **Assimp** 5.0.1 for poligonal mesh loading (.ply, .obj, .stl, .fbx, ...)
* **EASTL** 3.17.06 for optimized templated containers
* **FreeImage** 3.18 for image loading (supports almost all formats)
* **GLAD** for OpenGL extension management.

All of this dependencies are already contained in the application, and the CMake build system will not bother looking for them in your system (as you might have any of them installed with a different version that might break compilation). The building process (explained ahead) will download them all.

### Build process

Building the engine, both in Windows and Linux based system, requires execution of very few commands:

```shell
git clone --recursive https://github.com/NadirRoGue/RenderEngine.git
mkdir RenderEngine/build
cd RenderEngine/build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake . --build
```

Once the process is complete, in the **build/bin// directory, all executables from the samples can be
found and executed. 
