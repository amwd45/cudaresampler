#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2019 gr-cudaresampler author.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import cudaresampler_swig as cudaresampler

class qa_cudares_ff (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_cudares_ff (self):
       # set up fg
        src_data = (-3, 5, 12, 1)
        expected_result = (0.966, -0.225, 0.9)
        src = blocks.vector_source_f(src_data)
        fil = cudaresampler.cudares_ff()
        dst = blocks.vector_sink_f()
        self.tb.connect(src, fil)
        self.tb.connect(fil, dst)
        self.tb.run()

        # check data
        result_data = dst.data()
        self.assertFloatTuplesAlmostEqual(expected_result, result_data, 7)


if __name__ == '__main__':
    gr_unittest.run(qa_cudares_ff, "qa_cudares_ff.xml")
