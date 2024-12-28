#ifndef LAMPMATH_H
#define LAMPMATH_H
#include <complex>

namespace Lamp {
    namespace {
        template <typename T>
        class Vec3 {
        public:
            T x = 0, y = 0, z = 0;

            T Dot(const Vec3& _rhs) {
                return x*_rhs.x + y*_rhs.y + z*_rhs.z;
            }

            Vec3 Cross(const Vec3& _rhs) const {
                Vec3 result;

                result.x = (y*_rhs.z) - (z*_rhs.y);
                result.y = (z*_rhs.x) - (x*_rhs.z);
                result.z = (x*_rhs.y) - (y*_rhs.x);

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

            Vec3& operator-=(const Vec3 & _rhs) {
                x -= _rhs.x;
                y -= _rhs.y;
                z -= _rhs.z;

                return *this;
            }

            Vec3 operator-(const Vec3 & _rhs) const {
                Vec3 result = *this;
                result -= _rhs;
                return result;
            }

            Vec3& operator+=(const Vec3 & _rhs) {
                x += _rhs.x;
                y += _rhs.y;
                z += _rhs.z;

                return *this;
            }

            Vec3 operator+(const Vec3 & _rhs) const {
                Vec3 result = *this;
                result += _rhs;
                return result;
            }
        };

        template <typename T>
        class Vec4 {
        public:
            union {
                struct {
                    T x = 0, y = 0, z = 0;
                };
                Vec3<T> vec3;
            };
            T w = 1.0f;

            Vec4() {}
            Vec4(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w) { }

            T Length() {
                return std::sqrt(x*x + y*y + z*z + w*w);
            }

            Vec4 Normalize() {
                const T length = Length();
                x /= length;
                y /= length;
                z /= length;
                w /= length;

                return *this;
            }

            Vec4& operator+=(const Vec4& _rhs) {
                x += _rhs.x;
                y += _rhs.y;
                z += _rhs.z;
                w += _rhs.w;

                return *this;
            }

            Vec4 operator+(const Vec4& _rhs) const {
                Vec4 result = *this;
                result += _rhs;
                return result;
            }

            Vec4& operator-=(const Vec4& _rhs) {
                x -= _rhs.x;
                y -= _rhs.y;
                z -= _rhs.z;
                w -= _rhs.w;

                return *this;
            }

            Vec4 operator-(const Vec4& _rhs) const {
                Vec4 result = *this;
                result -= _rhs;
                return result;
            }
        };

        template <typename T>
        class Mat4 {

        public:
            Vec4<T> c0 = {1.0f, 0.0f, 0.0f, 0.0f};
            Vec4<T> c1 = {0.0f, 1.0f, 0.0f, 0.0f};
            Vec4<T> c2 = {0.0f, 0.0f, 1.0f, 0.0f};
            Vec4<T> c3 = {0.0f, 0.0f, 0.0f, 1.0f};

            static Mat4 Translate(T _x, T _y, T _z) {
                Mat4 result;

                result.c3.vec3 = {_x, _y, _z};

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

            static Mat4 LookAt(const Vec3<T>& _eye, const Vec3<T>& _center, const Vec3<T>& _up) {
                Mat4 result;
                Vec3<T> a;
                Vec3<T> b;
                Vec3<T> c;

                c = _eye - _center;
                c.Normalize();

                a = _up.Cross(c);

                b = c.Cross(a);

                a.Normalize();
                b.Normalize();

                result.c0.vec3 = {a.x, b.x, c.x};
                result.c1.vec3 = {a.y, b.y, c.y};
                result.c2.vec3 = {a.z, b.z, c.z};

                return result;
            }
        };
    }

    using Vec3f = Vec3<float>;
    using Vec4f = Vec4<float>;
    using Mat4f = Mat4<float>;

};

#endif //LAMPMATH_H
