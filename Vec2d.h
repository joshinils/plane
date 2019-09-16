#ifndef VEC2D_H
#define VEC2D_H

class Vec2d
{
public:
	double x = 0;
	double y = 0;

	Vec2d(double x_ = 0, double y_ = 0)
		: x(x_)
		, y(y_)
	{ }

	inline Vec2d const& normalize()
	{
		double l = length();

		if(l!=0)
		{
			x /= l;
			y /= l;
		}
		return *this;
	}

	inline double angleTo(Vec2d const& other) const
	{
		double cosTheta = (double)(operator* (other) / (this->length() * other.length()));

		return acos(cosTheta);
	}

	inline double length() const
	{
		return sqrt(x*x + y*y);
	}

	// dot product
	inline double operator* (Vec2d const& v) const
	{
		return x * v.x + y * v.y;
	}

	// scaling
	inline Vec2d operator* (double s) const
	{
		Vec2d res = *this;
		return res *= s;
	}

	// other order of arguments, double first
	friend inline Vec2d operator* (double s, Vec2d const& v)
	{
		return v * s;
	}

	inline Vec2d const& operator*= (double s)
	{
		x *= s;
		y *= s;

		return *this;
	}

	inline Vec2d operator/ (double s) const
	{
		Vec2d res = *this;
		return res /= s;
	}

	inline Vec2d const& operator/= (double s)
	{
		x /= s;
		y /= s;

		return *this;
	}

	inline Vec2d operator+ (Vec2d const& v) const
	{
		Vec2d res = *this;
		return res += v;
	}

	inline Vec2d const& operator+= (Vec2d const& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	inline Vec2d operator- (Vec2d const& v) const
	{
		Vec2d res = *this;
		return res -= v;
	}

	inline Vec2d operator- () const
	{
		return operator*(-1);
	}

	inline Vec2d const& operator-= (Vec2d const& v)
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	friend inline std::ostream& operator<< (std::ostream& ostr, Vec2d const& v)
	{
		return ostr << "(" << v.x << ", " << v.y << ")";
	}
};

#endif // VEC2D_H
