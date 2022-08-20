# CS350_w.huang
## CS350 Advanced Computer Graphics II

This demo showcases various submissions for CS350 Advanced Computer Graphics II.</br>
The framework behind this program is written by me, it uses the Vulkan graphics API.</br>
The framework was originally written for C++20 but has been rewritten to target C++17.</br>
This was done to facilitate building on the lab machines in DigiPen Institute of Technology Redmond campus.<br />
The original can be found in my other repository: [CS2150-MT](https://github.com/owenwensong/CSD2150-MT)</br>

This demo requires the user to have a graphics driver supporting Vulkan API 1.1 or above.</br>
To view the demo without compiling, download the CS350Demo from the releases page or [click here](https://github.com/owenwensong/CS350_w.huang/releases/download/PublicRelease/CS350Demo.zip).

## To compile a version for yourself
1. Clone the repository.
2. Open the solution in Visual Studio and compile for your desired architecture (It is recommended to build in release mode).

#### Note: This version does not use getDependencies.bat

## Project requires
- A version of Visual Studio supporting C++17

#### Note: This version does not require the Vulkan SDK, instead the library files from Vulkan SDK 1.2.198.1 is included in dependencies. 

#### Important folders for the project
- Assets folder (for the meshes, textures, shaders used)
- prop-pages folder (QOL so no need to manually copy DLLs and such)
- dependencies folder (precompiled dependencies provided for this project)
