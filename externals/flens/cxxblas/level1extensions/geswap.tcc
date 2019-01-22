/*
 *   Copyright (c) 2013, Klaus Pototzky
 *
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1) Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2) Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *   3) Neither the name of the FLENS development group nor the names of
 *      its contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CXXBLAS_LEVEL1EXTENSIONS_GESWAP_TCC
#define CXXBLAS_LEVEL1EXTENSIONS_GESWAP_TCC 1

#include <cxxstd/algorithm.h>
#include <cxxstd/cassert.h>
#include <cxxblas/cxxblas.h>

namespace cxxblas {

//
//  swap A and B
//
template <typename IndexType, typename MA, typename MB>
void
geswap(StorageOrder orderA, StorageOrder orderB,
       IndexType m, IndexType n,
       MA *A, IndexType ldA,
       MB *B, IndexType ldB)
{
    CXXBLAS_DEBUG_OUT("geswap_generic");

    if ( (orderA==ColMajor) && (orderB==ColMajor) ) {

        if ( (m==ldA) && (m==ldB) ) {

            cxxblas::swap(m*n, A, IndexType(1), B, IndexType(1));

        } else {

            for (IndexType j=0; j<n; ++j) {
                cxxblas::swap(m, A+j*ldA, IndexType(1), B+j*ldB, IndexType(1));
            }
        }

    } else if ( (orderA==RowMajor) && (orderB==RowMajor) ){

        if ( (n==ldA) && (n==ldB) ) {

            cxxblas::swap(m*n, A, IndexType(1), B, IndexType(1));

        } else {

            for (IndexType i=0; i<m; ++i) {
                cxxblas::swap(n, A+i*ldA, IndexType(1), B+i*ldB, IndexType(1));
            }
        }
    } else if ( (orderA==ColMajor) && (orderB==RowMajor) ) {

      for (IndexType i=0; i<n; ++i) {
          cxxblas::swap(m, A+i*ldA, IndexType(1), B+i, ldB);
      }

    } else if ( (orderA==RowMajor) && (orderB==ColMajor) ) {

        for (IndexType j=0; j<m; ++j) {
            cxxblas::swap(n, A+j, ldA, B+j*ldB, IndexType(1));
        }
    } else {
        ASSERT(0);
    }
}

} // namespace cxxblas

#endif // CXXBLAS_LEVEL1EXTENSIONS_GESWAP_TCC
