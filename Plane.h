#ifndef PLANE_H

#include "olcPixelGameEngine/olcPixelGameEngine.h"

class Plane
	: public olc::PixelGameEngine
{
public:
	Plane() = default;
	~Plane() = default;

	virtual bool OnUserCreate()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}

private:

};

#endif // PLANE_H
