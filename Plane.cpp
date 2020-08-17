#include "Plane.h"
#include "Vec2d.h"
//#include<functional> // sort

bool Plane::OnUserCreate()
{
    _originX = olc::PixelGameEngine::ScreenWidth() / 2;
    _translX = _originX;
    _originY = olc::PixelGameEngine::ScreenHeight() / 2;
    _translY = _originY;

    _translX = -6500;
    _translY = -36000;
    _scale   = 0.5;

    int64_t border      = 0;
    _screenBoundaryXmin = border;
    _screenBoundaryXmax = (int64_t)olc::PixelGameEngine::GetDrawTargetWidth() - border;
    _screenBoundaryYmin = border;
    _screenBoundaryYmax = (int64_t)olc::PixelGameEngine::GetDrawTargetHeight() - border;

    // set up one white pixel for decal drawing a single color
    if(_gradientSprite == nullptr || _gradientDecal == nullptr)
    {
        delete _gradientDecal;
        delete _gradientSprite;

        _gradientSprite = new olc::Sprite(1, 1);
        _gradientSprite->SetPixel({ 0, 0 }, olc::WHITE);
        _gradientDecal = new olc::Decal(_gradientSprite);
    }

    return true;
}

void Plane::handleUserInput(double fElapsedTime)
{
    if(GetKey(olc::W).bHeld)
    { _translY += _flipY ? -fElapsedTime / _scale * _movementSpeed : fElapsedTime / _scale * _movementSpeed; }
    if(GetKey(olc::S).bHeld)
    { _translY -= _flipY ? -fElapsedTime / _scale * _movementSpeed : fElapsedTime / _scale * _movementSpeed; }
    if(GetKey(olc::A).bHeld)
    { _translX += _flipX ? -fElapsedTime / _scale * _movementSpeed : fElapsedTime / _scale * _movementSpeed; }
    if(GetKey(olc::D).bHeld)
    { _translX -= _flipX ? -fElapsedTime / _scale * _movementSpeed : fElapsedTime / _scale * _movementSpeed; }

    if(GetKey(olc::T).bHeld)
    {
        _scale += fElapsedTime * _scale;
        _movementSpeed += fElapsedTime * 100;
    }

    if(GetKey(olc::G).bHeld)
    {
        _scale -= fElapsedTime * _scale;
        _movementSpeed -= fElapsedTime * 100;
    }

    if(GetKey(olc::Z).bHeld) { _movementSpeed += fElapsedTime * 100; }
    if(GetKey(olc::H).bHeld) { _movementSpeed -= fElapsedTime * 100; }
    if(GetKey(olc::X).bPressed) { _flipX = !_flipX; }
    if(GetKey(olc::Y).bPressed) { _flipY = !_flipY; }
}

bool Plane::OnUserUpdate(float fElapsedTime)
{
    this->_totalElapsedTime += fElapsedTime;
    handleUserInput(fElapsedTime);

    Clear(olc::BLACK);
    /*	olc::PixelGameEngine::DrawRect(_screenBoundaryXmin, _screenBoundaryYmin, _screenBoundaryXmax - _screenBoundaryXmin, _screenBoundaryYmax - _screenBoundaryYmin, olc::GREY);

        double mx = GetMouseX();
        double my = GetMouseY();
        mx=428;
        my=34;
        std::cout << mx << " " << my << std::endl;
        mx = stocx((int32_t)mx);
        my = stocy((int32_t)my);
        double middleX = stocx((_screenBoundaryXmax + _screenBoundaryXmin) / 2);
        double middleY = stocy((_screenBoundaryYmax + _screenBoundaryYmin) / 2);
        FillTriangle(middleX - 10, middleY + 10, middleX + 10, middleY - 10, mx, my, olc::YELLOW);
        */

    return true;
}

double Plane::minX()
{
    return stocx(0); //(-_originX) / _scale + _originX - _translX;
}

double Plane::minY()
{
    return stocy(0); //(-_originY) / _scale + _originY - _translY;
}

double Plane::maxX()
{
    return stocx(
    olc::PixelGameEngine::
    GetDrawTargetWidth()); //(olc::PixelGameEngine::GetDrawTargetWidth() - _originX) / _scale + _originX - _translX;
}

double Plane::maxY()
{
    return stocy(
    olc::PixelGameEngine::
    GetDrawTargetHeight()); //(olc::PixelGameEngine::GetDrawTargetHeight() - _originY) / _scale + _originY - _translY;
}

// Draws a single olc::Pixel
bool Plane::Draw(double x, double y, olc::Pixel p) { return olc::PixelGameEngine::Draw(ctosx(x), ctosy(y), p); }

// Draws a line from (x1,y1) to (x2,y2)
void Plane::DrawLineScreen(int x1, int y1, int x2, int y2, olc::Pixel p, uint32_t pattern)
{
    //	return olc::PixelGameEngine::DrawLine(x1, y1, x2, y2, p, pattern);

    //TODO

    if(pattern == 0xFFFFFFFF)
    {
        DrawLineScreen(x1, y1, x2, y2, p);
        return;
    }

    int dx = x2 - x1;
    int dy = y2 - y1;

    // straight lines idea by gurkanctn
    if(dx == 0) // Line is vertical
    {
        if(y2 < y1) { std::swap(y1, y2); }

        // dont draw where not visible
        if(x1 != clampX(x1)) { return; }

        for(int y = (int32_t)clampY(y1); y <= (int32_t)clampY(y2); y++) { rol(x1, y, p, pattern); }
        return;
    }

    if(dy == 0) // Line is horizontal
    {
        if(x2 < x1) { std::swap(x1, x2); }

        // dont draw where not visible
        if(y2 != clampY(y1)) { return; }

        for(int x = (int32_t)clampX(x1); x <= (int32_t)clampX(x2); x++) { rol(x, y1, p, pattern); }
        return;
    }

    // Line is Funk-aye
    int dx1 = abs(dx);
    int dy1 = abs(dy);
    int px  = 2 * dy1 - dx1;
    int py  = 2 * dx1 - dy1;
    if(dy1 <= dx1)
    {
        int x  = 0;
        int y  = 0;
        int xe = 0;

        if(dx >= 0)
        {
            x  = x1;
            y  = y1;
            xe = x2;
        }
        else
        {
            x  = x2;
            y  = y2;
            xe = x1;
        }

        rol(x, y, p, pattern);

        for(/*x*/; x < xe; ++x)
        {
            if(px < 0) { px += 2 * dy1; }
            else
            {
                if((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) { ++y; }
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
        int x  = 0;
        int y  = 0;
        int ye = 0;

        if(dy >= 0)
        {
            x  = x1;
            y  = y1;
            ye = y2;
        }
        else
        {
            x  = x2;
            y  = y2;
            ye = y1;
        }

        rol(x, y, p, pattern);

        for(/*y*/; y < ye; ++y)
        {
            if(py <= 0) { py += 2 * dx1; }
            else
            {
                if((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) { ++x; }
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
void Plane::DrawLineScreen(int x1, int y1, int x2, int y2, olc::Pixel p)
{
    //	return olc::PixelGameEngine::DrawLine(x1, y1, x2, y2, p, pattern);

    //TODO

    int dx = x2 - x1;
    int dy = y2 - y1;

    // straight lines idea by gurkanctn
    if(dx == 0) // Line is vertical
    {
        if(y2 < y1) { std::swap(y1, y2); }

        // dont draw where not visible
        if(x1 != clampX(x1)) { return; }
        int32_t end = (int32_t)clampY(y2);
        for(int y = (int32_t)clampY(y1); y <= end; y++) { olc::PixelGameEngine::Draw(x1, y, p); }
        return;
    }

    if(dy == 0) // Line is horizontal
    {
        if(x2 < x1) { std::swap(x1, x2); }

        // dont draw where not visible
        if(y2 != clampY(y1)) { return; }

        int32_t end = (int32_t)clampX(x2);
        for(int x = (int32_t)clampX(x1); x <= end; x++) { olc::PixelGameEngine::Draw(x, y1, p); }
        return;
    }

    // Line is Funk-aye
    int dx1 = abs(dx);
    int dy1 = abs(dy);
    int px  = 2 * dy1 - dx1;
    int py  = 2 * dx1 - dy1;
    if(dy1 <= dx1)
    {
        int x  = 0;
        int y  = 0;
        int xe = 0;

        if(dx >= 0)
        {
            x  = x1;
            y  = y1;
            xe = x2;
        }
        else
        {
            x  = x2;
            y  = y2;
            xe = x1;
        }

        olc::PixelGameEngine::Draw(x, y, p);

        for(/*x*/; x < xe; ++x)
        {
            if(px < 0) { px += 2 * dy1; }
            else
            {
                if((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) { ++y; }
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
        int x  = 0;
        int y  = 0;
        int ye = 0;

        if(dy >= 0)
        {
            x  = x1;
            y  = y1;
            ye = y2;
        }
        else
        {
            x  = x2;
            y  = y2;
            ye = y1;
        }

        olc::PixelGameEngine::Draw(x, y, p);

        for(/*y*/; y < ye; ++y)
        {
            if(py <= 0) { py += 2 * dx1; }
            else
            {
                if((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) { ++x; }
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


// Draws a circle located at (x,y) with radius
void Plane::DrawCircle(double x, double y, double radius, olc::Pixel p, uint8_t mask)
{
    return olc::PixelGameEngine::DrawCircle(ctosx(x), ctosy(y), int32_t(radius * _scale), p, mask);
}

// Fills a circle located at (x,y) with radius
void Plane::FillCircle(double x, double y, double radius, olc::Pixel p)
{
    x = ctosx(x);
    y = ctosy(y);
    radius *= _scale;

    // Taken from wikipedia
    if(!radius) return;
    // if not visible at all, i dont care at all
    if(x + radius < _screenBoundaryXmin) return;
    if(x - radius > _screenBoundaryXmax) return;
    if(y + radius < _screenBoundaryYmin) return;
    if(y - radius > _screenBoundaryYmax) return;
    int x0 = 0;
    int y0 = (int32_t)radius;
    int d  = 3 - 2 * (int32_t)radius;

    while(y0 >= x0)
    {
        // Modified to draw scan-lines instead of edges
        // clamping is so that things outside of visible area are not drawn
        int ex = (int)clampX(x + x0);
        int ny = (int)clampY(y - y0);
        for(int i = (int32_t)clampX(x - x0); i <= ex; i++) olc::PixelGameEngine::Draw(i, ny, p);
        ex = (int)clampX(x + y0);
        ny = (int)clampY(y - x0);
        for(int i = (int32_t)clampX(x - y0); i <= ex; i++) olc::PixelGameEngine::Draw(i, ny, p);
        ex = (int)clampX(x + x0);
        ny = (int)clampY(y + y0);
        for(int i = (int32_t)clampX(x - x0); i <= ex; i++) olc::PixelGameEngine::Draw(i, ny, p);
        ex = (int)clampX(x + y0);
        ny = (int)clampY(y + x0);
        for(int i = (int32_t)clampX(x - y0); i <= ex; i++) olc::PixelGameEngine::Draw(i, ny, p);

        if(d < 0) d += 4 * x0++ + 6;
        else
            d += 4 * (x0++ - y0--) + 10;
    }
}

// Draws a rectangle at (x,y) to (x+w,y+h)
void Plane::DrawRect(double x, double y, double w, double h, olc::Pixel p)
{
    return olc::PixelGameEngine::DrawRect(ctosx(x), ctosy(y), int32_t(w * _scale), int32_t(h * _scale), p);
}

// Fills a rectangle at (x,y) to (x+w,y+h)
void Plane::FillRect(double x, double y, double w, double h, olc::Pixel p)
{
    return olc::PixelGameEngine::FillRect(
    ctosx(x), ctosy(y), int32_t(std::max(ceil(w * _scale), 1.)), int32_t(std::max(ceil(h * _scale), 1.)), p);
}

// Draws a triangle between points (x1,y1), (x2,y2) and (x3,y3)
void Plane::DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p)
{
    //	olc::PixelGameEngine::DrawTriangle(ctosx(x1), ctosy(y1), ctosx(x2), ctosy(y2), ctosx(x3), ctosy(y3), olc::RED);
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
    int32_t end  = (int)ceil(clampY(v2.y));
    for(int scanlineY = (int)floor(v1.y); scanlineY <= end; scanlineY++)
    {
        DrawLineScreen((int)round(curx1), scanlineY, (int)round(curx2), scanlineY, p /*olc::GREEN*/);
        curx1 += invslope1;
        curx2 += invslope2;
    }
    //olc::PixelGameEngine::Draw(v1.x,v1.y,olc::DARK_GREEN);
    //olc::PixelGameEngine::Draw(v2.x,v2.y,olc::DARK_GREEN);
    //olc::PixelGameEngine::Draw(v3.x,v3.y,olc::DARK_GREEN);
}

inline void Plane::fillTopFlatTriangle(Vec2d v1, Vec2d v2, Vec2d v3, olc::Pixel const& p)
{
    double invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    double invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

    double curx1 = v3.x;
    double curx2 = v3.x;
    int32_t end  = (int)floor(clampY(v1.y));
    for(int scanlineY = (int)ceil(v3.y); scanlineY >= end; scanlineY--)
    {
        DrawLineScreen((int)round(curx1), scanlineY, (int)round(curx2), scanlineY, p /*olc::YELLOW*/);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
    //olc::PixelGameEngine::Draw(v1.x,v1.y,olc::DARK_YELLOW);
    //olc::PixelGameEngine::Draw(v2.x,v2.y,olc::DARK_YELLOW);
    //olc::PixelGameEngine::Draw(v3.x,v3.y,olc::DARK_YELLOW);
}

void Plane::FillTriangleHomebrew(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p)
{
    Vec2d p1(x1 = ctosx(x1), y1 = ctosy(y1));
    Vec2d p2(x2 = ctosx(x2), y2 = ctosy(y2));
    Vec2d p3(x3 = ctosx(x3), y3 = ctosy(y3));

    //	olc::PixelGameEngine::FillTriangle(x1,y1,x2,y2,x3,y3, olc::RED);

    if(p1.y > p2.y) std::swap(p1, p2);
    if(p2.y > p3.y) std::swap(p2, p3);
    if(p1.y > p2.y) std::swap(p1, p2);

    // from: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

    /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
    // sortVerticesAscendingByY();

    /* here we know that v1.y <= v2.y <= v3.y */
    /* check for trivial case of bottom-flat triangle */
    if(p2.y == p3.y) { fillBottomFlatTriangle(p1, p2, p3, p); }
    /* check for trivial case of top-flat triangle */
    else if(p1.y == p2.y)
    {
        fillTopFlatTriangle(p1, p2, p3, p);
    }
    else
    {
        /* general case - split the triangle in a topflat and bottom-flat one */
        Vec2d p4((int)(p1.x + ((double)(p2.y - p1.y) / (double)(p3.y - p1.y)) * (p3.x - p1.x)), (int)p2.y);
        fillBottomFlatTriangle(p1, p2, p4, p);
        fillTopFlatTriangle(p2, p4, p3, p);
    }

    //olc::PixelGameEngine::Draw(x1,y1,olc::WHITE);
    //olc::PixelGameEngine::Draw(x2,y2,olc::WHITE);
    //olc::PixelGameEngine::Draw(x3,y3,olc::WHITE);
}

// Flat fills a triangle between points (x1,y1), (x2,y2) and (x3,y3)
void Plane::FillTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p)
{
    //olc::PixelGameEngine::FillTriangle(ctosx(x1), ctosy(y1), ctosx(x2), ctosy(y2), ctosx(x3), ctosy(y3), olc::RED);
    //FillTriangleHomebrew(x1, y1, x2, y2, x3, y3, p);
    //olc::PixelGameEngine::FillTriangle(ctosx(x1), ctosy(y1), ctosx(x2), ctosy(y2), ctosx(x3), ctosy(y3), olc::RED);
    FillTriangleHomebrew(x1, y1, x2, y2, x3, y3, p);
}

// Draws an entire olc::Sprite at location (x,y)
void Plane::DrawSprite(double x, double y, olc::Sprite* sprite, uint32_t scale)
{
    return olc::PixelGameEngine::DrawSprite(ctosx(x), ctosy(y), sprite, scale);
}

// Draws an area of a olc::Sprite at location (x,y), where the
// selected area is (ox,oy) to (ox+w,oy+h)

void Plane::DrawPartialSprite(
double x, double y, olc::Sprite* sprite, double ox, double oy, double w, double h, uint32_t scale)
{
    return olc::PixelGameEngine::DrawPartialSprite(ctosx(x),
                                                   ctosy(y),
                                                   sprite,
                                                   int32_t(ox * _scale),
                                                   int32_t(oy * _scale),
                                                   int32_t(w * _scale),
                                                   int32_t(h * _scale),
                                                   scale);
}

// Draws a single line of text
void Plane::DrawString(double x, double y, std::string sText, olc::Pixel col, uint32_t scale)
{
    //x = ctosx(x);
    //y = ctosy(y);
    //	return olc::PixelGameEngine::DrawString(ctosx(x), ctosy(y), sText, col, scale);

    int32_t sx = 0;
    int32_t sy = 0;
    if(col.ALPHA != 255) SetPixelMode(olc::Pixel::ALPHA);
    else
        SetPixelMode(olc::Pixel::MASK);
    for(auto c : sText)
    {
        if(c == '\n')
        {
            sx = 0;
            sy += 8 * scale;
        }
        else
        {
            int64_t ox = (c - 32) % 16;
            int64_t oy = (c - 32) / 16;

            if(scale > 1)
            {
                uint32_t is = 0;
                uint32_t js = 0;
                for(uint64_t i = 0; i < 8; i++)
                    for(uint64_t j = 0; j < 8; j++)
                        if(_fontSprite->GetPixel(uint32_t(i + ox * 8), uint32_t(j + oy * 8)).r > 0)
                        {
                            // todo fill gaps
                            DrawDecal(ctos({ float(x + sx + (i * scale) + is), float(y + sy + (j * scale) + js) }),
                                      _gradientDecal,
                                      { (float)(scale * _scale), (float)(scale * _scale) },
                                      col);
                            //FillRect(x + sx + (i * scale) + is, y + sy + (j * scale) + js, scale, scale, col);
                        }
            }
            else
            {
                for(uint32_t i = 0; i < 8; i++)
                    for(uint32_t j = 0; j < 8; j++)
                        if(_fontSprite->GetPixel(uint32_t(i + ox * 8), uint32_t(j + oy * 8)).r > 0)
                            FillRect(x + sx + i, y + sy + j, 1.01, 1.01, col);
            }
            sx += 8 * scale;
        }
    }
}

void Plane::DrawStringDecal(const olc::vf2d& pos,
                            const std::string& sText,
                            const olc::Pixel col,
                            const olc::vf2d& scale)
{
    olc::PixelGameEngine::DrawStringDecal(ctos(pos), sText, col, scale * _scale);
}

void Plane::DrawStringDecalMinScale(
const olc::vf2d& pos, const std::string& sText, const olc::Pixel col, const olc::vf2d& scale, double minScale /* = 1 */)
{
    olc::vf2d s = { std::max(scale.x * _scale, minScale), std::max(scale.y * _scale, minScale) };
    olc::PixelGameEngine::DrawStringDecal(ctos(pos), sText, col, s);
}
