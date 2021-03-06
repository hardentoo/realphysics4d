#ifndef VECTOR2_H
#define VECTOR2_H

// Libraries
#include <math.h>
#include <assert.h>

namespace utility_engine
{

// Class Vector2
// This class represents a 2D vector.
class Vector2
{

    public:

        // -------------------- Attributes -------------------- //

        // Components of the vector
        float x, y;


        // -------------------- Methods -------------------- //

        // Constructor
        Vector2(float x=0, float y=0) : x(x), y(y) {}

        // Constructor
        Vector2(const Vector2& vector) : x(vector.x), y(vector.y) {}


        // DotProduct
        float dot( const Vector2& vector ) const
        {
            return ((x*vector.x) + (y*vector.y));
        }

        // CrossProduct
        float cross( const Vector2& vector ) const
        {
            return ((x*vector.y) - (y*vector.x));
        }


        //--------------------------------//

        // - operator
        Vector2 operator-() const
        {
            return Vector2(-x, -y);
        }


        // = operator
        Vector2& operator=(const Vector2& vector)
        {
            if (&vector != this)
            {
                x = vector.x;
                y = vector.y;
            }
            return *this;
        }

        // == operator
        bool operator==(const Vector2 &v) const
        {
            return x == v.x && y == v.y;
        }

        // [] operator
        float &operator[](int i)
        {
            assert(i >= 0 && i <= 1);
            switch (i)
            {
                case 0: return x;
                case 1: return y;
            }
            return y;
        }




        // -------------------- Friends -------------------- //

         // + operator
        friend Vector2 operator+(const Vector2& vector1, const Vector2& vector2)
        {
          return Vector2( vector1.x + vector2.x, vector1.y + vector2.y );
        }

         // - operator
        friend Vector2 operator-(const Vector2& vector1, const Vector2& vector2)
        {
          return Vector2( vector1.x - vector2.x, vector1.y - vector2.y );
        }

        // * operator
        friend Vector2 operator*(const Vector2& vector, float number)
        {
            return Vector2( number * vector.x, number * vector.y );
        }

        // * operator
        friend Vector2 operator*(float number, const Vector2& vector)
        {
            return Vector2( vector.x * number , vector.y * number );
        }

        // / operator
        friend Vector2 operator/(const Vector2& vector, float number)
        {
          assert(number!=0);
          float inv = 1.f / number;
          return Vector2( inv * vector.x, inv * vector.y );
        }

        // / operator
        friend Vector2 operator/(float number, const Vector2& vector)
        {
          assert(number!=0);
          float inv = 1.f / number;
          return Vector2( vector.x * inv  , vector.y * inv );
        }

        // * operator vector
        friend Vector2 operator*(const Vector2& vector1, const Vector2& vector2)
        {
          return Vector2( vector1.x * vector2.x, vector1.y * vector2.y );
        }


        //---------------------------------------------------------------//


        // += operator
        Vector2& operator+=(const Vector2 &v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }


        // -= operator
        Vector2& operator-=(const Vector2 &v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }



        // *= operator
        Vector2 &operator*=(float f)
        {
            x *= f;
            y *= f;
            return *this;
        }


        // /= operator
        Vector2 &operator/=(float f)
        {
            assert(f!=0);
            float inv = 1.f / f;
            x *= inv;
            y *= inv;
            return *this;
        }


        //        // + operator
        //        Vector2 operator+(const Vector2 &v) const
        //        {
        //            return Vector2(x + v.x, y + v.y);
        //        }


        //        // - operator
        //        Vector2 operator-(const Vector2 &v) const
        //        {
        //            return Vector2(x - v.x, y - v.y);
        //        }



        //        // * operator
        //        Vector2 operator*(float f) const
        //        {
        //            return Vector2(f*x, f*y);
        //        }



        //        // / operator
        //        Vector2 operator/(float f) const
        //        {
        //            assert(f!=0);
        //            float inv = 1.f / f;
        //            return Vector2(x * inv, y * inv);
        //        }



        //---------------------------------------------------------------//


        // Normalize the vector and return it
        Vector2 normalize()
        {
            float l = length();
            assert(l > 0);
            x /= l;
            y /= l;
            return *this;
        }

        // Clamp the vector values between 0 and 1
        Vector2 clamp01()
        {
            if (x>1.f) x=1.f; else if (x<0.f) x=0.f;
            if (y>1.f) y=1.f; else if (y<0.f) y=0.f;
            return *this;
        }

        // Return the squared length of the vector
        float lengthSquared() const { return x*x + y*y; }

        // Return the length of the vector
        float length() const { return sqrt(lengthSquared()); }
};


}


#endif
