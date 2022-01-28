#include <lizy1/type.h>
#include <type_traits>
#include <memory>
#include <atomic>
using namespace lizy1::type;


struct A {};
struct B {};
struct C {};                    // A, B, and C are complete types
struct D;
struct E;
struct F;                       // D, E, and F are incomplete types
template<class T> struct G;
template<class T> struct H;     // G and H are class templates (only declaration)


int main()
{
    /// gadgets
    {
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
    }
    /// condition
    {
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
    }
    /// calculation
    {
        /// _A (reomve cvref)
        static_assert(_s<_A<const volatile A &>, A>::value);
        /// _C (conditional)
        static_assert(_s<_C<true , A, B>, A>::value);
        static_assert(_s<_C<false, A, B>, B>::value);
        /// _D (decay)
        static_assert(_s<_D<const volatile A &>, A  >::value);
        static_assert(_s<_D<A[3]              >, A *>::value);
        /// _Ur (remove ref)
        static_assert(_s<_Ur<const volatile A & >, const volatile A>::value);
        static_assert(_s<_Ur<const volatile A &&>, const volatile A>::value);
        /// _Urr (remove rvalue reference)
        static_assert(_s<_Urr<const volatile A & >, const volatile A &>::value);
        static_assert(_s<_Urr<const volatile A &&>, const volatile A  >::value);
    }
    /// pack calculation
    {
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
    }
    /// concept
    {
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
    }
    return 0;
}
