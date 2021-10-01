#pragma once
#include "Demo.h"
#include "Point.h"
#include <vector>
#include <string>
using namespace std;
#ifdef _WIN32
#include <OpenTissue\Utility\GL\GLUtil.h>
#else
#include <GLUT/glut.h>
#endif
// This gives the API to the DemoHandler-class that you use to draw points, lines, text, polygons and to read keyboard and mouse.

namespace pc
{
	enum Color { RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, GRAY, WHITE, BLACK, PINK }; // The colors for points, lines, text and polygons.
	enum View { XY_PLANE, ZY_PLANE, ZX_PLANE, PERSPECTIVE }; // The views we can select (3 ortographic and a perspective view).
	enum Button { RIGHT, MIDDLE, LEFT }; // The mouse buttons.

	class DemoHandler
	{
		// The index of the active demo.
		unsigned mActiveDemo;
		// Vector of registered demos.
		std::vector<Demo*> mDemos;
		Point mMousePos;
		View mView;
		// A vector of all added console-lines. The memory will be freed at exit. 
		std::vector<string> mTextLines;
		bool mMouseClicked[3];
		bool mMouseState[3];
		bool mKeyStates[256];
		GLUquadric* mGluQ;

		void clearEvents(); // Removes all reported mouse and key events. The mouse state is left unchanged.
		void setColor(Color); // Sets the specified color (in openGL).
	protected:
		DemoHandler(void);
		~DemoHandler(void);
	public:
		// Registers a demo to be handled. It will be added to the menu. The last added demo will be chosen at startup.
		void addDemo(Demo*);
		// Adds one (or several lines separetad with '/n') to be shown in the console.
		void addMessage(const string&);

		/// Draw functions.
		// Draws a point with the specified color and size (radius).
		void drawPoint(const Point&, const Color = GRAY, const float size = 0.1);
		// Draws a point with the specified color and size.
		void drawBox(const Point&, const Color = GRAY, const Point& size = Point(3, 3, 3), const Point& rotateAxis = Point(1, 0, 0), const float angleInDegrees = 0);
		// Draws a line between two points (with the given color and width (radius)). Note openGL may not draw large line widths (>0.15) in ortographic view.
		void drawLine(const Point&, const Point&, const Color = GRAY, const float width = 0.05);
		// Draws a text at the given position.
		void drawText(const Point&, const string, const Color = WHITE);
		// Draws a convex polygon with the given color. Points should lie in the same plane and be ordered counter-clockwise. 
		void drawPolygon(const std::vector<Point>&, const Color = WHITE);
		// Draws a plane (actually a rect) containing the given point and with the given normal. The size of the rect can be specified.
		void drawPlane(const Point&, const Point&, const Color = WHITE, const float = 1000);

		/// Input functions
		// Returns the mouse location as a point. The mouse location is only translated to a point when we have a ortographic view. In perspective view mouse motion will be ignored.
		const Point getMouseLocation();
		// Returns true if the specified mouse button has been clicked since last update.
		bool mouseClicked(const Button b = LEFT);
		// Returns true if the given mouse button is currently pressed.
		bool isMouseDown(Button b = LEFT);
		// Returns true if the specified character has been typed since last update.
		bool keyTyped(const char);

		// To get the (singleton) handler object. It is only needed to add demos.
		static DemoHandler& inst();

		/// Methods that are only used internally by OpenGLStuff.
		// Sets the current mouse position in 3d. (one component will be 0)
		void setMousePos(float, float, float);
		// Report a mouse-click on the specified button.
		void setMouseClick(Button);
		// Report a change of state for the specified button (true == down, false == up).
		void setMouseState(Button, bool);
		// Report that a key has been typed.
		void setKeyTyped(char);
		// Returns the number of registered demos.
		int getDemoCnt();
		// Returns the name of the spcified demo.
		const string getName(int);
		// Select the specified demo as active. First demo starts at 0.
		void setActive(int);
		// Returns the index of the active demo. First demo starts at 0.
		int getActive();
		// Signals that the demo should be updated.
		void update();
		// Outputs the console.
		void printConsole(int, int);
		// Changes the current view.
		void setView(View);
	};
}

// Original work by Henrik Engström, henrik.engstrom@his.se (2009)
// Modified work by Peter Nilsson and Christian Larsson licensed under CC BY-SA 4.0