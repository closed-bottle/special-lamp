#ifndef LAMPPAIR_H
#define LAMPPAIR_H

namespace Lamp {
    template<typename T1, typename T2>
    struct pair {
        pair() = default;

        pair(const T1 &t1, const T2 &t2) : first(t1), second(t2) {
        }

        pair(T1&& t1, T2&& t2) : first(std::move(t1)), second(std::move(t2)) {
        }

        bool operator==(const pair &_rhs) const {
            return _rhs.first == first && _rhs.second == second;
        }

        bool operator!=(const pair &_rhs) const {
            return _rhs.first != first || _rhs.second != second;
        }

        bool operator<(const pair& _rhs) const {
            return first < _rhs.first;
        }

        bool operator>(const pair& _rhs) const {
            return first > _rhs.first;
        }

        T1 first;
        T2 second;
    };
}

#endif //LAMPPAIR_H
