# Install script for directory: /home/fzz/SSTNN/lib_eigen/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/AdolcForward"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/AlignedVector3"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/ArpackSupport"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/AutoDiff"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/BVH"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/EulerAngles"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/FFT"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/IterativeSolvers"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/KroneckerProduct"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/LevenbergMarquardt"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/MatrixFunctions"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/MoreVectorization"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/MPRealSupport"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/NonLinearOptimization"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/NumericalDiff"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/OpenGLSupport"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/Polynomials"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/Skyline"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/SparseExtra"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/SpecialFunctions"
    "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/Splines"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "/home/fzz/SSTNN/lib_eigen/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/fzz/SSTNN/lib_eigen/build/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

