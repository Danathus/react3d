#ifndef RAGDOLL_LIBRARY_EXPORT_H
#define RAGDOLL_LIBRARY_EXPORT_H

////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#   ifdef DTRAGDOLL_EXPORTS
#      define RAGDOLL_LIBRARY_EXPORT __declspec(dllexport)
#   else
#      define RAGDOLL_LIBRARY_EXPORT __declspec(dllimport)
#   endif
#else
#   define RAGDOLL_LIBRARY_EXPORT
#endif

/*
Regarding MSVC warning C4251
  warning: class std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'AClass'

employing potential solution written about here:
  http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
*/

////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#  define EXPORT_RAGDOLL_REF_PTR(rtype) \
      template class RAGDOLL_LIBRARY_EXPORT dtCore::RefPtr< rtype >;
   #define EXPORT_RAGDOLL_STL_VECTOR(vectype) \
      template class RAGDOLL_LIBRARY_EXPORT std::allocator< vectype >; \
      template class RAGDOLL_LIBRARY_EXPORT std::vector< vectype, std::allocator< vectype> >;

   #define EXPORT_RAGDOLL_STL_MAP(mapkey, mapvalue)                            \
      template struct RAGDOLL_LIBRARY_EXPORT std::pair< mapkey, mapvalue >;    \
      template class RAGDOLL_LIBRARY_EXPORT std::allocator<                    \
         std::pair<const mapkey, mapvalue> >;                                  \
      template struct RAGDOLL_LIBRARY_EXPORT std::less< mapkey >;              \
      template class RAGDOLL_LIBRARY_EXPORT std::allocator<                    \
         std::_Tree_ptr<std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>, \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> > >;       \
      template class RAGDOLL_LIBRARY_EXPORT std::allocator<                    \
         std::_Tree_nod<std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>, \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> > >;       \
      template class RAGDOLL_LIBRARY_EXPORT std::_Tree_nod<                    \
         std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>,                \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> >;         \
      template class RAGDOLL_LIBRARY_EXPORT std::_Tree_ptr<                    \
         std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>,                \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> >;         \
      template class RAGDOLL_LIBRARY_EXPORT std::_Tree_val<                    \
         std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>,                \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> >;         \
      template class RAGDOLL_LIBRARY_EXPORT std::map<                          \
         mapkey, mapvalue, std::less< mapkey >,                                \
         std::allocator<std::pair<const mapkey, mapvalue> > >;
#else
#  define EXPORT_RAGDOLL_REF_PTR(rtype)
#  define EXPORT_RAGDOLL_STL_VECTOR(vectype)
#  define EXPORT_RAGDOLL_STL_MAP(mapkey, mapvalue)
#endif //Visual Studio

////////////////////////////////////////////////////////////////////////////////

#endif // RAGDOLL_LIBRARY_EXPORT_H
