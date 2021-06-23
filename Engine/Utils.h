#ifndef UTILS_H
#define UTILS_H

template <typename Map>
struct less_by_t {
    Map f;

    template <typename T>
    bool operator ()(T const& a, T const& b) const {
        return f(a) < f(b);
    }
};

template <typename Map>
less_by_t<Map> less_by(Map map) {
    return {map};
}

#endif //UTILS_H
