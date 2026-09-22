
This engine was originally designed for 32-bit x86 machines using the MSVC 2003 toolchain; it is currently undergoing a porting process to 64-bit x86_64/AMD64 and a cross-platform CMake GCC/MSVC toolchain.

1. Extract source code from original installer, clean up vulnerabilties and clean up windows binaries (`.exe`, `.dll`, `.lib`, `.ren`, etc)

2. [[Porting 1]]: CMake and GCC in Archlinux x86_64/AMD64, 64bit machines

3. [[Vulkan 1]]: Initial Vulkan tests with SDL2/SDL3
   There are issues with the Arch Linux packages: the SDL2 development package is no longer in the main repositories. I tried using the SDL3 development package; I will have to support both, since the developers believe it is the same technology due to the similarity in names.

4. [[sealhunter 1]]: Added `sealhunter` self-contained demo

5. [[sealhunter 2]] Decopling demo of that self-containment and recoupling it to our engine fork

6. [[sealhunter 3]]: Running game menu but broken renderization in gameplay

7. [[Porting 2]]: CMake and MSVC in Windows 10 x86_64/AMD64

8. [[sealhunter 4]]: Scene renderization regenerated 
   problems with saturation and animation
   gameplay is broken
   I get the feeling that the physics aren't working yet, but I can't confirm it because the gameplay hasn't been fixed yet. The collision between models and maps, and the sliding on ice, depend on the gameplay code.
   
9. [[lithrez]]: tool restored
   This was the original tool for packing and unpacking .rez files.

10. [[Documentation 1]]: Low quality AI generated documentation needs reinterpretation and rewrite


