#pragma once

#define HAS_METHOD(Class, MethodName, ReturnType, ...) (std::is_same<ReturnType (Class::*)(__VA_ARGS__), decltype(&Class::MethodName)>::value)
#define HAS_STATIC_METHOD(Class, MethodName, ReturnType, ...) (std::is_same<ReturnType (*)(__VA_ARGS__), decltype(&Class::MethodName)>::value)

#define ASSERT_METHOD(Class, MethodName, ReturnType, ...) \
    static_assert(HAS_METHOD(Class, MethodName, ReturnType, __VA_ARGS__), \
        "Class does not have required method or method has wrong signature")

#define ASSERT_STATIC_METHOD(Class, MethodName, ReturnType, ...) \
    static_assert(HAS_STATIC_METHOD(Class, MethodName, ReturnType, __VA_ARGS__), \
    "Class does not have required method or method has wrong signature")