#pragma once

struct AbstractDrawable
{
	virtual ~AbstractDrawable() = default;
	virtual void draw() = 0;
};