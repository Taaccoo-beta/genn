/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
  
   Institute: Center for Computational Neuroscience and Robotics
              University of Sussex
              Falmer, Brighton BN1 9QJ, UK
  
   email to:  T.Nowotny@sussex.ac.uk
  
   initial version: 2015-08-19
   
--------------------------------------------------------------------------*/

#ifndef SPARSE_PROJECTION
#define SPARSE_PROJECTION

//! \brief class (struct) for defining a spars connectivity projection
struct SparseProjection{
    unsigned int *indInG;
    unsigned int *ind;
    unsigned int *preInd;
    unsigned int *revIndInG;
    unsigned int *revInd;
    unsigned int *remap;
    unsigned int connN; 
};

//! \brief class for definining a sparse connectivity
//! projection which is partitioned into N blocks
//! **THINK** could use uint8 or uint16 for sub row length
template<unsigned int N>
struct PartitionedSparseProjection
{
    unsigned int *subRowBeginIndices[N];
    unsigned int *subRowLength[N];
    unsigned int *postIndices;

    unsigned int connN;
};

#endif
