#ifndef REACT3D_CORE_LIBRARY_EXPORT_H
#define REACT3D_CORE_LIBRARY_EXPORT_H

////////////////////////////////////////////////////////////////////////////////

#if !defined(USE_STATIC_LINKING) && (defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__))
#   ifdef REACT3D_CORE_EXPORTS
#      define REACT3D_CORE_LIBRARY_EXPORT __declspec(dllexport)
#   else
#      define REACT3D_CORE_LIBRARY_EXPORT __declspec(dllimport)
#   endif
#else
#   define REACT3D_CORE_LIBRARY_EXPORT
#endif

/*
Regarding MSVC warning C4251
  warning: class std::vector<_Ty>' needs to have dll-interface to be used by clients of class 'AClass'

employing potential solution written about here:
  http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
*/

////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#  define EXPORT_REACT3D_CORE_REF_PTR(rtype) \
      template class REACT3D_CORE_LIBRARY_EXPORT dtCore::RefPtr< rtype >;
   #define EXPORT_REACT3D_CORE_STL_VECTOR(vectype) \
      template class REACT3D_CORE_LIBRARY_EXPORT std::allocator< vectype >; \
      template class REACT3D_CORE_LIBRARY_EXPORT std::vector< vectype, std::allocator< vectype> >;

   #define EXPORT_REACT3D_CORE_STL_MAP(mapkey, mapvalue)                            \
      template struct REACT3D_CORE_LIBRARY_EXPORT std::pair< mapkey, mapvalue >;    \
      template class REACT3D_CORE_LIBRARY_EXPORT std::allocator<                    \
         std::pair<const mapkey, mapvalue> >;                                  \
      template struct REACT3D_CORE_LIBRARY_EXPORT std::less< mapkey >;              \
      template class REACT3D_CORE_LIBRARY_EXPORT std::allocator<                    \
         std::_Tree_ptr<std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>, \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> > >;       \
      template class REACT3D_CORE_LIBRARY_EXPORT std::allocator<                    \
         std::_Tree_nod<std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>, \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> > >;       \
      template class REACT3D_CORE_LIBRARY_EXPORT std::_Tree_nod<                    \
         std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>,                \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> >;         \
      template class REACT3D_CORE_LIBRARY_EXPORT std::_Tree_ptr<                    \
         std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>,                \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> >;         \
      template class REACT3D_CORE_LIBRARY_EXPORT std::_Tree_val<                    \
         std::_Tmap_traits<mapkey, mapvalue, std::less<mapkey>,                \
         std::allocator<std::pair<const mapkey, mapvalue> >, false> >;         \
      template class REACT3D_CORE_LIBRARY_EXPORT std::map<                          \
         mapkey, mapvalue, std::less< mapkey >,                                \
         std::allocator<std::pair<const mapkey, mapvalue> > >;
#else
#  define EXPORT_REACT3D_CORE_REF_PTR(rtype)
#  define EXPORT_REACT3D_CORE_STL_VECTOR(vectype)
#  define EXPORT_REACT3D_CORE_STL_MAP(mapkey, mapvalue)
#endif // Visual Studio

////////////////////////////////////////////////////////////////////////////////

#endif // REACT3D_CORE_LIBRARY_EXPORT_H
