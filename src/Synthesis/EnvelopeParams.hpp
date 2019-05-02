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

#ifndef ENVELOPEPARAMS_HPP
#define ENVELOPEPARAMS_HPP

namespace CATE {

/* "Essence" for Envelope class to store parameters. */
class EnvelopeParams
{
public:
    EnvelopeParams(int sample_size);

    /* Get attack value. */
    float get_attack() const { return attack; }

    /* Get sustain value. */
    float get_sustain() const { return sustain; }

    /* Get release value. */
    float get_release() const { return release; }

    /* Get sample size value. */
    float get_sample_size() const { return sample_size; }

    /* Set attack value. */
    void set_attack(float new_attack) { attack = new_attack; }

    /* Set sustain value. */
    void set_sustain(float new_sustain) { sustain = new_sustain; }

    /* Set release value. */
    void set_release(float new_release) { release = new_release; }

    /* Set sample size value. */
    void set_sample_size(int new_sample_size) { sample_size = new_sample_size; }

private:
    int sample_size;
    float attack;
    float sustain;
    float release;
};


} // CATE

#endif
