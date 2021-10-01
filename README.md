# README #

This work has been done for the Bachelor thesis for the game development program at University of Skövde.

This README would normally document whatever steps are necessary to get your application up and running.

### Summary ###

This project is the source code for a master thesis about ragdoll physics.

### What is this repository for? ###

* Quick summary
The purpose of this repository is to hold project code for a master thesis written by Peter Nilsson and Christian Larsson.

* Version
0.0.1 (Alpha-phase)

### How do I get set up? ###

* Summary of set up

* Configuration

This project used Visual Studio 2017 (v141).
It used Multi-Byte Character Set to work with the parts of OpenTissue (https://github.com/erleben/OpenTissue) that was included in the project.
C/C++ -> General -> Additional Include Directories: ".;Include;"
Linker -> Input -> Additional Dependencies: "DevIL.lib;ILU.lib;ILUT.lib;glut32.lib;opengl32.lib;glu32.lib;glew32.lib;cg.lib;cgD3D9.lib;cgD3D10.lib;cgD3D11.lib;cgGL.lib;TinyXML.lib;%(AdditionalDependencies)"

* Dependencies

DevIL Library (DevIL.lib, ILU.lib, ILUT.lib)
OpenGL Library (glut32.lib, opengl32.lib, glu32.lib, glew32.lib)
NVIDIA's "C for graphics" (cg.lib, cgD3D9.lib, cgD3D10.lib, cgD3D11.lib and cgGL.lib)
TinyXML Library (TinyXML.lib)

### Who do I talk to? ###

* Repo owner or admin

### Keybindings ###

#### General Bindings ####

| Keys					| Description														    |
| --------------------- | --------------------------------------------------------- |
| **Key \<Right-click\>** | opens menu 											    |
| **Key \<F1\>**			| changes to XY view									    |
| **Key \<F2\>**			| changes to ZY view									    |
| **Key \<F3\>**			| changes to zx view									    |
| **Key \<F4\>**			| changes to perspective view							    |
| **Key \<F5\>**			| toggle ragdoll joints (used for skinning in ragdoll demo) |
| **Key \<F9\>**			| toggle full-sceen											|
| **Key \<F10\>**			| toggle grid												|
| **Key \<F11\>**			| toggle snap												|
| **Key \<F12\>**			| toggle output												|
| **Key \<Shift-F1\>**	| particle demo												|
| **Key \<Shift-F2\>**	| particle cuboid demo										|
| **Key \<Shift-F3\>**	| joint demo												|
| **Key \<Shift-F4\>**	| hinge demo												|
| **Key \<Shift-F5\>**	| ragdoll demo												|
| **Key \<HOME\>**		| shows help												|
| **Key \<ESC\>**			| exits the program											|

#### Camera Bindings (Perspective View, 'F4') ####

| Special keys **without** <Ctrl> or <Alt> pressed | Description														    	   			  |
| ------------------------------------------------ | ---------------------------------------------------------------------------- |
| **Key \<Left\>**								   | circulates the camera **clockwise** around its focus point 	   			  |
| **Key \<Right\>**								   | circulates the camera **counterclockwise** around its focus point 			  |
| **Key \<Up\>**									   | moves the camera and its focus point **inwards** in its current direction    |
| **Key \<Down\>**								   | moves the camera and its focus point **outwards** from its current direction |
| **Key \<Page Up\>** 							   | moves the camera **upwards** (tilts the camera down)						  |
| **Key \<Page Down\>**							   | moves the camera **downwards** (tilts the camera up)						  |

| Special keys **with** <Ctrl> pressed | Description													      |
| ------------------------------------ | -------------------------------------------------------------------- |
| **Key \<Left\>**			  		   | circulates the camera **counterclockwise** around its own position   |
| **Key \<Right\>**			  		   | circulates the camera **clockwise** around its own position		  |
| **Key \<Up\>**				  		   | moves the focus point **upwards** (tilts the camera up)			  |
| **Key \<Down\>**			  		   | moves the focus point **downwards** (tilts the camera down)		  |
| **Key \<Page Up\>** 		  		   | moves the focus point **upwards** (tilts the camera up)			  |
| **Key \<Page Down\>**		  		   | moves the focus point **downwards** (tilts the camera down)		  |

| Special keys **with** 'Shift' pressed | Description										  			  	|
| ------------------------------------- | ----------------------------------------------------------------- |
| **Key \<Up\>**							| same as 'Page Up' key without modifiers, for use if its missing	|
| **Key \<Down\>** 						| same as 'Page Down' key without modifiers, for use if its missing	|

#### Demo Ragdoll Bindings ####

| Number keys             	  | Description																																		  |
| --------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Key \<1-3\>**		  	  	  |	sets ragdoll type to 1, 2 or 3																													  |
| **Key \<4\>**			  	  |	sets constraint solver type to 1: uses relaxation. 3 relaxation loops																			  |
| **Key \<5\>**			  	  |	sets constraint solver type to 2: uses relaxation and priority on biggest displacement. 3 relaxation loops										  |
| **Key \<6\>**			  	  |	sets constraint solver type to 2: uses relaxation and priority on biggest displacement. 1 relaxation loops										  |
| **Key \<7\>**			  	  |	sets constraint solver type to 3: uses relaxation and priority on simulated closing velocity and simulated displacement. 3 relaxation loops. TODO |
| **Key \<8\>**			  	  |	sets constraint solver type to 4: uses relaxation and priority on largest closing velocity. 3 relaxation loops									  |


| Letter keys			  	  |	Description											  			 		   |
| --------------------------- | -------------------------------------------------------------------------- |
| **Key \<A\>**			  	  | gives the selected body part an upward force push				 		   |
| **Key \<B\>**			  	  | prints info about the bones of the ragdoll to console				 	   |
| **Key \<C\>**			  	  | rotates ragdoll skin along y-axel (experimental)					 	   |
| **Key \<D\>**			  	  |	switches draw state e.g. debug mode (normal, debug, skin only, no drawing) |
| **Key \<F\>**			  	  |	toggle skin rendering (experimental)		 		   					   |
| **Key \<G\>**			  	  |	toggle gravity										  			 		   |
| **Key \<I\>**			  	  |	starts recording scenario 2 statistics (resets simulation first) 		   |
| **Key \<J\>**			  	  |	increments selected body part index					  			 		   |
| **Key \<K\>**			  	  |	decrements selected body part index					  			 		   |
| **Key \<L\>**			  	  |	toggle ragdoll joint position rendering					  			 	   |
| **Key \<R\>**			  	  |	resets the simulation								  			 		   |
| **Key \<S\>**			  	  |	gives the selected body part an eastward force push				 		   |
| **Key \<T\>**			  	  |	starts recording statistics							  			 		   |
| **Key \<U\>**			  	  |	starts recording scenario 1 statistics (resets simulation first) 		   |
| **Key \<X\>**			  	  |	rotates ragdoll skin along x-axel (experimental)	  			 		   |
| **Key \<Y\>**			  	  |	stops recording statistics and cancels the process	  			 		   |
| **Key \<Z\>**			  	  |	rotates ragdoll skin along z-axel (experimental)	  			 		   |

| Mouse key			      	  | Description																		          |
| --------------------------- | ----------------------------------------------------------------------------------------- |
| **Mouse Key Left Pressed**  | force push the selected ragdoll part towards mouse position and draws a line between them |
