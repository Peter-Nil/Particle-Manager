#include <cmath>
#include "DemoHandler.h"
#include "Vector.h"
#ifdef _WIN32
//#include <OpenTissue\Utility\GL\GLUtil.h>
//#include <GL/glut.h>
//#include "glut.h"
#include <algorithm>
#else
#include <GLUT/glut.h>
#endif
#include <sstream>

using namespace pc;
using namespace std;

// Red, green, blue, yellow, magenta, cyan, gray, white, black, pink.
static float color[][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 0}, {1, 0, 1}, {0, 1, 1}, {.5f, .5f, .5f}, {1, 1, 1}, {0, 0, 0}, {1, .4f, .7f} };

DemoHandler& DemoHandler::inst()
{
	static DemoHandler mHandler = DemoHandler();
	return mHandler;
}

DemoHandler::DemoHandler(void)
{
	mGluQ = 0;
	clearEvents();
	mMouseState[LEFT] = false;
	mMouseState[MIDDLE] = false;
	mMouseState[RIGHT] = false;
}

DemoHandler::~DemoHandler(void)
{
	if(mGluQ != 0) gluDeleteQuadric(mGluQ);
	mTextLines.clear();
}


void DemoHandler::setColor(const Color C)
{
	//if(mView == PERSPECTIVE)
	//{
		glColor4d(1, 1, 1, 1);
		GLfloat diffuseMaterial[] = {color[C][0], color[C][1], color[C][2], 1.0};
		GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0};
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuseMaterial);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//} else
	//	glColor3f(color[C][0], color[C][1], color[C][2]);
}

void DemoHandler::drawPoint(const Point& P, const Color C, const float Size)
{	
	glEnable(GL_POINT_SMOOTH);
	glPointSize(Size * 50 * 2);	
	setColor(C);
	//if(mView == PERSPECTIVE)
	//{
		glPushMatrix();
		glTranslated(P.mX, P.mY, P.mZ);
		gluSphere(mGluQ, Size, 64, 64);
		glPopMatrix();
	//} else
	//{
	//	glBegin(GL_POINTS);
	//	if(mView == XY_PLANE) // We addd the half size to lift the point from other stuff at the same dist (e.g. polys).
	//		glVertex3f(P.mX, P.mY, P.mZ + Size * 0.5);
	//	else if(mView == ZY_PLANE) glVertex3f(P.mX - Size * 0.5, P.mY, P.mZ);
	//	else glVertex3f(P.mX, P.mY + Size * 0.5, P.mZ);
	//	glEnd();
	//}
}


void DemoHandler::drawBox(const Point& P, const Color C, const Point& Size, const Point& rotateAxis, const float angleInDegrees)
{
	setColor(C);
	glPushMatrix();
	glTranslated(P.mX, P.mY, P.mZ);		
	glRotatef(angleInDegrees, rotateAxis.mX, rotateAxis.mY, rotateAxis.mZ);
	glScalef(Size.mX, Size.mY, Size.mZ);
	glutSolidCube(1);
	glPopMatrix();
}

void DemoHandler::drawLine(const Point& P1, const Point& P2, const Color C, const float Width)
{
	setColor(C);
	//if(mView == PERSPECTIVE)
	//{
		Point d = Point(P2.mX - P1.mX, P2.mY - P1.mY, P2.mZ - P1.mZ);		
		float length = sqrt(d.mX * d.mX + d.mY * d.mY + d.mZ * d.mZ);
		if(length < 0.01) return;
		glPushMatrix();
		glTranslatef(P1.mX, P1.mY, P1.mZ);
		
		if(fabs(d.mZ) > 0.01)
		{
			float angle = 180.0 / 3.14159 * acos(d.mZ / length);
			if(d.mZ < 0) angle *= -1;
			if((fabs(d.mX) < 0.01) && (abs(d.mY) < 0.01)) glRotatef(angle, 1, 0.0, 0.0);		
			else glRotatef(angle, -d.mY * d.mZ, d.mX * d.mZ, 0.0);
		} else glRotatef(-90, d.mY, -d.mX, 0.0);
		
		gluCylinder(mGluQ, Width, Width, length, 8, 1);
		glPopMatrix();
	//}	else 
	//{
	//	glLineWidth(Width * 50 * 2);
	//	glBegin(GL_LINES);
	//	glVertex3f(P1.mX, P1.mY, P1.mZ);
	//	glVertex3f(P2.mX, P2.mY, P2.mZ);
	//	glEnd();
	//}
}

void DemoHandler::drawText(const Point& P, const string Text, const Color C)
{
	if(Text.length() < 1) return;
	setColor(C);
	glDepthFunc(GL_ALWAYS); // We want the text to be visible even if the sphere is bigger (in perspective mode).
	glNormal3d(1, 0, 0);
	glPushMatrix();
	glTranslatef(P.mX, P.mY, P.mZ);
	glRasterPos2f(0, 0);	
	for(unsigned i = 0; i < Text.length(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, Text[i]);
	glPopMatrix();
	glDepthFunc(GL_LESS);
}

void DemoHandler::drawPolygon(const std::vector<Point>& pts, Color c)
{
	if(pts.size() < 3) return;
	pc::Vector p0 = pc::Vector(pts[0]);
	pc::Vector p1 = pc::Vector(pts[1]);
	pc::Vector p2 = pc::Vector(pts[2]);
	
	pc::Vector n = (p2 - p1) % (p0 - p1);
	n.normalize();
	
	setColor(c);
	glBegin(GL_POLYGON);	
	for(unsigned i = 0; i < pts.size(); i++)
	{
		glNormal3f(n.mX, n.mY, n.mZ);
		glVertex3f(pts[i].mX, pts[i].mY, pts[i].mZ);
	}
	glEnd();
}

void DemoHandler::drawPlane(const Point& p, const Point& nn, const Color color, const float size)
{
	float nl = sqrt(nn.mX * nn.mX + nn.mY * nn.mY + nn.mZ * nn.mZ);
	if(nl < 0.0001) return;
	Point n = Point(nn.mX / nl, nn.mY / nl, nn.mZ / nl);
	Point v1 = Point(0, 1, 0);
	float l = v1.mX * n.mX + v1.mY * n.mY + v1.mZ * n.mZ;
	if(l > 0.9) v1 = Point(1, 0, 0);
	l = v1.mX * n.mX + v1.mY * n.mY + v1.mZ * n.mZ;	
	v1 = Point(v1.mX - n.mX * l, v1.mY - n.mY * l, v1.mZ - n.mZ * l);
	Point v2 = Point(v1.mY * n.mZ - v1.mZ * n.mY, v1.mZ * n.mX - v1.mX * n.mZ, v1.mX * n.mY - v1.mY * n.mX);
	v1 = Point(v1.mX * size, v1.mY * size, v1.mZ * size);
	v2 = Point(v2.mX * size, v2.mY * size, v2.mZ * size);
		
	Point p1 = Point(p.mX + v1.mX + v2.mX, p.mY + v1.mY + v2.mY, p.mZ + v1.mZ + v2.mZ);
	Point p2 = Point(p.mX + v1.mX - v2.mX, p.mY + v1.mY - v2.mY, p.mZ + v1.mZ - v2.mZ);
	Point p3 = Point(p.mX - v1.mX - v2.mX, p.mY - v1.mY - v2.mY, p.mZ - v1.mZ - v2.mZ);
	Point p4 = Point(p.mX - v1.mX + v2.mX, p.mY - v1.mY + v2.mY, p.mZ - v1.mZ + v2.mZ);
	setColor(color);
	glBegin(GL_QUADS);	
		glNormal3f(n.mX, n.mY, n.mZ);
		glVertex3f(p1.mX, p1.mY, p1.mZ);
		glVertex3f(p2.mX, p2.mY, p2.mZ);
		glVertex3f(p3.mX, p3.mY, p3.mZ);
		glVertex3f(p4.mX, p4.mY, p4.mZ);
	glEnd();
}


const Point DemoHandler::getMouseLocation()
{
	return mMousePos;
}

bool DemoHandler::mouseClicked(Button b)
{
	return mMouseClicked[b];
}

bool DemoHandler::isMouseDown(Button b)
{
	return mMouseState[b];
}

bool DemoHandler::keyTyped(char k)
{
	if(k < 0 || k > 255) return false;
	return mKeyStates[(int)k];
}

void DemoHandler::addMessage(const string& mess)
{
	if(mess.length() < 1) return;
	stringstream out;
	for(unsigned i = 0; i < mess.length(); i++)
	{
		if(mess[i] == '\n')
		{
			mTextLines.push_back(out.str());
			out.str("");
		} else out << mess[i];
	}
	mTextLines.push_back(out.str());
}


void DemoHandler::setMousePos(float x, float y, float z)
{
	if(mView != PERSPECTIVE)
	{
		mMousePos.mX = x;
		mMousePos.mY = y;
		mMousePos.mZ = z;
	}
}

void DemoHandler::setMouseClick(Button b)
{
	mMouseClicked[b] = true;
}


void DemoHandler::setMouseState(Button b, bool isDown)
{
	mMouseState[b] = isDown;
}

void DemoHandler::setKeyTyped(char k)
{
	if(k < 0 || k > 255) return;
	mKeyStates[(int)k] = true;
}

void DemoHandler::addDemo(Demo* d)
{
	addMessage(d->getInfo());
	mDemos.push_back(d);
	mActiveDemo = (int)mDemos.size() - 1;
}

int DemoHandler::getDemoCnt()
{
	return (int)mDemos.size();
}

const string DemoHandler::getName(int i)
{
	return mDemos[i]->getName();
}

void DemoHandler::setActive(int idx)
{
	mActiveDemo = idx;
	addMessage("\n\n\n\n\n");
	addMessage(mDemos[idx]->getInfo());
}

int DemoHandler::getActive()
{
	return mActiveDemo;
}


void DemoHandler::update()
{
	if(mGluQ == 0)
	{
		// We do it here due to some feature on mac that the glu context has to be active when we call gluNewQuadratic.
		mGluQ = gluNewQuadric();
		gluQuadricNormals(mGluQ, GLU_SMOOTH);
		gluQuadricOrientation(mGluQ, GLU_OUTSIDE);
	}
	//if(mView == PERSPECTIVE)
		glEnable(GL_LIGHTING);
	//else
	//	glDisable(GL_LIGHTING);

	if(mActiveDemo >= 0 && mActiveDemo < mDemos.size()) mDemos[mActiveDemo]->update(this);
	clearEvents();
}

void DemoHandler::printConsole(int width, int height)
{
	int startX = (width - 800) / 2;
	if(startX < 0) startX = 0;
	string title = getName(mActiveDemo);
	glDisable(GL_LIGHTING);	
	glDepthFunc(GL_ALWAYS);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	GLint viewport [4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluOrtho2D(0, viewport[2], viewport[3], 0);
	
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.7, 0.7, 0.7, 0.7);
	glBegin(GL_QUADS);
	glVertex2d(startX + 5, 100);
	glVertex2d(startX + 795, 100);
	glVertex2d(startX + 795, 5);
	glVertex2d(startX + 5, 5);
	glEnd();
	glLineWidth(1);
	glColor4f(1, 1, 1, 0.7);	
	glBegin(GL_LINES);
	glVertex2d(startX + 5, 100);
	glVertex2d(startX + 795, 100);
	glVertex2d(startX + 795, 100);
	glVertex2d(startX + 795, 5);
	glVertex2d(startX + 398 - title.length() * 9 / 2 - 5, 24);
	glVertex2d(startX + 398 + title.length() * 9 / 2 + 5, 24);
	glEnd();
	
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	
	
	glColor3f(1, 1, 0);	
	const int end = (int)mTextLines.size();
	int y = 40;
	const int maxChar = (width - 20) / 8;
	for(int i = std::max(0, end - 5); i < end; i++)
	{
		string text = mTextLines[i];
		glRasterPos2f(startX + 10, y);
		int cnt = 0;
		for(unsigned j = 0; j < text.length(); j++)
			if(cnt++ < maxChar) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[j]);
		y+= 14;
	}
	
	glRasterPos2f(startX + 398 - title.length() * 9 / 2, 20);
	for(unsigned i = 0; i < title.length(); i++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, title[i]);
	
	glDepthFunc(GL_LESS);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void DemoHandler::clearEvents()
{
	mMouseClicked[LEFT] = false;
	mMouseClicked[MIDDLE] = false;
	mMouseClicked[RIGHT] = false;
	for(int i = 0; i < 256; i++)
		mKeyStates[i] = false;
}
void DemoHandler::setView(View v)
{
	mView = v;
}

// Original work by Henrik Engström, henrik.engstrom@his.se (2009)
// Modified work by Peter Nilsson and Christian Larsson licensed under CC BY-SA 4.0