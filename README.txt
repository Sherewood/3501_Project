101067032
Julien Rougerie
Shader project:
The following variations have been added:
-Lit_fp has been modfied to handle Phong shading 
-Lit_fp has also been made to take in a vertex from the a specific vertex in the program
-Procedural has also been modfied 
-Now produces a checkerboard shader which changes with each the seconds of the timer 
New Shader has been created: new_shd_fp
-Basically the combined shaders of procedural and Lit_fp
-produces a shader which has a light source as well as a color 
Modifcations to the code itself
-three items have been made, each taking in either 1 of the 3 shaders. If you don't see them too clearly, look left and right 
-Light class has been created, it has two functions; getPostion which returns the position of the light (and converts it to the negative) and update, which changes what position the light holds.
-A light object has also been added to the code, it basically moves back and forth. The light vertex is tied to this object so most other objects should react as if it is the light source 
