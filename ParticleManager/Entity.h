#pragma once
#include "DemoHandler.h"

namespace pc
{
	class Entity
	{
	protected:
		// Draw color
		Color mColor;
	public:
		virtual ~Entity() {}
		virtual void update(float dt){}
		virtual void draw(DemoHandler* draw, const bool drawDebug = false){}
		Color getColor() { return mColor; } const
		void setColor(Color color) { mColor = color; }
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0