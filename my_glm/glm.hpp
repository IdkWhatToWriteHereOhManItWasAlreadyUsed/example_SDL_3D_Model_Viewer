#pragma once


#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <cmath>
#include <immintrin.h>


/*
namespace glm 
{
    template<typename T>
    constexpr T pi() { return static_cast<T>(3.14159265358979323846264338327950288); }

    template<typename T>
    T radians(T degrees) { return degrees * pi<T>() / static_cast<T>(180); }

    struct vec2;
    struct vec3;
    struct vec4;
    struct mat3;
    struct mat4;

    template<typename T>
    T min(T a, T b) { return a < b ? a : b; }

    template<typename T>
    T max(T a, T b) { return a > b ? a : b; }

    struct vec2 
    {
        union 
        {
            struct { float x, y; };
            struct { float r, g; };
            struct { float s, t; };
            float data[2];
        };

        vec2() : x(0), y(0) {}
        explicit vec2(float s) : x(s), y(s) {}
        vec2(float x, float y) : x(x), y(y) {}

        float& operator[](size_t i) { return data[i]; }
        const float& operator[](size_t i) const { return data[i]; }

        vec2 operator-() const { return vec2(-x, -y); }
        
        vec2& operator+=(const vec2& v) { x += v.x; y += v.y; return *this; }
        vec2& operator-=(const vec2& v) { x -= v.x; y -= v.y; return *this; }
        vec2& operator*=(float s) { x *= s; y *= s; return *this; }
        vec2& operator/=(float s) { x /= s; y /= s; return *this; }
        vec2& operator*=(const vec2& v) { x *= v.x; y *= v.y; return *this; }
        vec2& operator/=(const vec2& v) { x /= v.x; y /= v.y; return *this; }
    };

    inline vec2 operator+(const vec2& a, const vec2& b) { return vec2(a.x + b.x, a.y + b.y); }
    inline vec2 operator-(const vec2& a, const vec2& b) { return vec2(a.x - b.x, a.y - b.y); }
    inline vec2 operator*(const vec2& a, const vec2& b) { return vec2(a.x * b.x, a.y * b.y); }
    inline vec2 operator/(const vec2& a, const vec2& b) { return vec2(a.x / b.x, a.y / b.y); }
    inline vec2 operator*(const vec2& a, float s) { return vec2(a.x * s, a.y * s); }
    inline vec2 operator*(float s, const vec2& a) { return vec2(a.x * s, a.y * s); }
    inline vec2 operator/(const vec2& a, float s) { return vec2(a.x / s, a.y / s); }

    struct vec3 
    {
        union 
        {
            struct { float x, y, z; };
            struct { float r, g, b; };
            struct { float s, t, p; };
            float data[3];
        };

        vec3() : x(0), y(0), z(0) {}
        explicit vec3(float s) : x(s), y(s), z(s) {}
        vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        vec3(const vec2& v, float z) : x(v.x), y(v.y), z(z) {}

     

        float& operator[](size_t i) { return data[i]; }
        const float& operator[](size_t i) const { return data[i]; }

        vec3 operator-() const { return vec3(-x, -y, -z); }
        
        vec3& operator+=(const vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
        vec3& operator-=(const vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
        vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
        vec3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
        vec3& operator*=(const vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
        vec3& operator/=(const vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
    };

    inline vec3 operator+(const vec3& a, const vec3& b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
    inline vec3 operator-(const vec3& a, const vec3& b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
    inline vec3 operator*(const vec3& a, const vec3& b) { return vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
    inline vec3 operator/(const vec3& a, const vec3& b) { return vec3(a.x / b.x, a.y / b.y, a.z / b.z); }
    inline vec3 operator*(const vec3& a, float s) { return vec3(a.x * s, a.y * s, a.z * s); }
    inline vec3 operator*(float s, const vec3& a) { return vec3(a.x * s, a.y * s, a.z * s); }
    inline vec3 operator/(const vec3& a, float s) { return vec3(a.x / s, a.y / s, a.z / s); }

    struct vec4 
    {
        union 
        {
            struct { float x, y, z, w; };
            struct { float r, g, b, a; };
            struct { float s, t, p, q; };
            float data[4];
        };

        vec4() : x(0), y(0), z(0), w(0) {}
        explicit vec4(float s) : x(s), y(s), z(s), w(s) {}
        vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        vec4(const vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

        float& operator[](size_t i) { return data[i]; }
        const float& operator[](size_t i) const { return data[i]; }

        vec4 operator-() const { return vec4(-x, -y, -z, -w); }
        
        vec4& operator+=(const vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
        vec4& operator-=(const vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
        vec4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
        vec4& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }

        inline operator vec3() const { return vec3(x, y, z); }
    };

    inline vec4 operator+(const vec4& a, const vec4& b) { return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
    inline vec4 operator-(const vec4& a, const vec4& b) { return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
    inline vec4 operator*(const vec4& a, float s) { return vec4(a.x * s, a.y * s, a.z * s, a.w * s); }
    inline vec4 operator*(float s, const vec4& a) { return vec4(a.x * s, a.y * s, a.z * s, a.w * s); }
    inline vec4 operator/(const vec4& a, float s) { return vec4(a.x / s, a.y / s, a.z / s, a.w / s); }

    struct mat3 
    {
        vec3 value[3];

        mat3() : value{vec3(1,0,0), vec3(0,1,0), vec3(0,0,1)} {}
        explicit mat3(float s) : value{vec3(s,0,0), vec3(0,s,0), vec3(0,0,s)} {}
        mat3(const vec3& c0, const vec3& c1, const vec3& c2) : value{c0, c1, c2} {}

        vec3& operator[](size_t i) { return value[i]; }
        const vec3& operator[](size_t i) const { return value[i]; }

        const float* data() const { return &value[0].x; }
    };

    struct mat4 
    {
        vec4 value[4];

        mat4() : value{vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1)} {}
        explicit mat4(float s) : value{vec4(s,0,0,0), vec4(0,s,0,0), vec4(0,0,s,0), vec4(0,0,0,s)} {}
        mat4(const vec4& c0, const vec4& c1, const vec4& c2, const vec4& c3) : value{c0, c1, c2, c3} {}

        vec4& operator[](size_t i) { return value[i]; }
        const vec4& operator[](size_t i) const { return value[i]; }

        const float* data() const { return &value[0].x; }
    };

    inline mat4 operator*(const mat4& m1, const mat4& m2) 
    {
        const float* m1_ptr = m1.data();
        const float* m2_ptr = m2.data();
    
        mat4 result;
        float* res_ptr = (float*)result.data();
        
        for (int i = 0; i < 4; ++i) 
        {
            for (int j = 0; j < 4; ++j) 
            {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k) 
                {
                    sum += m1_ptr[i + k * 4] * m2_ptr[k + j * 4];
                }
                res_ptr[i + j * 4] = sum;
            }
        }
        return result;
    }

    inline mat3 operator*(const mat3& m1, const mat3& m2) 
    {
        const float* m1_ptr = m1.data();
        const float* m2_ptr = m2.data();
        
        mat3 result;
        float* res_ptr = (float*)result.data();
        
        for (int i = 0; i < 3; ++i) 
        {
            for (int j = 0; j < 3; ++j) 
            {
                float sum = 0.0f;
                for (int k = 0; k < 3; ++k) {
                    sum += m1_ptr[i + k * 3] * m2_ptr[k + j * 3];
                }
                res_ptr[i + j * 3] = sum;
            }
        }
        return result;
    }

    inline vec4 operator*(const mat4& m, const vec4& v) 
    {
        const float* m_ptr = m.data();
        return vec4
        (
            m_ptr[0] * v.x + m_ptr[4] * v.y + m_ptr[8] * v.z + m_ptr[12] * v.w,
            m_ptr[1] * v.x + m_ptr[5] * v.y + m_ptr[9] * v.z + m_ptr[13] * v.w,
            m_ptr[2] * v.x + m_ptr[6] * v.y + m_ptr[10] * v.z + m_ptr[14] * v.w,
            m_ptr[3] * v.x + m_ptr[7] * v.y + m_ptr[11] * v.z + m_ptr[15] * v.w
        );
    }

    inline vec3 operator*(const mat3& m, const vec3& v)
     {
        const float* m_ptr = m.data();
        return vec3(
            m_ptr[0] * v.x + m_ptr[3] * v.y + m_ptr[6] * v.z,
            m_ptr[1] * v.x + m_ptr[4] * v.y + m_ptr[7] * v.z,
            m_ptr[2] * v.x + m_ptr[5] * v.y + m_ptr[8] * v.z
        );
    }

    inline vec4 operator*(const vec4& v, const mat4& m) 
    {
        const float* m_ptr = m.data();
        return vec4
        (
            v.x * m_ptr[0] + v.y * m_ptr[1] + v.z * m_ptr[2] + v.w * m_ptr[3],
            v.x * m_ptr[4] + v.y * m_ptr[5] + v.z * m_ptr[6] + v.w * m_ptr[7],
            v.x * m_ptr[8] + v.y * m_ptr[9] + v.z * m_ptr[10] + v.w * m_ptr[11],
            v.x * m_ptr[12] + v.y * m_ptr[13] + v.z * m_ptr[14] + v.w * m_ptr[15]
        );
    }

    inline float dot(const vec2& a, const vec2& b) { return a.x * b.x + a.y * b.y; }
    inline float dot(const vec3& a, const vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    inline float dot(const vec4& a, const vec4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

    inline float length(const vec2& v) { return std::sqrt(dot(v, v)); }
    inline float length(const vec3& v) { return std::sqrt(dot(v, v)); }
    inline float length(const vec4& v) { return std::sqrt(dot(v, v)); }

    inline vec2 normalize(const vec2& v) 
    { 
        float len = length(v);
        return len > 0 ? v / len : vec2(0); 
    }
    
    inline vec3 normalize(const vec3& v) 
    { 
        float len = length(v);
        return len > 0 ? v / len : vec3(0); 
    }
    
    inline vec4 normalize(const vec4& v) 
    { 
        float len = length(v);
        return len > 0 ? v / len : vec4(0); 
    }

    inline vec3 cross(const vec3& a, const vec3& b) 
    {
        return vec3
        (
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    inline mat4 translate(const mat4& m, const vec3& v) 
    {
        mat4 result(m);
        result[3] = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
        return result;
    }

    inline mat4 rotate(const mat4& m, float angle, const vec3& axis) 
    {
        const float a = angle;
        const float c = std::cos(a);
        const float s = std::sin(a);
        
        vec3 norm = normalize(axis);
        vec3 temp = norm * (1.0f - c);
        
        mat4 rot;
        rot[0][0] = c + temp[0] * norm[0];
        rot[0][1] = temp[0] * norm[1] + s * norm[2];
        rot[0][2] = temp[0] * norm[2] - s * norm[1];
        
        rot[1][0] = temp[1] * norm[0] - s * norm[2];
        rot[1][1] = c + temp[1] * norm[1];
        rot[1][2] = temp[1] * norm[2] + s * norm[0];
        
        rot[2][0] = temp[2] * norm[0] + s * norm[1];
        rot[2][1] = temp[2] * norm[1] - s * norm[0];
        rot[2][2] = c + temp[2] * norm[2];
        
        mat4 result;
        result[0] = m[0] * rot[0][0] + m[1] * rot[0][1] + m[2] * rot[0][2];
        result[1] = m[0] * rot[1][0] + m[1] * rot[1][1] + m[2] * rot[1][2];
        result[2] = m[0] * rot[2][0] + m[1] * rot[2][1] + m[2] * rot[2][2];
        result[3] = m[3];
        
        return result;
    }

    inline mat4 perspective(float fovy, float aspect, float zNear, float zFar) 
    {  
        float const tanHalfFovy = std::tan(fovy / 2.0f);
        
        mat4 result(0.0f);
        result[0][0] = 1.0f / (aspect * tanHalfFovy);
        result[1][1] = 1.0f / tanHalfFovy;
        result[2][2] = -(zFar + zNear) / (zFar - zNear);
        result[2][3] = -1.0f;
        result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
        
        return result;
    }

    inline mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) 
    {
        vec3 f = normalize(center - eye);
        vec3 s = normalize(cross(f, up));
        vec3 u = cross(s, f);
        
        mat4 result(1.0f);
        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;
        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;
        result[0][2] = -f.x;
        result[1][2] = -f.y;
        result[2][2] = -f.z;
        result[3][0] = -dot(s, eye);
        result[3][1] = -dot(u, eye);
        result[3][2] = dot(f, eye);
        
        return result;
    }

    inline mat4 scale(const mat4& m, const vec3& v) 
    {
        mat4 result(m);
        result[0] = m[0] * v.x;
        result[1] = m[1] * v.y;
        result[2] = m[2] * v.z;
        return result;
    }

    inline float distance(const vec3& a, const vec3& b) 
    {
        return length(b - a);
    }

    inline vec3 reflect(const vec3& I, const vec3& N) 
    {
        return I - N * dot(N, I) * 2.0f;
    }

    template<typename T>
    T clamp(T x, T minVal, T maxVal) { 
        return min(max(x, minVal), maxVal); 
    }

    inline vec3 clamp(vec3& x, float minVal, float maxVal) 
    { 
        return vec3
        (
            min(max(x[0], minVal), maxVal),
            min(max(x[1], minVal), maxVal),
            min(max(x[2], minVal), maxVal)
        ); 
    }

    inline vec2 clamp(const vec2& x, const vec2& minVal, const vec2& maxVal) {
        return vec2(clamp(x.x, minVal.x, maxVal.x), clamp(x.y, minVal.y, maxVal.y));
    }

    inline vec3 clamp(const vec3& x, const vec3& minVal, const vec3& maxVal) {
        return vec3(clamp(x.x, minVal.x, maxVal.x), 
                    clamp(x.y, minVal.y, maxVal.y), 
                    clamp(x.z, minVal.z, maxVal.z));
    }

    inline vec4 clamp(const vec4& x, const vec4& minVal, const vec4& maxVal) {
        return vec4(clamp(x.x, minVal.x, maxVal.x),
                    clamp(x.y, minVal.y, maxVal.y),
                    clamp(x.z, minVal.z, maxVal.z),
                    clamp(x.w, minVal.w, maxVal.w));
    }
}
*/