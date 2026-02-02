
#ifndef ACTION_ATA_PRODUCT
#define ACTION_ATA_PRODUCT
#include "utilities.h"
#include "STL_interface.hh"
#include <string>
#include "init/init_function.hh"
#include "init/init_vector.hh"
#include "init/init_matrix.hh"

using namespace std;

template<class Interface>
class Action_ata_product {

public :

  // Ctor

  Action_ata_product( int size ):_size(size)
  {
    MESSAGE("Action_ata_product Ctor");

    // STL matrix and vector initialization

    init_matrix<pseudo_random>(A_stl,_size);
    init_matrix<null_function>(X_stl,_size);
    init_matrix<null_function>(resu_stl,_size);

    // generic matrix and vector initialization

    Interface::matrix_from_stl(A_ref,A_stl);
    Interface::matrix_from_stl(X_ref,X_stl);

    Interface::matrix_from_stl(A,A_stl);
    Interface::matrix_from_stl(X,X_stl);

  }

  // invalidate copy ctor

  Action_ata_product( const  Action_ata_product & )
  {
    INFOS("illegal call to Action_ata_product Copy Ctor");
    exit(0);
  }

  // Dtor

  ~Action_ata_product( void ){

    MESSAGE("Action_ata_product Dtor");

    // deallocation

    Interface::free_matrix(A,_size);
    Interface::free_matrix(X,_size);

    Interface::free_matrix(A_ref,_size);
    Interface::free_matrix(X_ref,_size);

  }

  // action name

  static inline std::string name( void )
  {
    return "ata_"+Interface::name();
  }

  double nb_op_base( void ){
    return 2.0*_size*_size*_size;
  }

  inline void initialize( void ){

    Interface::copy_matrix(A_ref,A,_size);
    Interface::copy_matrix(X_ref,X,_size);

  }

  inline void calculate( void ) {

      Interface::ata_product(A,X,_size);

  }

  void check_result( void ){
    if (_size>128) return;
    // calculation check

    Interface::matrix_to_stl(X,resu_stl);

    STL_interface<typename Interface::real_type>::ata_product(A_stl,X_stl,_size);

    typename Interface::real_type error=
      STL_interface<typename Interface::real_type>::norm_diff(X_stl,resu_stl);

    if (error>1.e-6){
      INFOS("WRONG CALCULATION...residual=" << error);
      exit(1);
    }

  }

private :

  typename Interface::stl_matrix A_stl;
  typename Interface::stl_matrix X_stl;
  typename Interface::stl_matrix resu_stl;

  typename Interface::gene_matrix A_ref;
  typename Interface::gene_matrix X_ref;

  typename Interface::gene_matrix A;
  typename Interface::gene_matrix X;


  int _size;

};


#endif



