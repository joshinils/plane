#include "Plane.h"
#include "Vec2d.h"
//´#include<functional> // sort

bool Plane::OnUserCreate()
{
	_originX = olc::PixelGameEngine::ScreenWidth() / 2;
	_translX = _originX;
	_originY = olc::PixelGameEngine::ScreenHeight() / 2;
	_translY = _originY;

	int border = 00;
	_screenBoundaryXmin = border;
	_screenBoundaryXmax = olc::PixelGameEngine::GetDrawTargetWidth() - border;
	_screenBoundaryYmin = border;
	_screenBoundaryYmax = olc::PixelGameEngine::GetDrawTargetHeight() - border;

	return true;
}

void Plane::handleUserInput(double fElapsedTime)
{
	if (GetKey(olc::W).bHeld)
	{
		_translY += fElapsedTime / _scale * _movementSpeed;
	}

	if (GetKey(olc::S).bHeld)
	{
		_translY -= fElapsedTime / _scale * _movementSpeed;
	}

	if (GetKey(olc::A).bHeld)
	{
		_translX += fElapsedTime / _scale * _movementSpeed;
	}

	if (GetKey(olc::D).bHeld)
	{
		_translX -= fElapsedTime / _scale * _movementSpeed;
	}

	if (GetKey(olc::T).bHeld)
	{
		_scale += fElapsedTime * _scale;
		_movementSpeed += fElapsedTime;
	}

	if (GetKey(olc::G).bHeld)
	{
		_scale -= fElapsedTime * _scale;
		_movementSpeed -= fElapsedTime;
	}
}

bool Plane::OnUserUpdate(float fElapsedTime)
{
	handleUserInput(fElapsedTime);

	Clear(olc::BLACK);
	olc::PixelGameEngine::DrawRect(_screenBoundaryXmin, _screenBoundaryYmin, _screenBoundaryXmax - _screenBoundaryXmin, _screenBoundaryYmax - _screenBoundaryYmin, olc::GREY);

	double mx = GetMouseX();
	double my = GetMouseY();
	mx = stocx((int32_t)mx);
	my = stocy((int32_t)my);
	double middleX = stocx((_screenBoundaryXmax + _screenBoundaryXmin) / 2);
	double middleY = stocy((_screenBoundaryYmax + _screenBoundaryYmin) / 2);
	FillTriangle(middleX - 10, middleY + 10, middleX + 10, middleY - 10, mx, my, olc::YELLOW);

	return true;
}

//coordinate to screen x
inline int32_t Plane::ctosx(double x)
{
	return (int32_t)round((x + _translX - _originX) * _scale + _originX);
}

// screen to coordinate x
inline double Plane::stocx(int32_t x)
{
	return (x - _originX) / _scale + _originX - _translX;
}

//coordinate to screen y
inline int32_t Plane::ctosy(double y)
{
	return (int32_t)round((y + _translY - _originY) * _scale + _originY);
}

// screen to coordinate y
inline double Plane::stocy(int32_t y)
{
	return (y - _originY) / _scale + _originY - _translY;
}

// clamps x to visible pixels
inline double Plane::clampX(double x)
{
	if (x > _screenBoundaryXmax)
		return _screenBoundaryXmax;
	if (x < _screenBoundaryXmin - 1)
		return _screenBoundaryXmin - 1;
	return x;
}

// clamps y to visible pixels
inline double Plane::clampY(double y)
{
	if (y > _screenBoundaryYmax)
		return _screenBoundaryYmax;
	if (y < _screenBoundaryYmin - 1)
		return _screenBoundaryYmin - 1;
	return y;
}

double Plane::minX()
{
	return stocx(0);//(-_originX) / _scale + _originX - _translX;
}

double Plane::minY()
{
	return stocy(0);//(-_originY) / _scale + _originY - _translY;
}

double Plane::maxX()
{
	return stocx(olc::PixelGameEngine::GetDrawTargetWidth());//(olc::PixelGameEngine::GetDrawTargetWidth() - _originX) / _scale + _originX - _translX;
}

double Plane::maxY()
{
	return stocy(olc::PixelGameEngine::GetDrawTargetHeight());//(olc::PixelGameEngine::GetDrawTargetHeight() - _originY) / _scale + _originY - _translY;
}

// Draws a single olc::Pixel
bool Plane::Draw(double x, double y, olc::Pixel p)
{
	return olc::PixelGameEngine::Draw(ctosx(x), ctosy(y), p);
}

inline bool Plane::rol(int32_t const & x, int32_t const & y, olc::Pixel const & p, uint32_t & pattern)
{
	pattern = (pattern << 1) | (pattern >> 31);
	if (pattern & 1)
	{
		olc::PixelGameEngine::Draw(x, y, p);
		return true;
	}
	return false;
}

// Draws a line from (x1,y1) to (x2,y2)
inline void Plane::DrawLineScreen(int x1, int y1, int x2, int y2, olc::Pixel p, uint32_t pattern)
{
	//	return olc::PixelGameEngine::DrawLine(x1, y1, x2, y2, p, pattern);

	//TODO

	if (pattern == 0xFFFFFFFF)
	{
		DrawLineScreen(x1, y1, x2, y2, p);
		return;
	}

	int dx = x2 - x1;
	int dy = y2 - y1;

	// straight lines idea by gurkanctn
	if (dx == 0) // Line is vertical
	{
		if (y2 < y1)
		{
			std::swap(y1, y2);
		}

		// dont draw where not visible
		if (x1 != clampX(x1))
		{
			return;
		}

		for (int y = (int32_t)clampY(y1); y <= (int32_t)clampY(y2); y++)
		{
			rol(x1, y, p, pattern);
		}
		return;
	}

	if (dy == 0) // Line is horizontal
	{
		if (x2 < x1)
		{
			std::swap(x1, x2);
		}

		// dont draw where not visible
		if (y2 != clampY(y1))
		{
			return;
		}

		for (int x = (int32_t)clampX(x1); x <= (int32_t)clampX(x2); x++)
		{
			rol(x, y1, p, pattern);
		}
		return;
	}

	// Line is Funk-aye
	int dx1 = abs(dx);
	int dy1 = abs(dy);
	int px = 2 * dy1 - dx1;
	int py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		int x = 0;
		int y = 0;
		int xe = 0;

		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}

		rol(x, y, p, pattern);

		for (x; x < xe; ++x)
		{
			if (px < 0)
			{
				px += 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					++y;
				}
				else
				{
					--y;
				}
				px += 2 * (dy1 - dx1);
			}
			rol(x, y, p, pattern);
		}
	}
	else
	{
		int x = 0;
		int y = 0;
		int ye = 0;

		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}

		rol(x, y, p, pattern);

		for (y; y < ye; ++y)
		{
			if (py <= 0)
			{
				py += 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					++x;
				}
				else
				{
					--x;
				}
				py += 2 * (dx1 - dy1);
			}

			rol(x, y, p, pattern);
		}
	}
}

// Draws a line from (x1,y1) to (x2,y2) without fiddling with patterns
inline void Plane::DrawLineScreen(int x1, int y1, int x2, int y2, olc::Pixel p)
{
	//	return olc::PixelGameEngine::DrawLine(x1, y1, x2, y2, p, pattern);

	//TODO

	int dx = x2 - x1;
	int dy = y2 - y1;

	// straight lines idea by gurkanctn
	if (dx == 0) // Line is vertical
	{
		if (y2 < y1)
		{
			std::swap(y1, y2);
		}

		// dont draw where not visible
		if (x1 != clampX(x1))
		{
			return;
		}
		int32_t end = (int32_t)clampY(y2);
		for (int y = (int32_t)clampY(y1); y <= end; y++)
		{
			olc::PixelGameEngine::Draw(x1, y, p);
		}
		return;
	}

	if (dy == 0) // Line is horizontal
	{
		if (x2 < x1)
		{
			std::swap(x1, x2);
		}

		// dont draw where not visible
		if (y2 != clampY(y1))
		{
			return;
		}

		int32_t end = (int32_t)clampX(x2);
		for (int x = (int32_t)clampX(x1); x <= end; x++)
		{
			olc::PixelGameEngine::Draw(x, y1, p);
		}
		return;
	}

	// Line is Funk-aye
	int dx1 = abs(dx);
	int dy1 = abs(dy);
	int px = 2 * dy1 - dx1;
	int py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		int x = 0;
		int y = 0;
		int xe = 0;

		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}

		olc::PixelGameEngine::Draw(x, y, p);

		for (x; x < xe; ++x)
		{
			if (px < 0)
			{
				px += 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					++y;
				}
				else
				{
					--y;
				}
				px += 2 * (dy1 - dx1);
			}
			olc::PixelGameEngine::Draw(x, y, p);
		}
	}
	else
	{
		int x = 0;
		int y = 0;
		int ye = 0;

		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}

		olc::PixelGameEngine::Draw(x, y, p);

		for (y; y < ye; ++y)
		{
			if (py <= 0)
			{
				py += 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					++x;
				}
				else
				{
					--x;
				}
				py += 2 * (dx1 - dy1);
			}

			olc::PixelGameEngine::Draw(x, y, p);
		}
	}
}

// Draws a line from (x1,y1) to (x2,y2)
void Plane::DrawLine(double X1, double Y1, double X2, double Y2, olc::Pixel p, uint32_t pattern)
{
	int x1 = ctosx(X1);
	int y1 = ctosy(Y1);
	int x2 = ctosx(X2);
	int y2 = ctosy(Y2);
	DrawLineScreen(ctosx(X1), ctosx(Y1), ctosx(X2), ctosx(Y2), pattern, pattern);
}

// Draws a circle located at (x,y) with radius
void Plane::DrawCircle(double x, double y, double radius, olc::Pixel p, uint8_t mask)
{
	return olc::PixelGameEngine::DrawCircle(ctosx(x), ctosy(y), int32_t(radius*_scale), p, mask);
}

// Fills a circle located at (x,y) with radius
void Plane::FillCircle(double x, double y, double radius, olc::Pixel p)
{
	x = ctosx(x);
	y = ctosy(y);
	radius *= _scale;

	// Taken from wikipedia
	if (!radius) return;
	// if not visible at all, i dont care at all
	if (x + radius < _screenBoundaryXmin) return;
	if (x - radius > _screenBoundaryXmax) return;
	if (y + radius < _screenBoundaryYmin) return;
	if (y - radius > _screenBoundaryYmax) return;
	int x0 = 0;
	int y0 = (int32_t)radius;
	int d = 3 - 2 * (int32_t)radius;

	while (y0 >= x0)
	{
		// Modified to draw scan-lines instead of edges
		// clamping is so that things outside of visible area are not drawn
		int ex = (int)clampX(x + x0);
		int ny = (int)clampY(y - y0);
		for (int i = (int32_t)clampX(x - x0); i <= ex; i++)
			olc::PixelGameEngine::Draw(i, ny, olc::CYAN);
		ex = (int)clampX(x + y0);
		ny = (int)clampY(y - x0);
		for (int i = (int32_t)clampX(x - y0); i <= ex; i++)
			olc::PixelGameEngine::Draw(i, ny, olc::YELLOW);
		ex = (int)clampX(x + x0);
		ny = (int)clampY(y + y0);
		for (int i = (int32_t)clampX(x - x0); i <= ex; i++)
			olc::PixelGameEngine::Draw(i, ny, olc::BLUE);
		ex = (int)clampX(x + y0);
		ny = (int)clampY(y + x0);
		for (int i = (int32_t)clampX(x - y0); i <= ex; i++)
			olc::PixelGameEngine::Draw(i, ny, olc::GREEN);

		if (d < 0) d += 4 * x0++ + 6;
		else d += 4 * (x0++ - y0--) + 10;
	}
}

// Draws a rectangle at (x,y) to (x+w,y+h)
void Plane::DrawRect(double x, double y, double w, double h, olc::Pixel p)
{
	return olc::PixelGameEngine::DrawRect(ctosx(x), ctosy(y), int32_t(w*_scale), int32_t(h*_scale), p);
}

// Fills a rectangle at (x,y) to (x+w,y+h)
void Plane::FillRect(double x, double y, double w, double h, olc::Pixel p)
{
	return olc::PixelGameEngine::FillRect(ctosx(x), ctosy(y), int32_t(w*_scale), int32_t(h*_scale), p);
}

// Draws a triangle between points (x1,y1), (x2,y2) and (x3,y3)
void Plane::DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p)
{
	DrawLine(x1, y1, x2, y2, p);
	DrawLine(x2, y2, x3, y3, p);
	DrawLine(x3, y3, x1, y1, p);
}

inline void Plane::fillBottomFlatTriangle(Vec2d v1, Vec2d v2, Vec2d v3, olc::Pixel const& p)
{
	double invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
	double invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

	double curx1 = v1.x;
	double curx2 = v1.x;
	int32_t end = (int32_t)clampY(v2.y);
	for (int scanlineY = (int32_t)v1.y; scanlineY <= end; scanlineY++)
	{
		DrawLineScreen((int)curx1, scanlineY, (int)curx2, scanlineY, p);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

inline void Plane::fillTopFlatTriangle(Vec2d v1, Vec2d v2, Vec2d v3, olc::Pixel const& p)
{
	double invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
	double invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

	double curx1 = v3.x;
	double curx2 = v3.x;
	int32_t end = (int32_t)clampY(v1.y);
	for (int scanlineY = (int32_t)v3.y; scanlineY > end; scanlineY--)
	{
		DrawLineScreen((int)curx1, scanlineY, (int)curx2, scanlineY, p);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void Plane::FillTriangleHomebrew(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p)
{
	//TODO
	Vec2d p1(ctosx(x1), ctosy(y1));
	Vec2d p2(ctosx(x2), ctosy(y2));
	Vec2d p3(ctosx(x3), ctosy(y3));

	std::vector<Vec2d> sortedY;
	sortedY.push_back(p1);
	sortedY.push_back(p2);
	sortedY.push_back(p3);
	std::sort(sortedY.begin(), sortedY.end(), [](const Vec2d& a, const Vec2d& b) { return a.y < b.y; });
	p1 = sortedY[0];
	p2 = sortedY[1];
	p3 = sortedY[2];

	// from: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

	/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
	// sortVerticesAscendingByY();

	/* here we know that v1.y <= v2.y <= v3.y */
	/* check for trivial case of bottom-flat triangle */
	if (p2.y == p3.y)
	{
		fillBottomFlatTriangle(p1, p2, p3, p);
	}
	/* check for trivial case of top-flat triangle */
	else if (p1.y == p2.y)
	{
		fillTopFlatTriangle(p1, p2, p3, p);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vec2d p4((int)(p1.x + ((double)(p2.y - p1.y) / (double)(p3.y - p1.y)) * (p3.x - p1.x)), p2.y);
		fillBottomFlatTriangle(p1, p2, p4, p);
		fillTopFlatTriangle(p2, p4, p3, p);
	}
}

// Flat fills a triangle between points (x1,y1), (x2,y2) and (x3,y3)
void Plane::FillTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p)
{
	olc::PixelGameEngine::FillTriangle(ctosx(x1), ctosy(y1), ctosx(x2), ctosy(y2), ctosx(x3), ctosy(y3), olc::RED);
	FillTriangleHomebrew(x1, y1, x2, y2, x3, y3, p);
	olc::PixelGameEngine::FillTriangle(ctosx(x1), ctosy(y1), ctosx(x2), ctosy(y2), ctosx(x3), ctosy(y3), olc::RED);
	FillTriangleHomebrew(x1, y1, x2, y2, x3, y3, p);
}

// Draws an entire olc::Sprite at location (x,y)
void Plane::DrawSprite(double x, double y, olc::Sprite * sprite, uint32_t scale)
{
	return olc::PixelGameEngine::DrawSprite(ctosx(x), ctosy(y), sprite, scale);
}

// Draws an area of a olc::Sprite at location (x,y), where the
// selected area is (ox,oy) to (ox+w,oy+h)

void Plane::DrawPartialSprite(double x, double y, olc::Sprite * sprite, double ox, double oy, double w, double h, uint32_t scale)
{
	return olc::PixelGameEngine::DrawPartialSprite(ctosx(x), ctosy(y), sprite, int32_t(ox*_scale), int32_t(oy*_scale), int32_t(w*_scale), int32_t(h*_scale), scale);
}

// Draws a single line of text
void Plane::DrawString(double x, double y, std::string sText, olc::Pixel col, uint32_t scale)
{
	return olc::PixelGameEngine::DrawString(ctosx(x), ctosy(y), sText, col, scale);
}
