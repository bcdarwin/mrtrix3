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

#ifndef __registration_mean_squared_metric_h__
#define __registration_mean_squared_metric_h__

namespace MR
{
  namespace Registration
  {
    namespace Metric
    {
      class MeanSquared {
        public:
          MeanSquared () { }

          template <class Param>
          float operator() (Param& param, Math::Vector<float>& gradient) {
//              gradient += my_gradient_at_this_point();
              return param.fixed_image.value() - param.moving_image.value();
          }
      };
    }
  }
}
#endif
