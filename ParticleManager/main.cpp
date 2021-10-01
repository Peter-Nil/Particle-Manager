#include "OpenGLStuff.h"
#include "DemoHandler.h"
// Demos.
#include "DemoParticle.h"
#include "DemoParticleCuboid.h"
#include "DemoJoint.h"
#include "DemoHinge.h"
#include "DemoRagdoll.h"

// The entry point for the program. You need to add your demos before you call start.
int main(int argc, char* argv[])
{
	Vector4 var = Vector4();
	var += 10.0f;
	DemoHandler::inst().addDemo(new DemoParticle());
	DemoHandler::inst().addDemo(new DemoParticleCuboid());
	DemoHandler::inst().addDemo(new DemoJoint());
	DemoHandler::inst().addDemo(new DemoHinge());
	DemoHandler::inst().addDemo(new DemoRagdoll());

	//DemoHandler::inst().setColor
	DemoHandler::inst().setActive(4);

	start(argc, argv); // function in "OpenGLStuff"
	
	return 0;
};


// Original work by Henrik Engström, henrik.engstrom@his.se (2009)
// Modified work by Peter Nilsson and Christian Larsson licensed under CC BY-SA 4.0