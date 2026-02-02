# Install script for directory: /home/fzz/SSTNN/lib_eigen/Eigen

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE FILE FILES
    "/home/fzz/SSTNN/lib_eigen/Eigen/Cholesky"
    "/home/fzz/SSTNN/lib_eigen/Eigen/CholmodSupport"
    "/home/fzz/SSTNN/lib_eigen/Eigen/Core"
    "/home/fzz/SSTNN/lib_eigen/Eigen/Dense"
    "/home/fzz/SSTNN/lib_eigen/Eigen/Eigen"
    "/home/fzz/SSTNN/lib_eigen/Eigen/Eigenvalues"
    "/home/fzz/SSTNN/lib_eigen/Eigen/Geometry"
    "/home/fzz/SSTNN/lib_eigen/Eigen/Householder"
    "/home/fzz/SSTNN/lib_eigen/Eigen/IterativeLinearSolvers"
    "/home/fzz/SSTNN/lib_eigen/Eigen/Jacobi"
    "/home/fzz/SSTNN/lib_eigen/Eigen/LU"
    "/home/fzz/SSTNN/lib_eigen/Eigen/MetisSupport"
    "/home/fzz/SSTNN/lib_eigen/Eigen/OrderingMethods"
    "/home/fzz/SSTNN/lib_eigen/Eigen/PaStiXSupport"
    "/home/fzz/SSTNN/lib_eigen/Eigen/PardisoSupport"
    "/home/fzz/SSTNN/lib_eigen/Eigen/QR"
    "/home/fzz/SSTNN/lib_eigen/Eigen/QtAlignedMalloc"
    "/home/fzz/SSTNN/lib_eigen/Eigen/SPQRSupport"
    "/home/fzz/SSTNN/lib_eigen/Eigen/SVD"
    "/home/fzz/SSTNN/lib_eigen/Eigen/Sparse"
    "/home/fzz/SSTNN/lib_eigen/Eigen/SparseCholesky"
    "/home/fzz/SSTNN/lib_eigen/Eigen/SparseCore"
    "/home/fzz/SSTNN/lib_eigen/Eigen/SparseLU"
    "/home/fzz/SSTNN/lib_eigen/Eigen/SparseQR"
    "/home/fzz/SSTNN/lib_eigen/Eigen/StdDeque"
    "/home/fzz/SSTNN/lib_eigen/Eigen/StdList"
    "/home/fzz/SSTNN/lib_eigen/Eigen/StdVector"
    "/home/fzz/SSTNN/lib_eigen/Eigen/SuperLUSupport"
    "/home/fzz/SSTNN/lib_eigen/Eigen/UmfPackSupport"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "/home/fzz/SSTNN/lib_eigen/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

