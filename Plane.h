#ifndef PLANE_H
#define PLANE_H

#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include "Vec2d.h"

class Plane
	: public olc::PixelGameEngine
{
private:
	double _originX = 0;
	double _originY = 0;
	double _translX = 0;
	double _translY = 0;
	double _scale = 10;

	int _screenBoundaryXmin;
	int _screenBoundaryXmax;
	int _screenBoundaryYmin;
	int _screenBoundaryYmax;
	double _movementSpeed = 100;

public:
	Plane() = default;
	~Plane() = default;

	virtual bool OnUserCreate();
	void handleUserInput(double fElapsedTime);
	virtual bool OnUserUpdate(float fElapsedTime);

	//coordinate to screen x
	inline int32_t ctosx(double x);

	// screen to coordinate x
	inline double stocx(int32_t x);

	//coordinate to screen y
	inline int32_t ctosy(double y);

	// screen to coordinate y
	inline double stocy(int32_t y);

	// clamps x to visible pixels
	inline double clampX(double x);

	// clamps y to visible pixels
	inline double clampY(double y);

	double minX();
	double minY();
	double maxX();
	double maxY();

public:
	// Draws a single olc::Pixel
	inline bool Draw(double x, double y, olc::Pixel p = olc::WHITE);

	inline bool rol(int32_t const& x, int32_t const& y, olc::Pixel const& p, uint32_t& pattern);;

	// Draws a line from (x1,y1) to (x2,y2)
	inline void DrawLine(double x1, double y1, double x2, double y2, olc::Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF);
	inline void DrawLineScreen(int x1, int y1, int x2, int y2, olc::Pixel p, uint32_t pattern);
	inline void DrawLineScreen(int x1, int y1, int x2, int y2, olc::Pixel p = olc::WHITE);

	// Draws a circle located at (x,y) with radius
	void DrawCircle(double x, double y, double radius, olc::Pixel p = olc::WHITE, uint8_t mask = 0xFF);

	// Fills a circle located at (x,y) with radius
	void FillCircle(double x, double y, double radius, olc::Pixel p = olc::WHITE);

	// Draws a rectangle at (x,y) to (x+w,y+h)
	void DrawRect(double x, double y, double w, double h, olc::Pixel p = olc::WHITE);

	// Fills a rectangle at (x,y) to (x+w,y+h)
	void FillRect(double x, double y, double w, double h, olc::Pixel p = olc::WHITE);

	// Draws a triangle between points (x1,y1), (x2,y2) and (x3,y3)
	void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p = olc::WHITE);

	// Flat fills a triangle between points (x1,y1), (x2,y2) and (x3,y3)
	void FillTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p = olc::WHITE);
	void FillTriangleHomebrew(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p = olc::WHITE);
	void fillBottomFlatTriangle(Vec2d v1, Vec2d v2, Vec2d v3, olc::Pixel const& p);
	void fillTopFlatTriangle(Vec2d v1, Vec2d v2, Vec2d v3, olc::Pixel const& p);

	// Draws an entire olc::Sprite at location (x,y)
	void DrawSprite(double x, double y, olc::Sprite* sprite, uint32_t scale = 1);

	// Draws an area of a olc::Sprite at location (x,y), where the
	// selected area is (ox,oy) to (ox+w,oy+h)
	void DrawPartialSprite(double x, double y, olc::Sprite* sprite, double ox, double oy, double w, double h, uint32_t scale = 1);

	// Draws a single line of text
	void DrawString(double x, double y, std::string sText, olc::Pixel col = olc::WHITE, uint32_t scale = 1);

};

#endif // PLANE_H
