#ifndef LAMPMATH_H
#define LAMPMATH_H

namespace Lamp {
    namespace {
        template <typename T>
        class Vec3 {
        public:
            T x = 0, y = 0, z = 0;
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
        };

        template <typename T>
        class Mat4 {

        public:
            Vec4<T> c0 = {1.0f, 0.0f, 0.0f, 0.0f};
            Vec4<T> c1 = {0.0f, 1.0f, 0.0f, 0.0f};
            Vec4<T> c2 = {0.0f, 0.0f, 1.0f, 0.0f};
            Vec4<T> c3 = {0.0f, 0.0f, 0.0f, 1.0f};
        };

    }

    using Vec3f = Vec3<float>;
    using Vec4f = Vec4<float>;
    using Mat4f = Mat4<float>;



};

#endif //LAMPMATH_H
