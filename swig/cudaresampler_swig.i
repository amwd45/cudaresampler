/* -*- c++ -*- */

#define CUDARESAMPLER_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "cudaresampler_swig_doc.i"

%{
#include "cudaresampler/cudares_ff.h"
%}


%include "cudaresampler/cudares_ff.h"
GR_SWIG_BLOCK_MAGIC2(cudaresampler, cudares_ff);
