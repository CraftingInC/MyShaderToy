# My Version of Shadertoy written in C

YOUTUBE DEMONSTRATION  
https://www.youtube.com/watch?v=AE0gdy_-90U  
  
The SHADERTOY website is not owned or ran by me. I just wanted a way to test their code using my own executable, without the need for their website.  
Shadertoy website : https://www.shadertoy.com/view/fd33zn  
The code from that URL will work with some miner tweaking of the void main().  

HOW THIS WORKS  
--------------
When running, you can edit the shader.txt file. Once you make a change to that shader code, click onto the running program and hit R to reload, and it will hot-reload your shader. Read the LOG.TXT file for any errors.  
  
TODO
--------------
I eventually want to add multi-texture ( all 4 channels ) as well as Sound.  
I also want to make a GUI so that you won't need a separate text editor to edit the shader. Currently, if the shader fails, the program fails. ( Although the window might stay open, it will just show a black screen. )  
  
WINDOWS SETUP  
--------------  
Get GCC for windows -- Tested this stable version on windows 10  
https://nuwen.net/mingw.html  
  
Get GLFW3 - Get the 64-Bit Windows Pre-Compiled Binaries  
https://www.glfw.org/download.html  
EXAMPLE : https://github.com/glfw/glfw/releases/download/3.3.7/glfw-3.3.7.bin.WIN64.zip  
NOTE : Pay attention to the version number.  
Extract zip into libs folder.  
EXAMPLE : The include path should look like this -> libs/glfw-3.3.7.bin.WIN64/include  
  
Instead of GLEW I use GLAD.  
https://glad.dav1d.de/  
  
Run this whole line at the command prompt.  
gcc -O2 -m64 -o test main.c libs/glad/glad/glad.c -I./libs/glad/ -I./libs/glfw-3.3.7.bin.WIN64/include/ -L./libs/glfw-3.3.7.bin.WIN64/lib-mingw-w64/ -lglfw3 -lgdi32
  
then run test.exe  
  
  
  
LINUX SETUP - Tested with Ubuntu using VirtualBox  
--------------  
NOTE : I was able to get OpenGL 4.5 on VirtualBox with this setup.  
  
Get GCC for Linux  
sudo apt install gcc  
  
NOTE : After updating GLAD, Mesa 22.1.3 is now reporting correctly, but OpenGL 4.6 is still not working yet in VirtualBox. I'm not sure if it's GLFW3 or VirtualBox that has the problem. OpenGL 4.5 still works though.  
https://itsfoss.com/install-mesa-ubuntu/  
  
If you need GLXINFO  
sudo apt install mesa-utils  
  
Get GLFW3  
sudo apt install libglfw3-dev  
  
Instead of GLEW I use GLAD.  
https://glad.dav1d.de/  
  
gcc -O2 -m64 -o test main.c libs/glad/glad/glad.c -I./libs/glad/ -I/usr/include/libdrm -lglfw -ldl
  
then run ./test  
