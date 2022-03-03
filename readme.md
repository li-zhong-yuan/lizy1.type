# lizy1.type

This library defines some compile-time utilities to query or modify the properties of types.

## Table of Contents

- [Package](#package)
    - [Interface lizy1.type](#interface-lizy1type)
- [Introduction & Examples](#introduction--examples)
    - [Gadgets](#gadgets)
    - [Condition](#condition)
    - [Calculation](#calculation)
    - [Pack Calculation](#pack-calculation)
    - [Concept](#concept)
- [Build & Integrate](#build--integrate)
    - [Build via CMake](#build-via-cmake)
    - [Integrate via CMake](#integrate-via-cmake)

## Package

**Package Name**: lizy1.type

**Compatibility Mode**: AnyNewerVersion

**Targets List**: lizy1.type

### Interface lizy1.type

**Features**: C++20

**Interface Dependencies**: None

## Introduction & Examples

Utilities defined by this library are divided into serveral sections. The following text lists all these sections and their examples. You can find the examples in [/test_and_example/test_and_example.cpp](https://github.com/li-zhong-yuan/lizy1.type/blob/master/test_and_example/test_and_example.cpp).

First, let's create some types and class templates for our examples,

```cpp
struct A {};
struct B {};
struct C {};                    // A, B, and C are complete types
struct D;
struct E;
struct F;                       // D, E, and F are incomplete types
template<class T> struct G;
template<class T> struct H;     // G and H are class templates (only declaration)
```

### Gadgets

This section defines gadgets. Gadgets are helper classes and class templates that form the basis of this library.

```cpp
/// _b, _f & _t (bool constant)
static_assert(!_b<false>::value);               // _b<...> is similar to std::bool_constant<...>
static_assert( _b<true >::value);
static_assert(std::is_same_v<_f, _b<false>>);   // _f is _b<false>
static_assert(std::is_same_v<_t, _b<true >>);   // _t is _b<true>
/// _i (int constant)
static_assert(_i<7>::value == 7);               // _i<...> is similar to std::integral_constant<int, ...>
/// _Is (integer pack)
using Integers = _Is<1, 2, 6, 7>;               // _Is<...> is similar to std::integer_sequence<int, ...>,
                                                // but the former is incomplete
/// _m, _mf & _mt (match result)
static_assert(!_m<false>::is_matched);          // _m<...> is similar to _b<...>,
                                                // but used by deduction classes mainly
static_assert( _m<true >::is_matched);
static_assert(std::is_same_v<_mf, _m<false>>);  // _mf is _m<false>
static_assert(std::is_same_v<_mt, _m<true >>);  // _mt is _m<true>
/// _P (type pack)
using Types = _P<A, B, C, D, E, F>;             // _P<...> is incomplete
/// _T (type wrapper)
static_assert(std::is_same_v<_T<A>::type, A>);  // _T<...> is similar to std::type_identity<...>
/// _void (empty class)
_void v;                                        // an empty class (but complete) representing void
std::make_shared<_void>();                      // ok
// std::make_shared<void>();                    // won't compile
```

### Condition

This section defines alias templates for type test. Each of these templates refers to either `_b<true>` or `_b<false>` as the test result.

```cpp
/// _a (non-cvref test)
static_assert( _a<A         >::value);
static_assert(!_a<const A   >::value);
static_assert(!_a<volatile A>::value);
static_assert(!_a<A &       >::value);
/// _c (complete test)
static_assert( _c<A    >::value);
static_assert( _c<A &  >::value);
static_assert( _c<int()>::value);
// static_assert(!_c<D>::value);    // ok, but testing an incomplete type leads to
                                    // another static assertion failure inside _c<...>
static_assert(!_c<void >::value);
/// _false (deduced false)
static_assert(!_false<A>::value);
/// _in (in test)
static_assert( _in<B, _P<A, B, C, D, E>>::value);
static_assert(!_in<F, _P<A, B, C, D, E>>::value);
static_assert( _in<_Is<6>, _Is<1, 2, 6, 7>>::value);
static_assert(!_in<_Is<5>, _Is<1, 2, 6, 7>>::value);
/// _s (same test)
static_assert( _s<A, A>::value);
static_assert(!_s<A, B>::value);
/// _tpl (template test)
static_assert( _tpl<G<A>, G>::value);
static_assert(!_tpl<G<A>, H>::value);
```

### Calculation

This section defines alias templates for non-pack calculation. **Non-Pack Calculation** is compile-time type calculation that does not involve parameter packs. Each of these templates defined in this section refers to a type as its calculation result.

```cpp
/// _A (reomve cvref)
static_assert(_s<_A<const volatile A &>, A>::value);
/// _C (conditional)
static_assert(_s<_C<true , A, B>, A>::value);
static_assert(_s<_C<false, A, B>, B>::value);
/// _D (decay)
static_assert(_s<_D<const volatile A &>, A  >::value);
static_assert(_s<_D<A[3]              >, A *>::value);
/// _Uc (remove const)
static_assert(_s<_Uc<const A>, A>::value);
/// _Ucr (remove const ref)
static_assert(_s<_Ucr<const A &>, A>::value);
/// _Ur (remove ref)
static_assert(_s<_Ur<const volatile A & >, const volatile A>::value);
static_assert(_s<_Ur<const volatile A &&>, const volatile A>::value);
/// _Urr (remove rvalue reference)
static_assert(_s<_Urr<const volatile A & >, const volatile A &>::value);
static_assert(_s<_Urr<const volatile A &&>, const volatile A  >::value);
```

### Pack Calculation

This section defines alias templates for pack calculation. **Pack Calculation** is compile-time type calculation that involves parameter packs. Each of these templates defined in this section refers to a type as its calculation result.

```cpp
/// _M (merge)
static_assert(_s<
    _M<_P<A, B, C>, _P<D, E, F>>,
    _P<A, B, C, D, E, F>
>::value);
static_assert(_s<
    _M<_Is<1, 2, 3>, _Is<7, 8, 9>>,
    _Is<1, 2, 3, 7, 8, 9>
>::value);
/// _R (reverse)
static_assert(_s<
    _R<_P<A, B, C, D, E, F>>,
    _P<F, E, D, C, B, A>
>::value);
/// _Range (integer range)
static_assert(_s<_Range<4, 9>, _Is<4, 5, 6, 7, 8>>::value);
/// _Sz (Size of Parameters)
static_assert(_Sz<std::tuple<int, int, long>>::value == 3);
```

### Concept

This section defines some concepts.

```cpp
/// Bool
static_assert( Bool<bool  >);
static_assert( Bool<int   >);
static_assert( Bool<void *>);
static_assert(!Bool<A     >);
/// Complete
static_assert( Complete<A    >);
static_assert( Complete<A &  >);
static_assert( Complete<int()>);
// static_assert(!Complete<D>);     // ok, but testing an incomplete type leads to
                                    // another static assertion failure inside Complete<...>
static_assert(!Complete<void >);
/// FunctionPrototype
static_assert( FunctionPrototype<void(int)>);
static_assert(!FunctionPrototype<int>);
/// NonCvref
static_assert( NonCvref<A         >);
static_assert(!NonCvref<const A   >);
static_assert(!NonCvref<volatile A>);
static_assert(!NonCvref<A &       >);
/// Template
static_assert( Template<G<A>, G>);
static_assert(!Template<G<A>, H>);
/// Unique
static_assert( Unique<std::unique_ptr<int>>);
static_assert( Unique<std::shared_ptr<int>>);
static_assert(!Unique<std::atomic_int>);
```

## Build & Integrate

### Build via CMake

[This section is incomplete]

### Integrate via CMake

To use this library from a CMake project, you can locate it directly with `find_package()`, after this library is properly installed and CMake paths are properly set,

```cmake
# CMakeLists.txt
find_package("lizy1.type" 0 REQUIRED)
...
add_library("foo" ...)
...
target_link_libraries("foo" PRIVATE "lizy1.type")
```
