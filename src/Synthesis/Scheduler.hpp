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

/* Maintains for activating each grain, based upon onset times and durations
 * supplied by a SequenceStrategy object. It handles grain allocation and
 * mixing of grains to form a single output. */

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "SequenceStrategy.hpp"

class Scheduler
{
public:
    void synthesize();

private:
    SequenceStrategy sequence_strategy;
};

#endif
