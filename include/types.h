#pragma once

#include "EigenInclude.h"
#include <vector>
#include <utility>

namespace UVLM
{
    namespace Types
    {
        // Working precision
        typedef double Real;

        // Eigen shortcuts
        // Matrices
        typedef Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> MatrixX;
        typedef Eigen::Map<MatrixX> MapMatrixX;
        typedef Eigen::Map<const MatrixX> cMapMatrixX;
        typedef std::vector<MatrixX> VecMatrixX;
        typedef std::vector<VecMatrixX> VecVecMatrixX;

        typedef Eigen::DenseBase<Real> DenseBase;

        // Vectors
        typedef Eigen::Matrix<Real, 3, 1> Vector3;

        // std custom containers
        typedef std::pair<unsigned int, unsigned int> IntPair;
        typedef std::vector<IntPair> VecDimensions;


        struct VMopts {
        	bool ImageMethod;
        	unsigned int Mstar;
        	bool Steady;
        	bool KJMeth;
        	bool NewAIC;
        	double DelTime;
        	bool Rollup;
        	unsigned int NumCores;
        	unsigned int NumSurfaces;
        };

        template <typename t_mat>
        inline void generate_dimensions
        (
            const t_mat& mat,
            UVLM::Types::VecDimensions& dimensions
        )
        {
            dimensions.resize(mat.size());
            for (unsigned int i_surf=0; i_surf<dimensions.size(); ++i_surf)
            {
                dimensions[i_surf] = UVLM::Types::IntPair
                                                    (
                                                        mat[i_surf][0].rows(),
                                                        mat[i_surf][0].cols()
                                                    );
            }
        }


        inline void allocate_VecMat
        (
            UVLM::Types::VecMatrixX& mat,
            const UVLM::Types::VecDimensions& dimensions,
            const int& correction = 0,
            const UVLM::Types::Real& initial_value = 0.0
        )
        {
            unsigned int n_surf = dimensions.size();
            for (unsigned int i_surf=0; i_surf<n_surf; ++i_surf)
            {
                int M = dimensions[i_surf].first + correction;
                int N = dimensions[i_surf].second + correction;
                mat.push_back(UVLM::Types::MatrixX());
                if (initial_value == 0.0)
                {
                    mat[i_surf].setZero(M,N);
                } else if (initial_value == 1.0)
                {
                    mat[i_surf].setOnes(M,N);
                } else
                {
                    mat[i_surf].setConstant(M,N,initial_value);
                }


            }
        }



        inline void allocate_VecVecMat
        (
            UVLM::Types::VecVecMatrixX& mat,
            const int& n_surf,
            const int& n_dim,
            const int& M,
            const int& N
        )
        {
            mat.resize(n_surf);
            for (auto& surf: mat)
            {
                surf.resize(n_dim);
                for (unsigned int i_dim=0; i_dim<n_dim; ++i_dim)
                {
                    surf.push_back(UVLM::Types::MatrixX());
                    surf[i_dim].resize(M, N);
                    surf[i_dim].setZero(M, N);
                }
            }
        }

        inline void allocate_VecVecMat
        (
            UVLM::Types::VecVecMatrixX& mat,
            const int& n_dim,
            const UVLM::Types::VecDimensions& dimensions,
            const int& correction = 0
        )
        {
            unsigned int n_surf = dimensions.size();
            mat.resize(n_surf);
            for (unsigned int i_surf=0; i_surf<n_surf; ++i_surf)
            {
                int M = dimensions[i_surf].first + correction;
                int N = dimensions[i_surf].second + correction;
                mat[i_surf].resize(n_dim);
                for (unsigned int i_dim=0; i_dim<n_dim; ++i_dim)
                {
                    mat[i_surf].push_back(UVLM::Types::MatrixX());
                    mat[i_surf][i_dim].resize(M, N);
                    mat[i_surf][i_dim].setZero(M, N);
                }
            }
        }

        inline void allocate_VecVecMat
        (
            UVLM::Types::VecVecMatrixX& mat,
            const UVLM::Types::VecVecMatrixX& in_dimensions,
            const int& correction = 0
        )
        {
            unsigned int n_surf = in_dimensions.size();
            mat.resize(n_surf);
            for (unsigned int i_surf=0; i_surf<n_surf; ++i_surf)
            {
                const unsigned int M = in_dimensions[i_surf][0].rows() + correction;
                const unsigned int N = in_dimensions[i_surf][0].cols() + correction;
                mat[i_surf].resize(in_dimensions[i_surf].size());
                for (unsigned int i_dim=0; i_dim<in_dimensions[i_surf].size(); ++i_dim)
                {
                    mat[i_surf].push_back(UVLM::Types::MatrixX());
                    mat[i_surf][i_dim].resize(M, N);
                    mat[i_surf][i_dim].setZero(M, N);
                }
            }
        }
    }
}

#include "typeutils.h"