# Render components

Render components (library name: *libRenderComponents*, namespace **rendercomp**) is a library of resusable 3D rendering components which contains the most common functionalities that a typical 3D application would use: Camera, Program (shaders), GPU Buffers, Asset importers, etc.

Interaction with the underlying graphics driver is done here (OpenGL, DirectX, Vulkan, etc.), so that any application or higher level library that uses it do not have to known the specific of such driver.
