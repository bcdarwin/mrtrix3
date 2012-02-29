/*
   Copyright 2012 Brain Research Institute, Melbourne, Australia

   Written by David Raffelt, 24/02/2012

   This file is part of MRtrix.

   MRtrix is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   MRtrix is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MRtrix.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef __registration_transform_affine_h__
#define __registration_transform_affine_h__

#include "math/vector.h"
#include "math/matrix.h"

using namespace MR::Math;

namespace MR
{
  namespace Registration
  {
    namespace Transform
    {

      /** \addtogroup Transforms
      @{ */

      /*! A 3D affine transformation class for registration.
       *
       * This class supports the ability to define the centre of rotation.
       * This should be set prior to commencing registration based on the centre of the target image.
       * The translation also should be initialised as moving image centre minus the target image centre.
       *
       */
      template <typename T = float>
      class Affine  {
        public:
          Affine () :
            param_vector_(12),
            matrix_(3,3),
            translation_(3),
            centre_(3),
            offset_(3){
            matrix_.identity();
          }

          template <class PointType>
          inline void transform (const PointType& in, PointType& out) {
              Math::mult(out, matrix_, in);
              out += offset_;
          }

          template <class PointType>
          inline void get_jacobian_with_respect_to_params (const PointType& p, Matrix<T>& jacobian) {
            jacobian.resize(3,12);
            jacobian.zero();
            Vector<T> v = p - centre_;
            size_t blockOffset = 0;
            for (size_t block = 0; block < 3; block++) {
              for (size_t dim = 0; dim < 3; dim++)
                jacobian(block, blockOffset + dim) = v[dim];
              blockOffset += 3;
            }
            for(size_t dim = 0; dim < 3; dim++) {
              jacobian(dim, blockOffset + dim) = 1.0;
            }
          }

          void set_transform (Matrix<T>& transform) {
            for (size_t row = 0; row < 3; row++) {
              for (size_t col = 0; col < 3; col++)
                matrix_(row, col) = transform(row, col);
              translation_[row] = transform(row, 3);
            }
            compute_offset();
          }

          Matrix<T> get_transform () {
            Matrix<T> transform(4,4);
            transform.identity();
            for (size_t row = 0; row < 3; row++) {
              for (size_t col = 0; col < 3; col++)
                transform(row,col) = matrix_(row,col);
              transform(row, 3) = offset_[row];
            }
            return transform;
          }

          void set_parameter_vector (Math::Vector<T>& param_vector) {
            param_vector_ = param_vector;
            size_t index = 0;
            for (size_t row = 0; row < 3; row++) {
              for (size_t col = 0; col < 3; col++)
                matrix_(row, col) = param_vector[index++];
            }
            for (size_t dim = 0; dim < 3; dim++)
              translation_[dim] = param_vector[index++];
            compute_offset();
          }

          Vector<T>& get_parameter_vector () {
            size_t index = 0;
            for (size_t row = 0; row < 3; row++) {
              for (size_t col = 0; col < 3; col++)
                param_vector_[index++] = matrix_(row, col);
            }
            for (size_t dim = 0; dim < 3; dim++)
              param_vector_[index++] = translation_[dim];
            return param_vector_;
          }

          void set_matrix (Matrix<T>& matrix) {
            matrix_ = matrix;
            compute_offset();
          }

          Matrix<T>& get_matrix () {
            return matrix_;
          }

          void set_translation (Vector<T>& translation) {
            translation_ = translation;
            compute_offset();
          }

          Vector<T>& get_translation () {
            return translation_;
          }

          void set_centre(Vector<T>& centre) {
            centre_ = centre;
            compute_offset();
          }

          Vector<T>& get_centre () {
            return centre_;
          }

          void compute_offset () {
            for( size_t i = 0; i < 3; i++ ) {
              offset_[i] = translation_[i] + centre_[i];
              for( size_t j = 0; j < 3; j++ )
                offset_[i] -= matrix_(i, j) * centre_[j];
            }
          }

          size_t size() {
            return param_vector_.size();
          }

        protected:
          Vector<T> param_vector_;
          Matrix<T> matrix_;
          Vector<T> translation_;
          Vector<T> centre_;
          Vector<T> offset_;

      };
      //! @}
    }
  }
}

#endif
