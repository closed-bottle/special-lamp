#ifndef LAMPMATH_H
#define LAMPMATH_H
#include <complex>

namespace Lamp {
        template<typename T>
    class Vec2 {
    public:
        T x = 0, y = 0;

        T Dot(const Vec2 &_rhs) {
            return x * _rhs.x + y * _rhs.y;
        }

        T Length() {
            return std::sqrt(Dot(*this));
        }

        Vec2 Normalize() {
            T length = Length();

            x /= length;
            y /= length;

            return *this;
        }

        Vec2 &operator-=(const Vec2 &_rhs) {
            x -= _rhs.x;
            y -= _rhs.y;

            return *this;
        }

        Vec2 operator-(const Vec2 &_rhs) const {
            Vec2 result = *this;
            result -= _rhs;
            return result;
        }

        Vec2 &operator+=(const Vec2 &_rhs) {
            x += _rhs.x;
            y += _rhs.y;

            return *this;
        }

        Vec2 operator+(const Vec2 &_rhs) const {
            Vec2 result = *this;
            result += _rhs;
            return result;
        }

        Vec2 operator-() const {
            return {-x, -y};
        }

        Vec2 operator+(const T& _rhs) const {
            Vec2 result = *this;
            result.x += _rhs;
            result.y += _rhs;
            return result;
        }

        Vec2 operator-(const T& _rhs) const {
            Vec2 result = *this;
            result.x -= _rhs;
            result.y -= _rhs;
            return result;
        }

        Vec2 operator*(const T& _rhs) const {
            Vec2 result = *this;
            result.x *= _rhs;
            result.y *= _rhs;
            return result;
        }

        Vec2 operator/(const T& _rhs) const {
            Vec2 result = *this;
            result.x /= _rhs;
            result.y /= _rhs;
            return result;
        }

        Vec2& operator+=(const T& _rhs) {
            x += _rhs;
            y += _rhs;
            return *this;
        }

        Vec2& operator-=(const T& _rhs) {
            x -= _rhs;
            y -= _rhs;
            return *this;
        }

        Vec2& operator*=(const float& _rhs) {
            x *= _rhs;
            y *= _rhs;
            return *this;
        }

        Vec2& operator/=(const float& _rhs) {
            x /= _rhs;
            y /= _rhs;
            return *this;
        }
    };

    template<typename T>
    class Vec3 {
    public:
        T x = 0, y = 0, z = 0;

        T Dot(const Vec3 &_rhs) {
            return x * _rhs.x + y * _rhs.y + z * _rhs.z;
        }

        Vec3 Cross(const Vec3 &_rhs) const {
            Vec3 result;

            result.x = (y * _rhs.z) - (z * _rhs.y);
            result.y = (z * _rhs.x) - (x * _rhs.z);
            result.z = (x * _rhs.y) - (y * _rhs.x);

            return result;
        }

        T Length() {
            return std::sqrt(Dot(*this));
        }

        Vec3 Normalize() {
            T length = Length();

            x /= length;
            y /= length;
            z /= length;

            return *this;
        }

        Vec3 &operator-=(const Vec3 &_rhs) {
            x -= _rhs.x;
            y -= _rhs.y;
            z -= _rhs.z;

            return *this;
        }

        Vec3 operator-(const Vec3 &_rhs) const {
            Vec3 result = *this;
            result -= _rhs;
            return result;
        }

        Vec3 &operator+=(const Vec3 &_rhs) {
            x += _rhs.x;
            y += _rhs.y;
            z += _rhs.z;

            return *this;
        }

        Vec3 operator+(const Vec3 &_rhs) const {
            Vec3 result = *this;
            result += _rhs;
            return result;
        }

        Vec3 operator-() const {
            return {-x, -y, -z};
        }

        Vec3 operator+(const T& _rhs) const {
            Vec3 result = *this;
            result.x += _rhs;
            result.y += _rhs;
            result.z += _rhs;
            return result;
        }

        Vec3 operator-(const T& _rhs) const {
            Vec3 result = *this;
            result.x -= _rhs;
            result.y -= _rhs;
            result.z -= _rhs;
            return result;
        }

        Vec3 operator*(const T& _rhs) const {
            Vec3 result = *this;
            result.x *= _rhs;
            result.y *= _rhs;
            result.z *= _rhs;
            return result;
        }

        Vec3 operator/(const T& _rhs) const {
            Vec3 result = *this;
            result.x /= _rhs;
            result.y /= _rhs;
            result.z /= _rhs;
            return result;
        }

        Vec3& operator+=(const T& _rhs) {
            x += _rhs;
            y += _rhs;
            z += _rhs;
            return *this;
        }

        Vec3& operator-=(const T& _rhs) {
            x -= _rhs;
            y -= _rhs;
            z -= _rhs;
            return *this;
        }

        Vec3& operator*=(const float& _rhs) {
            x *= _rhs;
            y *= _rhs;
            z *= _rhs;
            return *this;
        }

        Vec3& operator/=(const float& _rhs) {
            x /= _rhs;
            y /= _rhs;
            z /= _rhs;
            return *this;
        }
    };

    template<typename T>
    class Vec4 {
    public:
        union {
            struct {
                T x = 0, y = 0, z = 0;
            };

            Vec3<T> vec3;
        };

        T w = 1.0f;

        Vec4() {
        }

        Vec4(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w) {
        }

        T Length() {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        Vec4 Normalize() {
            const T length = Length();
            x /= length;
            y /= length;
            z /= length;
            w /= length;

            return *this;
        }

        Vec4 &operator+=(const Vec4 &_rhs) {
            x += _rhs.x;
            y += _rhs.y;
            z += _rhs.z;
            w += _rhs.w;

            return *this;
        }

        Vec4 operator+(const Vec4 &_rhs) const {
            Vec4 result = *this;
            result += _rhs;
            return result;
        }

        Vec4 &operator-=(const Vec4 &_rhs) {
            x -= _rhs.x;
            y -= _rhs.y;
            z -= _rhs.z;
            w -= _rhs.w;

            return *this;
        }

        Vec4 operator-(const Vec4 &_rhs) const {
            Vec4 result = *this;
            result -= _rhs;
            return result;
        }

        Vec4 operator+(const T& _rhs) const {
            Vec4 result = *this;
            result.x += _rhs;
            result.y += _rhs;
            result.z += _rhs;
            result.w += _rhs;
            return result;
        }

        Vec4 operator-(const T& _rhs) const {
            Vec4 result = *this;
            result.x -= _rhs;
            result.y -= _rhs;
            result.z -= _rhs;
            result.w -= _rhs;
            return result;
        }

        Vec4 operator*(const T& _rhs) const {
            Vec4 result = *this;
            result.x *= _rhs;
            result.y *= _rhs;
            result.z *= _rhs;
            result.w *= _rhs;
            return result;
        }

        Vec4 operator/(const T& _rhs) const {
            Vec4 result = *this;
            result.x /= _rhs;
            result.y /= _rhs;
            result.z /= _rhs;
            result.w /= _rhs;
            return result;
        }

        Vec4& operator+=(const T& _rhs) {
            x += _rhs;
            y += _rhs;
            z += _rhs;
            w += _rhs;
            return *this;
        }

        Vec4& operator-=(const T& _rhs) {
            x -= _rhs;
            y -= _rhs;
            z -= _rhs;
            w -= _rhs;
            return *this;
        }

        Vec4& operator*=(const float& _rhs) {
            x *= _rhs;
            y *= _rhs;
            z *= _rhs;
            w *= _rhs;
            return *this;
        }

        Vec4& operator/=(const float& _rhs) {
            x /= _rhs;
            y /= _rhs;
            z /= _rhs;
            w /= _rhs;
            return *this;
        }
    };

    // TODO: Add more effective algorithms.
    // For example, strassen for multiplication.
    // effective transpose.
    template<typename T>
    class Mat4 {
    public:
        union {
            struct {
                Vec4<T> c0 = {1.0f, 0.0f, 0.0f, 0.0f};
                Vec4<T> c1 = {0.0f, 1.0f, 0.0f, 0.0f};
                Vec4<T> c2 = {0.0f, 0.0f, 1.0f, 0.0f};
                Vec4<T> c3 = {0.0f, 0.0f, 0.0f, 1.0f};
            };

            struct {
                float arr[16];
            };
        };

        Mat4 operator*(const Mat4 &_rhs) const {
            Mat4 result;

            // Using Dotproduct instead?
            result.c0.x = c0.x * _rhs.c0.x + c1.x * _rhs.c0.y + c2.x * _rhs.c0.z + c3.x * _rhs.c0.w;
            result.c0.y = c0.y * _rhs.c0.x + c1.y * _rhs.c0.y + c2.y * _rhs.c0.z + c3.y * _rhs.c0.w;
            result.c0.z = c0.z * _rhs.c0.x + c1.z * _rhs.c0.y + c2.z * _rhs.c0.z + c3.z * _rhs.c0.w;
            result.c0.w = c0.w * _rhs.c0.x + c1.w * _rhs.c0.y + c2.w * _rhs.c0.z + c3.w * _rhs.c0.w;

            result.c1.x = c0.x * _rhs.c1.x + c1.x * _rhs.c1.y + c2.x * _rhs.c1.z + c3.x * _rhs.c1.w;
            result.c1.y = c0.y * _rhs.c1.x + c1.y * _rhs.c1.y + c2.y * _rhs.c1.z + c3.y * _rhs.c1.w;
            result.c1.z = c0.z * _rhs.c1.x + c1.z * _rhs.c1.y + c2.z * _rhs.c1.z + c3.z * _rhs.c1.w;
            result.c1.w = c0.w * _rhs.c1.x + c1.w * _rhs.c1.y + c2.w * _rhs.c1.z + c3.w * _rhs.c1.w;

            result.c2.x = c0.x * _rhs.c2.x + c1.x * _rhs.c2.y + c2.x * _rhs.c2.z + c3.x * _rhs.c2.w;
            result.c2.y = c0.y * _rhs.c2.x + c1.y * _rhs.c2.y + c2.y * _rhs.c2.z + c3.y * _rhs.c2.w;
            result.c2.z = c0.z * _rhs.c2.x + c1.z * _rhs.c2.y + c2.z * _rhs.c2.z + c3.z * _rhs.c2.w;
            result.c2.w = c0.w * _rhs.c2.x + c1.w * _rhs.c2.y + c2.w * _rhs.c2.z + c3.w * _rhs.c2.w;

            result.c3.x = c0.x * _rhs.c3.x + c1.x * _rhs.c3.y + c2.x * _rhs.c3.z + c3.x * _rhs.c3.w;
            result.c3.y = c0.y * _rhs.c3.x + c1.y * _rhs.c3.y + c2.y * _rhs.c3.z + c3.y * _rhs.c3.w;
            result.c3.z = c0.z * _rhs.c3.x + c1.z * _rhs.c3.y + c2.z * _rhs.c3.z + c3.z * _rhs.c3.w;
            result.c3.w = c0.w * _rhs.c3.x + c1.w * _rhs.c3.y + c2.w * _rhs.c3.z + c3.w * _rhs.c3.w;

            return result;
        }

        template<typename T2>
        Vec4<T2> operator*(const Vec4<T2> &_rhs) const {
            Vec4<T2> result;

            result.x = (c0.x * _rhs.x) + (c1.x * _rhs.y) + (c2.x * _rhs.z) + (c3.x * _rhs.w);
            result.y = (c0.y * _rhs.x) + (c1.y * _rhs.y) + (c2.y * _rhs.z) + (c3.y * _rhs.w);;
            result.z = (c0.z * _rhs.x) + (c1.z * _rhs.y) + (c2.z * _rhs.z) + (c3.z * _rhs.w);;
            result.w = (c0.w * _rhs.x) + (c1.w * _rhs.y) + (c2.w * _rhs.z) + (c3.w * _rhs.w);;

            return result;
        }

        Mat4 Transpose() {
            Mat4 result = *this;

            result.c0 = {c0.x, c1.x, c2.x, c3.x};
            result.c1 = {c0.y, c1.y, c2.y, c3.y};
            result.c2 = {c0.z, c1.z, c2.z, c3.z};
            result.c3 = {c0.w, c1.w, c2.w, c3.w};

            return result;
        }

        static Mat4 Translate(T _x, T _y, T _z) {
            Mat4 result;

            result.c3.vec3 = {_x, _y, _z};

            return result;
        }

        static Mat4 Translate(const Vec3<T> &_v) {
            Mat4 result;
            result.c3.vec3 = _v;
            return result;
        }

        static Mat4 Scale(T _x, T _y, T _z) {
            Mat4 result;

            result.c0.x = _x;
            result.c1.y = _y;
            result.c2.z = _z;

            return result;
        }

        static Mat4 Roll(T _rad) {
            Mat4 result;

            result.c1.y = std::cos(_rad);
            result.c1.z = std::sin(_rad);
            result.c2.y = -std::sin(_rad);
            result.c2.z = std::cos(_rad);

            return result;
        }

        static Mat4 Pitch(T _rad) {
            Mat4 result;

            result.c0.x = std::cos(_rad);
            result.c0.z = -std::sin(_rad);
            result.c2.x = std::sin(_rad);
            result.c2.z = std::cos(_rad);

            return result;
        }

        static Mat4 Yaw(T _rad) {
            Mat4 result;

            result.c0.x = std::cos(_rad);
            result.c0.y = std::sin(_rad);
            result.c1.x = -std::sin(_rad);
            result.c1.y = std::cos(_rad);

            return result;
        }

        static Mat4 LookAt(const Vec3<T> &_eye, const Vec3<T> &_center, const Vec3<T> &_global_up,
                           const bool _is_left_handed = true) {
            Vec3<T> forward;
            Vec3<T> right;
            Vec3<T> up;

            forward = (_eye - _center);
            forward.Normalize();

            if (_is_left_handed) {
                right = _global_up.Cross(forward);
                right.Normalize();
            }
            else {
                right = forward.Cross(_global_up);
                right.Normalize();
            }

            up = forward.Cross(right);
            up.Normalize();

            Mat4 t = Mat4::Translate(-_eye);
            Mat4 r;
            r.c0.vec3 = {right.x, up.x, -forward.x};
            r.c1.vec3 = {right.y, up.y, -forward.y};
            r.c2.vec3 = {right.z, up.z, -forward.z};


            Mat4 result = r * t;

            return result;
        }

        static Mat4 Perspective(T _fovy, T _aspect, T _near, T _far, bool flip_y = false) {
            if (_fovy < 0) {
                std::cout << "Perspective() doesnt take negative fov for clarity." << std::endl;
                std::cout << "To flip clip space vertically, use 4th parameter." << std::endl;
                return Mat4();
            }

            Mat4 result;
            T tan_half_angle = std::tan(_fovy / 2);

            result.c0.x = 1.0 / (_aspect * tan_half_angle);
            result.c1.y = 1.0 / (tan_half_angle);
            result.c2.z = -(_far + _near) / (_far - _near);
            result.c3.w = 0;

            result.c2.w = -1;
            result.c3.z = -(2.0 * _far * _near) / (_far - _near);


            if (flip_y) {
                result.c1.y = -result.c1.y;
            }

            return result;
        }
    };

    using Vec2f = Vec2<float>;
    using Vec2i = Vec2<int>;
    using Vec3f = Vec3<float>;
    using Vec4f = Vec4<float>;
    using Mat4f = Mat4<float>;
};

#endif //LAMPMATH_H
