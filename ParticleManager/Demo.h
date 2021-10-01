#pragma once
#include <string>

using namespace std;
namespace pc
{
	class DemoHandler;

	// This is the abstract base-class for demos. A descendant class Will implement the update-method which is called each frame by the demoHandler. Use the provided DemoHandler-object to draw and to read mouse and key events. Don't forget to add your demo in the Main.cpp-file.

	class Demo
	{
	public:
		virtual void update(DemoHandler*) = 0;
		const virtual string getName()
		{
			return "Please name me!";
		}
		const virtual string getInfo()
		{
			return " \nPlease describe your work by returning a description from the \"getInfo\"-method\n\nThank you!\n";
		}
	};
}

// Original work by Henrik Engström, henrik.engstrom@his.se (2009)
// Modified work by Peter Nilsson and Christian Larsson licensed under CC BY-SA 4.0