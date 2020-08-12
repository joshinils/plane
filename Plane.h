#ifndef PLANE_H
#define PLANE_H

#include "Vec2d.h"
#include "olcPixelGameEngine/olcPixelGameEngine.h"

class Plane : public olc::PixelGameEngine
{
private:
    double _originX = 0;
    double _originY = 0;
    double _translX = 0;
    double _translY = 0;
    double _scale   = 57.03;

    int64_t _screenBoundaryXmin = 0;
    int64_t _screenBoundaryXmax = INT64_MAX;
    int64_t _screenBoundaryYmin = 0;
    int64_t _screenBoundaryYmax = INT64_MAX;
    double _movementSpeed       = 100;

private:
    olc::Sprite* _fontSprite;

public:
    Plane()
    {
        std::string data;
        data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
        data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
        data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
        data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
        data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
        data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
        data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
        data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
        data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
        data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
        data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
        data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
        data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
        data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
        data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
        data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

        _fontSprite = new olc::Sprite(128, 48);
        int px = 0, py = 0;
        for(size_t b = 0; b < 1024; b += 4)
        {
            uint32_t sym1 = (uint32_t)data[b + 0] - 48;
            uint32_t sym2 = (uint32_t)data[b + 1] - 48;
            uint32_t sym3 = (uint32_t)data[b + 2] - 48;
            uint32_t sym4 = (uint32_t)data[b + 3] - 48;
            uint32_t r    = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

            for(int i = 0; i < 24; i++)
            {
                int k = r & (1 << i) ? 255 : 0;
                _fontSprite->SetPixel(px, py, olc::Pixel(k, k, k, k));
                if(++py == 48)
                {
                    px++;
                    py = 0;
                }
            }
        }
    }

    ~Plane() { delete _fontSprite; }

    virtual bool OnUserCreate();
    void handleUserInput(double fElapsedTime);
    virtual bool OnUserUpdate(float fElapsedTime);

    //coordinate to screen x
    inline int32_t ctosx(double x) { return (int32_t)round((x + _translX - _originX) * _scale + _originX); }

    // screen to coordinate x
    inline double stocx(int32_t x) { return (x - _originX) / _scale + _originX - _translX; }

    //coordinate to screen y
    inline int32_t ctosy(double y) { return (int32_t)round((y + _translY - _originY) * _scale + _originY); }

    // screen to coordinate y
    inline double stocy(int32_t y) { return (y - _originY) / _scale + _originY - _translY; }

    // clamps x to visible pixels
    inline double clampX(double x)
    {
        if(x > _screenBoundaryXmax) return double(_screenBoundaryXmax);
        if(x < _screenBoundaryXmin - 1) return double(_screenBoundaryXmin - 1);
        return x;
    }

    // clamps y to visible pixels
    inline double clampY(double y)
    {
        if(y > _screenBoundaryYmax) return double(_screenBoundaryYmax);
        if(y < _screenBoundaryYmin - 1) return double(_screenBoundaryYmin - 1);
        return y;
    }

    double minX();
    double minY();
    double maxX();
    double maxY();

public:
    // Draws a single olc::Pixel
    /*inline */ bool Draw(double x, double y, olc::Pixel p = olc::WHITE);

    inline bool rol(int32_t const& x, int32_t const& y, olc::Pixel const& p, uint32_t& pattern)
    {
        pattern = (pattern << 1) | (pattern >> 31);
        if(pattern & 1)
        {
            olc::PixelGameEngine::Draw(x, y, p);
            return true;
        }
        return false;
    }

    // Draws a line from (x1,y1) to (x2,y2)
    inline void DrawLine(olc::vf2d start, olc::vf2d end, olc::Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF)
    {
        DrawLine(start.x, start.y, end.x, end.y, p, pattern);
    }
    // Draws a line from (x1,y1) to (x2,y2)
    inline void
    DrawLine(double X1, double Y1, double X2, double Y2, olc::Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF)
    {
        DrawLineScreen(ctosx(X1), ctosy(Y1), ctosx(X2), ctosy(Y2), p, pattern);
    }
    void DrawLineScreen(int x1, int y1, int x2, int y2, olc::Pixel p, uint32_t pattern);
    void DrawLineScreen(int x1, int y1, int x2, int y2, olc::Pixel p = olc::WHITE);

    // Draws a circle located at (x,y) with radius
    void DrawCircle(double x, double y, double radius, olc::Pixel p = olc::WHITE, uint8_t mask = 0xFF);

    // Fills a circle located at (x,y) with radius
    void FillCircle(olc::vf2d center, double radius, olc::Pixel p = olc::WHITE)
    {
        FillCircle(center.x, center.y, radius, p);
    }
    void FillCircle(double x, double y, double radius, olc::Pixel p = olc::WHITE);

    // Draws a rectangle at (x,y) to (x+w,y+h)
    void DrawRect(double x, double y, double w, double h, olc::Pixel p = olc::WHITE);

    // Fills a rectangle at (x,y) to (x+w,y+h)
    void FillRect(double x, double y, double w, double h, olc::Pixel p = olc::WHITE);

    // Draws a triangle between points (x1,y1), (x2,y2) and (x3,y3)
    void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p = olc::WHITE);

    // Flat fills a triangle between points (x1,y1), (x2,y2) and (x3,y3)
    void FillTriangle(olc::vf2d p0, olc::vf2d p1, olc::vf2d p2, olc::Pixel p = olc::WHITE)
    {
        FillTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p);
    }
    void FillTriangle(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p = olc::WHITE);
    void
    FillTriangleHomebrew(double x1, double y1, double x2, double y2, double x3, double y3, olc::Pixel p = olc::WHITE);
    void fillBottomFlatTriangle(Vec2d v1, Vec2d v2, Vec2d v3, olc::Pixel const& p);
    void fillTopFlatTriangle(Vec2d v1, Vec2d v2, Vec2d v3, olc::Pixel const& p);

    // Draws an entire olc::Sprite at location (x,y)
    void DrawSprite(double x, double y, olc::Sprite* sprite, uint32_t scale = 1);

    // Draws an area of a olc::Sprite at location (x,y), where the
    // selected area is (ox,oy) to (ox+w,oy+h)
    void DrawPartialSprite(
    double x, double y, olc::Sprite* sprite, double ox, double oy, double w, double h, uint32_t scale = 1);

    // Draws a single line of text
    void DrawString(double x, double y, std::string sText, olc::Pixel col = olc::WHITE, uint32_t scale = 1);
};

#endif // PLANE_H
