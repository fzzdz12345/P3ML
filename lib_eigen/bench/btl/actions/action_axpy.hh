
#ifndef ACTION_AXPY
#define ACTION_AXPY
#include "utilities.h"
#include "STL_interface.hh"
#include <string>
#include "init/init_function.hh"
#include "init/init_vector.hh"
#include "init/init_matrix.hh"

using namespace std;

template<class Interface>
class Action_axpy {

public :

  // Ctor

  Action_axpy( int size ):_coef(1.0),_size(size)
  {
    MESSAGE("Action_axpy Ctor");

    // STL vector initialization

    init_vector<pseudo_random>(X_stl,_size);
    init_vector<pseudo_random>(Y_stl,_size);
    init_vector<null_function>(resu_stl,_size);

    // generic matrix and vector initialization

    Interface::vector_from_stl(X_ref,X_stl);
    Interface::vector_from_stl(Y_ref,Y_stl);

    Interface::vector_from_stl(X,X_stl);
    Interface::vector_from_stl(Y,Y_stl);


  }

  // invalidate copy ctor

  Action_axpy( const  Action_axpy & )
  {
    INFOS("illegal call to Action_axpy Copy Ctor");
    exit(1);
  }

  // Dtor

  ~Action_axpy( void ){

    MESSAGE("Action_axpy Dtor");

    // deallocation

    Interface::free_vector(X_ref);
    Interface::free_vector(Y_ref);

    Interface::free_vector(X);
    Interface::free_vector(Y);
  }

  // action name

  static inline std::string name( void )
  {
    return "axpy_"+Interface::name();
  }

  double nb_op_base( void ){
    return 2.0*_size;
  }

  inline void initialize( void ){
    Interface::copy_vector(X_ref,X,_size);
    Interface::copy_vector(Y_ref,Y,_size);
  }

  inline void calculate( void ) {
    BTL_ASM_COMMENT("mybegin axpy");
    Interface::axpy(_coef,X,Y,_size);
    BTL_ASM_COMMENT("myend axpy");
  }

  void check_result( void ){
    if (_size>128) return;
    // calculation check

    Interface::vector_to_stl(Y,resu_stl);

    STL_interface<typename Interface::real_type>::axpy(_coef,X_stl,Y_stl,_size);

    typename Interface::real_type error=
      STL_interface<typename Interface::real_type>::norm_diff(Y_stl,resu_stl);

    if (error>1.e-6){
      INFOS("WRONG CALCULATION...residual=" << error);
      exit(0);
    }

  }

private :

  typename Interface::stl_vector X_stl;
  typename Interface::stl_vector Y_stl;
  typename Interface::stl_vector resu_stl;

  typename Interface::gene_vector X_ref;
  typename Interface::gene_vector Y_ref;

  typename Interface::gene_vector X;
  typename Interface::gene_vector Y;

  typename Interface::real_type _coef;

  int _size;
};

#endif
