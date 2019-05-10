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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <vector>
#include <stdio.h>
#include "cudares_ff_impl.h"

const int INTERPOLATION = 24;
const int DECIMATION = 25;

namespace gr {
  namespace cudaresampler {

    cudares_ff::sptr
    cudares_ff::make()
    {
      return gnuradio::get_initial_sptr
        (new cudares_ff_impl());
    }



    /*
     * The private constructor
     */
    cudares_ff_impl::cudares_ff_impl()
      : gr::block("cudares_ff",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {

    }

    /*
     * Our virtual destructor.
     */
    cudares_ff_impl::~cudares_ff_impl()
    {
    }

    void
    cudares_ff_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items * DECIMATION / INTERPOLATION;
    }

    int
    cudares_ff_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
     const double OUT_SAMPLING_RATE = 48000; // 48kHz
      const float *in = (const float *) input_items[0]; // input item array
      float *out = (float *) output_items[0]; // output item array
      std::vector<float> samples(ninput_items[0] * (INTERPOLATION + 1)); // temp vector used for interpolation and decimation
      double cutoffFreq = 20000; // center frequency


      // Interpolation
      for(int up = 0; up < ninput_items[0]; up++)
      {
        samples[up * (INTERPOLATION + 1)] = in[up];

        for(int scale_index = 1; scale_index <= INTERPOLATION; scale_index++)
          samples[up * (INTERPOLATION + 1) + scale_index] = 0;
      }

      samples.shrink_to_fit();



      // Compute Taps
      int ntaps = 3;
      std::vector<float> taps(ntaps);

      int M = (ntaps - 1) / 2;
      double fwT0 = 2 * M_PI * cutoffFreq / OUT_SAMPLING_RATE;

      for(int n = -M; n <= M; n++)
      {
        if(n == 0)
          taps[n+M] = fwT0 / M_PI;

        else
        {
          taps[n+M] = sin(n * fwT0) / (n * M_PI);
        }
      }



      // Filter
      std::vector<float> f_samples(samples.size());
/*
      for(int f_index = 0; f_index < samples.size(); f_index++)
      {
        for(int t_index = 0; t_index < ntaps; t_index++)
        {
          if(f_index >= t_index)
            f_samples[f_index] += taps[t_index] * samples[f_index - t_index];
        }
      }
*/
cu_FIR_Filter(samples, ntaps,f_samples, taps);


      // Decimation
      for(int down = 0; down < noutput_items; down++)
      {
        out[down] = f_samples[DECIMATION * down];
      }



      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (ninput_items[0]);

      // Tell runtime system how many output items we produced.
return noutput_items;
    }

  } /* namespace cudaresampler */
} /* namespace gr */

