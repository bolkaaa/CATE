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

#ifndef PLOT_HPP
#define PLOT_HPP

#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_grid.h>

/* Uses the Qwt library from Qt to handle plotting of data. For now, just does
 * spectrogram plotting. Will implement other visualisation in the future. */

namespace CATE {

using std::string;

class Plot
{
public:
    Plot(int data_size);

    ~Plot();

    /* Set the appearance of the plot. */
    void set_appearance(string x_title, string y_title, int x_min, int x_max,
                        int y_min, int y_max, string text_font, int text_size);

    /* Set value of specified index of x_data array. */
    void set_x_data(int i, double x);

    /* Set value of specified index of y_data array. */
    void set_y_data(int i, double y);

    /* Set the scale of plot's x and y axes. */
    void set_axis_scale(int x_min, int x_max, int y_min, int y_max);

    /* Given changes to x_data and y_data, replot the plot. */
    void replot();

    QwtPlot *get_plotter()
    { return plotter; }

private:
    QwtPlot *plotter;
    QwtPlotCurve *curve;
    QwtPlotZoomer *zoomer;
    QwtPlotGrid *grid;
    QwtPlotPanner *panner;

    int data_size;
    double *x_data;
    double *y_data;
};

} // CATE

#endif
