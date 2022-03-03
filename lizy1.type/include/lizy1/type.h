#pragma once


/*
Lizy1 - Type
Compile-time type information

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
Copyright (c) 2021-2022 Li Zhongyuan
SPDX-License-Identifier: MIT

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <concepts>
#include <utility>
#include <type_traits>
namespace lizy1::type {
}


namespace lizy1::type::impl_type {  // this namespace and things defined in it, are for this lib's implementation only
    using namespace std;
}


/// Gadgets
namespace lizy1::type {


    /// _b, _f & _t (Bool Constant)
    template<bool b> struct _b :std::bool_constant<b> {};
    using _f = _b<false>;
    using _t = _b<true>;


    /// _i (Int Constant)
    template<int i> struct _i :std::integral_constant<int, i> {};


    /// _Is (Integer Pack)
    template<int... is> struct _Is;


    /// _m (Match Result)
    template<bool b> struct _m { static constexpr bool is_matched = b; };
    using _mf = _m<false>;
    using _mt = _m<true>;


    /// _P (Type Pack)
    template<class... Ts> struct _P;


    /// _T (Type Wrapper)
    template<class T> struct _T { using type = T; };


    /// _void (Empty Class)
    struct _void {};


}


/// Condition
namespace lizy1::type {


    /// _a (Non-Cvref Test)
    template<class T> using _a = _b<std::is_same_v<T, std::remove_cvref_t<T>>>;


    namespace impl_type {
        struct __dummy;
        template<class T, class = decltype(sizeof(T))> constexpr bool __c(int) { return true; }
        template<class T, enable_if_t<is_function_v<T>, int> = 0> constexpr bool __c(int) { return true; }
        template<class T, enable_if_t<is_same_v<T, void>, int> = 0> constexpr bool __c(int) { return false; }
        template<class T> constexpr bool __c(...) { \
            static_assert(is_same_v<__dummy, T>, "Require T is complete"); return false; }
    }


    /// _c (Complete Test)
    //  note static_assert(false) on test fail, except testing the type "void"
    template<class T> using _c = _b<impl_type::__c<T>(0)>;


    /// _false (Deduced False)
    template<class T> using _false = _b<std::is_same_v<impl_type::__dummy, T>>;


    namespace impl_type {
        template<class T, class P> struct _in :_f {};
        template<class T, template<class...> class C, class U, class... Us> struct _in<T, C<U, Us...>> :_in<T, C<Us...>> {};
        template<class T, template<class...> class C,          class... Us> struct _in<T, C<T, Us...>> :_t {};
    }
    namespace impl_type {
        template<int i, int... j> struct _in2 :_f {};
        template<int i, int j, int... k> struct _in2<i, j, k...> :_in2<i, k...> {};
        template<int i,        int... k> struct _in2<i, i, k...> :_t {};
        template<template<int> class C, template<int...> class D, int i, int... j> struct _in<C<i>, D<j...>> :_in2<i, j...> {};
    }


    /// _in (In Test)
    template<class T, class P> using _in = _b<impl_type::_in<T, P>::value>;


    /// _s (Same Test)
    template<class T, class U> using _s = _b<std::is_same_v<T, U>>;


    namespace impl_type {
        template<class    T , template<class...> class D> struct __tpl              :type::_f {};
        template<class... Ts, template<class...> class D> struct __tpl<D<Ts...>, D> :type::_t {};
    }


    /// _tpl (Template Test)
    // Is T an instance of class template D ?
    template<class T, template<class...> class D> using _tpl = _b<impl_type::__tpl<T, D>::value>;


}


/// Calcuation
namespace lizy1::type {


    /// _A (Remove Cvref)
    template<class T> using _A = std::remove_cvref_t<T>;


    /// _C (Conditional)
    template<bool b, class T, class U> using _C = std::conditional_t<b, T, U>;


    /// _D (Decay)
    template<class T> using _D = std::decay_t<T>;


    /// _Uc (Remove Const)
    template<class T> using _Uc = std::remove_const_t<T>;


    /// _Ucr (Remove Const Ref)
    template<class T> using _Ucr = std::remove_const_t<std::remove_reference_t<T>>;


    /// _Ur (Remove Ref)
    template<class T> using _Ur = std::remove_reference_t<T>;


    namespace impl_type {
        template<class T> struct __Urr       :_T<T> {};
        template<class T> struct __Urr<T &&> :_T<T> {};
    }


    // _Urr (Remove Rvalue Reference)
    template<class T> using _Urr = typename impl_type::__Urr<T>::type;


}


/// Pack Calculation
namespace lizy1::type {


    namespace impl_type {
        template<class P, class Q> struct __M;
        template<template<class...> class P, class... Ts, template<class...> class Q, class... Us> \
            struct __M<P<Ts...>, Q<Us...>> { using type = P<Ts..., Us...>; };
        template<template<int  ...> class P, int  ... is, template<int  ...> class Q, int  ... js> \
            struct __M<P<is...>, Q<js...>> { using type = P<is..., js...>; };
    }


    /// _M (Merge)
    template<class P, class Q> using _M = typename impl_type::__M<P, Q>::type;


    namespace impl_type {
        template<class P, class Q = _P<>> struct __R2;
        template<class... Qs> struct __R2<_P<>, _P<Qs...>> { using type = _P<Qs...>; };
        template<class P, class... Ps, class... Qs> struct __R2<_P<P, Ps...>, _P<Qs...>> :__R2<_P<Ps...>, _P<P, Qs...>> {};
    }
    namespace impl_type {
        template<class P, class Q = void> struct __R;
        template<template<class...> class C, class... Ps, class... Qs> \
            struct __R<C<Ps...>, _P<Qs...>> { using type = C<Qs...>; };
        template<template<class...> class C, class... Ps>              \
            struct __R<C<Ps...>, void> :__R<C<Ps...>, typename __R2<_P<Ps...>>::type> {};
    }


    /// _R (Reverse)
    template<class P> using _R = typename impl_type::__R<P>::type;


    namespace impl_type {
        template<int i, int j, class  R > struct __Range;
        template<int i,        int... rs> struct __Range<i, i, _Is<rs...>> { using type = _Is<rs...>; };
        template<int i, int j, int... rs> struct __Range<i, j, _Is<rs...>> :__Range<i + 1, j, _Is<rs..., i>> {};
    }


    /// _Range (Integer Range)
    template<int i, int j> using _Range = typename impl_type::__Range<i, j, _Is<>>::type;


    namespace impl_type {
        template<class P> struct __Sz;
        template<template<class...> class C, class... Ts> struct __Sz<C<Ts...>> :_i<sizeof...(Ts)> {};
        template<template<int...> class C, int... is> struct __Sz<C<is...>> :_i<sizeof...(is)> {};
    }


    /// _Sz (Size of Parameters)
    template<class P> using _Sz = _i<impl_type::__Sz<P>::value>;


}


/// Sfinae
namespace lizy1::type {


    /// _If (Enable If)
    template<bool b, typename R = int> using _If = std::enable_if_t<b, R>;


    /// _Inpl (Enable If Constructible)
    //  note "Inpl" stands for "inplace"
    template<class T, class... Ts> using _Inpl = _If<std::is_constructible_v<T, Ts...>>;


}


/// Concept
namespace lizy1::type {


    /// Bool
    //  require bool is no throw constructible from T
    template<class T> concept Bool = requires { { bool(std::declval<T>()) } noexcept; };


    /// Complete
    //  require T is complete
    //  note static_assert(false) on test fail, except testing the type "void"
    template<class T> concept Complete = _c<T>::value;


    namespace impl_type {
        template<class Fn> struct _FunctionPrototype :_f {};
        template<class Ret, class... Params> struct _FunctionPrototype<Ret(Params...)> :_t {};
    }


    /// FunctionPrototype
    //  require Fn can be written as Ret(Params...)
    template<class Fn> concept FunctionPrototype = impl_type::_FunctionPrototype<Fn>::value;


    /// NonCvref
    //  require T is non-cvref
    template<class T> concept NonCvref = _a<T>::value;


    /// Template
    //  require T is an instance of class template D
    template<class T, template<class...> class D> concept Template = _tpl<T, D>::value;


    /// Unique
    //  note T is some type like std::unique_ptr<...>
    template<class T> concept Unique =
        Complete<T>                                     &&
        NonCvref<T>                                     &&
        std::is_nothrow_default_constructible<T>::value &&
        std::is_nothrow_move_constructible<T>::value    &&
        std::is_destructible<T>::value;


}
