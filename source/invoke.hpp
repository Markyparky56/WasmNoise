#pragma once
// RemoveReference
template< typename T > struct RemoveReference { typedef T Type; };
template< typename T > struct RemoveReference<T&> { typedef T Type; };
template< typename T > struct RemoveReference<T&&> { typedef T Type; };

// MoveTemp
template< typename T >
WN_INLINE typename RemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
    return (typename RemoveReference<T>::Type&&)Obj;
}

// Forward
template< typename T>
WN_INLINE T&& Forward(typename RemoveReference<T>::Type& Obj)
{
    return (T&&)Obj;
}

template< typename T>
WN_INLINE T&& Forward(typename RemoveReference<T>::Type&& Obj)
{
    return (T&&)Obj;
}

// Thank you to @TartanLlama for these cool invoke functions
template <class F, class... Args>
WN_INLINE auto invoke(F&& f, Args&&... args) ->
  decltype(Forward<F>(f)(Forward<Args>(args)...)) 
{
  return Forward<F>(f)(Forward<Args>(args)...);
}

template <class T, class Ret, class Arg>
WN_INLINE auto invoke(Ret T::*func, Arg&& arg) ->
  decltype(Forward<Arg>(arg).*func) 
{
  return Forward<Arg>(arg).*func;
}

template <class T, class Ret, class... Params, class U, class... Args>
WN_INLINE auto invoke(Ret (T::*func)(Params...), U&& u, Args&&... args) ->
  decltype((Forward<U>(u)).*func(Forward<Args>(args)...)) 
{
  return (Forward<U>(u).*func)(Forward<Args>(args)...);
}

template <class T, class Ret, class... Params, class U, class... Args>
WN_INLINE auto invoke(Ret (T::*func)(Params...) const, U&& u, Args&&... args) ->
  decltype((Forward<U>(u).*func)(Forward<Args>(args)...))
{
  return (Forward<U>(u).*func)(Forward<Args>(args)...);
}

// template <class T, class Ret, class Arg>
// auto invoke(const Ret T::*func, Arg&& arg) ->
//   decltype(Forward<Arg>(arg).*func) 
// {
//   return Forward<Arg>(arg).*func;
// }

// template <class T, class Ret, class... Params, class U, class... Args>
// auto invoke(Ret (T::*func)(Params...) const, U&& u, Args&&... args) ->
//   decltype((Forward<U>(u)).*func(Forward<Args>(args)...)) 
// {
//   return (Forward<U>(u).*func)(Forward<Args>(args)...);
// }
