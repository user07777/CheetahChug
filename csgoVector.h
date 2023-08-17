#pragma once
#pragma once
#include <cmath>

#define DECL_ALIGN(x) __declspec(align(x))
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN16_POST DECL_ALIGN(16)

class vec3
{
public:
    vec3(void)
    {
        Invalidate();
    }
    vec3(float X, float Y, float Z)
    {
        x = X;
        y = Y;
        z = Z;
    }
    vec3(const float* clr)
    {
        x = clr[0];
        y = clr[1];
        z = clr[2];
    }

    void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
    {
        x = ix; y = iy; z = iz;
    }
    bool IsValid() const
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }
    void Invalidate()
    {
        x = y = z = std::numeric_limits<float>::infinity();
    }

    float& operator[](int i)
    {
        return ((float*)this)[i];
    }
    float operator[](int i) const
    {
        return ((float*)this)[i];
    }

    void Zero()
    {
        x = y = z = 0.0f;
    }

    bool operator==(const vec3& src) const
    {
        return (src.x == x) && (src.y == y) && (src.z == z);
    }
    bool operator!=(const vec3& src) const
    {
        return (src.x != x) || (src.y != y) || (src.z != z);
    }

    vec3& operator+=(const vec3& v)
    {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    vec3& operator-=(const vec3& v)
    {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    vec3& operator*=(float fl)
    {
        x *= fl;
        y *= fl;
        z *= fl;
        return *this;
    }
    vec3& operator*=(const vec3& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    vec3& operator/=(const vec3& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    vec3& operator+=(float fl)
    {
        x += fl;
        y += fl;
        z += fl;
        return *this;
    }
    vec3& operator/=(float fl)
    {
        x /= fl;
        y /= fl;
        z /= fl;
        return *this;
    }
    vec3& operator-=(float fl)
    {
        x -= fl;
        y -= fl;
        z -= fl;
        return *this;
    }

    void NormalizeInPlace()
    {
        *this = Normalized();
    }
    vec3 Normalized() const
    {
        vec3 res = *this;
        float l = res.Length();
        if (l != 0.0f) {
            res /= l;
        }
        else {
            res.x = res.y = res.z = 0.0f;
        }
        return res;
    }

    float DistTo(const vec3& vOther) const
    {
        vec3 delta;

        delta.x = x - vOther.x;
        delta.y = y - vOther.y;
        delta.z = z - vOther.z;

        return delta.Length();
    }
    float DistToSqr(const vec3& vOther) const
    {
        vec3 delta;

        delta.x = x - vOther.x;
        delta.y = y - vOther.y;
        delta.z = z - vOther.z;

        return delta.LengthSqr();
    }
    float Dot(const vec3& vOther) const
    {
        return (x * vOther.x + y * vOther.y + z * vOther.z);
    }
    float Length() const
    {
        return sqrt(x * x + y * y + z * z);
    }
    float LengthSqr(void) const
    {
        return (x * x + y * y + z * z);
    }
    float Length2D() const
    {
        return sqrt(x * x + y * y);
    }

    vec3& operator=(const vec3& vOther)
    {
        x = vOther.x; y = vOther.y; z = vOther.z;
        return *this;
    }

    vec3 operator-(void) const
    {
        return vec3(-x, -y, -z);
    }
    vec3 operator+(const vec3& v) const
    {
        return vec3(x + v.x, y + v.y, z + v.z);
    }
    vec3 operator-(const vec3& v) const
    {
        return vec3(x - v.x, y - v.y, z - v.z);
    }
    vec3 operator*(float fl) const
    {
        return vec3(x * fl, y * fl, z * fl);
    }
    vec3 operator*(const vec3& v) const
    {
        return vec3(x * v.x, y * v.y, z * v.z);
    }
    vec3 operator/(float fl) const
    {
        return vec3(x / fl, y / fl, z / fl);
    }
    vec3 operator/(const vec3& v) const
    {
        return vec3(x / v.x, y / v.y, z / v.z);
    }

    float x, y, z;
};

inline vec3 operator*(float lhs, const vec3& rhs)
{
    return rhs * lhs;
}
inline vec3 operator/(float lhs, const vec3& rhs)
{
    return rhs / lhs;
}

class ALIGN16 VectorAligned : public vec3
{
public:
    inline VectorAligned(void) {};

    inline VectorAligned(vec3 X, vec3 Y, vec3 Z)
    {
        //Init(X, Y, Z);
    }

    explicit VectorAligned(const vec3& vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
    }

    VectorAligned& operator=(const vec3& vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
        return *this;
    }

    float w;	// this space is used anyway
} ALIGN16_POST;