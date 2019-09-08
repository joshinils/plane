#ifndef PLANE_H

#include "olcPixelGameEngine/olcPixelGameEngine.h"

class Plane
	: public olc::PixelGameEngine
{
private:
	double _originX = 0;
	double _originY = 0;
	double _translX = 0;
	double _translY = 0;
	double _scale = 10;

	double _movementSpeed = 20;

public:
	Plane() = default;
	~Plane() = default;

	virtual bool OnUserCreate()
	{
		std::cout << __FUNCTION__ << std::endl;
		_originX = olc::PixelGameEngine::ScreenWidth()/2;
		_translX = _originX;
		_originY = olc::PixelGameEngine::ScreenHeight()/2;
		_translY = _originY;
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		std::cout << __FUNCTION__ << std::endl;

		if(GetKey(olc::W).bHeld)
		{
			_translY += fElapsedTime * _scale * _movementSpeed;
		}

		if(GetKey(olc::S).bHeld)
		{
			_translY -= fElapsedTime * _scale * _movementSpeed;
		}

		if(GetKey(olc::A).bHeld)
		{
			_translX += fElapsedTime * _scale * _movementSpeed;
		}

		if(GetKey(olc::D).bHeld)
		{
			_translX -= fElapsedTime * _scale * _movementSpeed;
		}

		if(GetKey(olc::T).bHeld)
		{
			_scale += fElapsedTime * _scale;
			_movementSpeed += fElapsedTime;
		}

		if(GetKey(olc::G).bHeld)
		{
			_scale -= fElapsedTime * _scale;
			_movementSpeed -= fElapsedTime;
		}


		std::cout << "_translX: " << _translX << "	_translY: " << _translY << std::endl;
		return true;
	}

	//coordinate to screen x
	double ctosx(double x)
	{
		return (x + _translX - _originX) * _scale + _originX;
	};

	//coordinate to screen y
	double ctosy(double y)
	{
		return (y + _translY - _originY) * _scale + _originY;
	};


	double minX()
	{
		// TODO: fix
		return (-_originX)/_scale +_originX -_translX;
	};

	double minY()
	{
		// TODO: fix
		return (-_originY)/_scale +_originY -_translY;
	};

	double maxX()
	{
		// TODO: fix
		return (olc::PixelGameEngine::GetDrawTargetWidth() -_originX)/_scale +_originX -_translX;
	};

	double maxY()
	{
		// TODO: fix
		return (olc::PixelGameEngine::GetDrawTargetHeight()-_originY)/_scale +_originY -_translY;
	};

public:
	// Draws a single olc::Pixel
	bool Draw(double x, double y, olc::Pixel p = olc::WHITE)
	{
		return olc::PixelGameEngine::Draw(ctosx(x), ctosy(y), p);
	}

	// Draws a line from (x1,y1) to (x2,y2)
	void DrawLine(double x1, double y1, double x2, double y2, olc::Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF)
	{
		return olc::PixelGameEngine::DrawLine(x1, y1, x2, y2, p, pattern);
	}

	// Draws a circle located at (x,y) with radius
	void DrawCircle(double x, double y, double radius, olc::Pixel p = olc::WHITE, uint8_t mask = 0xFF)
	{
		return olc::PixelGameEngine::DrawCircle(x, y, radius, p, mask);
	}

	// Fills a circle located at (x,y) with radius
	void FillCircle(double x, double y, double radius, olc::Pixel p = olc::WHITE)
	{
		return olc::PixelGameEngine::FillCircle(x, y, radius, p);
	}

	// Draws a rectangle at (x,y) to (x+w,y+h)
	void DrawRect(double x, double y, double w, double h, olc::Pixel p = olc::WHITE)
	{
		return olc::PixelGameEngine::DrawRect(x, y, w, h, p);
	}

	// Fills a rectangle at (x,y) to (x+w,y+h)
	void FillRect(double x, double y, double w, double h, olc::Pixel p = olc::WHITE)
	{
		return olc::PixelGameEngine::FillRect(x, y, w, h, p);
	}

	// Draws a triangle between points (x1,y1), (x2,y2) and (x3,y3)
	void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p = olc::WHITE)
	{
		return olc::PixelGameEngine::DrawTriangle(x1, y1, x2, y2, x3, y3, p);
	}

	// Flat fills a triangle between points (x1,y1), (x2,y2) and (x3,y3)
	void FillTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p = olc::WHITE)
	{
		return olc::PixelGameEngine::FillTriangle(x1, y1, x2, y2, x3, y3, p);
	}

	// Draws an entire olc::Sprite at location (x,y)
	void DrawSprite(double x, double y, olc::Sprite* sprite, uint32_t scale = 1)
	{
		return olc::PixelGameEngine::DrawSprite(x, y, sprite, scale);
	}

	// Draws an area of a olc::Sprite at location (x,y), where the
	// selected area is (ox,oy) to (ox+w,oy+h)
	void DrawPartialSprite(double x, double y, olc::Sprite* sprite, double ox, double oy, double w, double h, uint32_t scale = 1)
	{
		return olc::PixelGameEngine::DrawPartialSprite(x, y, sprite, ox, oy, w, h, scale);
	}

	// Draws a single line of text
	void DrawString(double x, double y, std::string sText, olc::Pixel col = olc::WHITE, uint32_t scale = 1)
	{
		return olc::PixelGameEngine::DrawString(x, y, sText, col, scale);
	}

};

#endif // PLANE_H
