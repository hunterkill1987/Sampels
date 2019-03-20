
class Point2d
{
public:
	Point2d() : m_X(0), m_Y(0) {}
	Point2d(float x, float y) : m_X(x), m_Y(y) {}

	float m_X, m_Y;

	Point2d& operator = (const Point2d& Point)
	{
		m_X = Point.m_X;
		m_Y = Point.m_Y;
		return *this;
	}

	Point2d& operator * (const float Value)
	{
		m_X *= Value;
		m_Y *= Value;
		return *this;
	}


	Point2d& operator + (const Point2d& Value)
	{
		m_X += Value.m_X;
		m_Y += Value.m_Y;
		return *this;
	}

	Point2d& operator += (const Point2d& Value)
	{
		m_X += Value.m_X;
		m_Y += Value.m_Y;
		return *this;
	}

	Point2d& operator - (const float Value)
	{
		Point2d New;
		New.m_X = m_X - Value;
		New.m_Y = m_Y - Value;
		return New;
	}

	Point2d& operator - (const Point2d& Value)
	{
		Point2d New;
		New.m_X = m_X - Value.m_X;
		New.m_Y = m_Y - Value.m_Y;
		return New;
	}


	float Length()
	{
		return sqrtf(m_X * m_X + m_Y * m_Y);
	}

	Point2d& Normalize()
	{
		m_X /= Length();
		m_Y /= Length();
		return *this;
	}
};