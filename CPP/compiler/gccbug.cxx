/*
For some reason, GNU 8.3 allows the code below. Note if the code is
compiled with -DUNUSED (removing an unused default argument) then
GNU 8.3 gives an error like all the other compilers. Trying to emulate
this GNU bug would be extremely difficult and it looks like the latest
version of GNU (9.1.0) has fixed this bug.
*/
template<typename _Tp, _Tp __v>
struct integral_constant
{
  static constexpr _Tp  value = __v;
};
  template<bool __v>
    using __bool_constant = integral_constant<bool, __v>;
 
  template<typename _B1, typename _B2>
    struct __and_ : public _B1::type
    { };
 
  template<typename _Tp>
    struct __is_referenceable
{
   static const bool value = true;
};
 
template<typename _Tp, typename... _Args>
struct is_constructible
      : public __bool_constant<__is_constructible(_Tp, _Args...)>
{
};
 
  template<typename _Tp, bool = __is_referenceable<_Tp>::value>
    struct __is_copy_constructible_impl;
 
  template<typename _Tp>
    struct __is_copy_constructible_impl<_Tp, false>
{
   static constexpr bool value = false;
};
 
  template<typename _Tp>
    struct __is_copy_constructible_impl<_Tp, true>
    : public is_constructible<_Tp, const _Tp&>
    { };
 
  template<typename _Tp>
    struct is_copy_constructible
    : public __is_copy_constructible_impl<_Tp>
    { };
 
  template<typename _Tp, typename... _Args>
    struct is_nothrow_constructible
    : public is_constructible<_Tp, _Args...>
    { };
 
template<typename _Tp>
struct is_nothrow_move_constructible
    : public is_constructible<_Tp, _Tp&&>
{ };
 
template<typename _Tp>
struct remove_const
{ typedef _Tp     type; };
 
template<typename _Tp>
struct remove_const<_Tp const>
    { typedef _Tp     type; };
 
template<typename _Tp>
struct remove_reference
{ typedef _Tp   type; };
 
template<typename _Tp>
struct remove_reference<_Tp&>
{ typedef _Tp   type; };
 
template<typename _Tp>
struct decay
{
   typedef typename remove_reference<_Tp>::type __remove_type;
   typedef typename remove_const<__remove_type>::type type;
};
 
template<bool, typename _Tp = void> struct enable_if { };
template<typename _Tp> struct enable_if<true, _Tp> { typedef _Tp type; };
template<bool _Cond, typename _Iftrue, typename _Iffalse>
struct conditional { typedef _Iftrue type; };
 
template<typename _Iftrue, typename _Iffalse>
struct conditional<false, _Iftrue, _Iffalse>
    { typedef _Iffalse type; };
 
template<bool _Cond, typename _Iftrue, typename _Iffalse>
    using conditional_t = typename conditional<_Cond, _Iftrue, _Iffalse>::type;
 
class any
{
    template<typename _Tp,
             typename _Safe = is_nothrow_move_constructible<_Tp> >
      using _Internal =  integral_constant<bool, _Safe::value>;
 
    template<typename _Tp>
      struct _Manager_internal;
 
    template<typename _Tp>
      struct _Manager_external;
 
    template<typename _Tp>
      using _Manager =
conditional_t<_Internal<_Tp>::value,
                                    _Manager_internal<_Tp>,
                                    _Manager_external<_Tp>>;
 
    template<typename _Tp, typename _Decayed = typename decay<_Tp>::type>
      using _Decay = typename enable_if<true, _Decayed>::type;
 
    constexpr any() {}
    template <typename _Res, typename _Tp, typename... _Args>
    using __any_constructible =
      enable_if<__and_<is_copy_constructible<_Tp>,
                       is_constructible<_Tp, _Args...>>::value,
                 _Res>;
 
    template <typename _Tp, typename... _Args>
    using __any_constructible_t =
      typename __any_constructible<bool, _Tp, _Args...>::type;
   
    template <typename _ValueType,
              typename _Tp = _Decay<_ValueType>,
#ifndef UNUSED
              typename _Mgr = _Manager<_Tp>,
#endif
              __any_constructible_t<_Tp, _ValueType&&> = true
                   >
      any(_ValueType&& __value);
 
template <class T> struct _Manager_internal {};
template <class T> struct _Manager_external {};
};
 
template<typename _Tp>
struct reference_wrapper
{
   reference_wrapper(_Tp&&) = delete;
};
 
void foo(reference_wrapper< any > ref1 ) {
   //reference_wrapper<any > ref2(ref1);
   reference_wrapper<any > ref2 = ref1;
}
