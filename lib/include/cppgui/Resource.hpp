#pragma once

/** This version is for resource types that need no translation
 */
template<typename T>
struct Non_mapped_resource {
    T val;
    void assign(const T &v) { val = v; }
    void map() {} // no-op
    T get() { return val; }
    void release() {} // no-op
};

template<typename ...Members> struct Resource_struct: public Members... {};