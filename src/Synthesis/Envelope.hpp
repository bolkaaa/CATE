/*
  CATE: Concatenative Audio Processing Application
  Copyright (c) 2019 Liam Wyllie. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef ENVELOPE_HPP
#define ENVELOPE_HPP

namespace CATE {

/* Envelope contains functionality for the creation of a parabolic envelope,
 * maintaining a state over its parameters for its lifespan. */
class Envelope
{
public:
    Envelope(float dur, float sustain, float sample_rate);

    /* Initialise parabolic envelope parameters. */
    void reset();

    /* Synthesize the next sample value and return it. */
    float synthesize();

private:
    float dur;
    float sustain;
    float amp;
    float curve;
    float slope;
    float sample_rate;
    float rdur;
    float rdur2;
    int sample_index;
    int sample_size;
};

} // CATE

#endif
