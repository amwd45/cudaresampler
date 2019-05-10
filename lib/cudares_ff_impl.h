/* -*- c++ -*- */
/* 
 * Copyright 2019 gr-cudaresampler author.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_CUDARESAMPLER_CUDARES_FF_IMPL_H
#define INCLUDED_CUDARESAMPLER_CUDARES_FF_IMPL_H

#include <cudaresampler/cudares_ff.h>
extern void cu_FIR_Filter(std::vector<float> samples,int ntaps,std::vector<float> &f_samples, std::vector<float> taps);
namespace gr {
  namespace cudaresampler {

    class cudares_ff_impl : public cudares_ff
    {
     private:
      // Nothing to declare in this block.

     public:
      cudares_ff_impl();
      ~cudares_ff_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace cudaresampler
} // namespace gr

#endif /* INCLUDED_CUDARESAMPLER_CUDARES_FF_IMPL_H */

