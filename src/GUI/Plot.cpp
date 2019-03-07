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

#include "Plot.hpp"

namespace CATE {

Plot::Plot(int data_size)
        : data_size(data_size),
          plotter(new QwtPlot()),
          zoomer(new QwtPlotZoomer(plotter->canvas(), true)),
          curve(new QwtPlotCurve()),
          grid(new QwtPlotGrid()),
          panner(new QwtPlotPanner(plotter->canvas())),
          x_data(new double[data_size]),
          y_data(new double[data_size])
{
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
    curve->attach(plotter);
}

Plot::~Plot()
{
    delete[] y_data;
    delete[] x_data;
    delete panner;
    delete grid;
    delete curve;
    delete zoomer;
    delete plotter;
}

void Plot::set_appearance(string x_title, string y_title, int x_min, int x_max,
                          int y_min, int y_max, string text_font,
                          int text_size)
{
    QwtText x_axis_title = QwtText(x_title.c_str());
    QwtText y_axis_title = QwtText(y_title.c_str());
    QwtText title = QwtText("Plot");
    x_axis_title.setFont(QFont(text_font.c_str(), text_size));
    y_axis_title.setFont(QFont(text_font.c_str(), text_size));
    title.setFont(QFont(text_font.c_str(), text_size));

    plotter->setAxisAutoScale(QwtPlot::yLeft);
    plotter->setAxisAutoScale(QwtPlot::xBottom);

    QFont axis_font = QFont(text_font.c_str(), static_cast<int> (text_size * 0.75));
    plotter->setAxisFont(QwtPlot::xBottom, axis_font);
    plotter->setAxisFont(QwtPlot::yLeft, axis_font);
    plotter->setAxisTitle(QwtPlot::xBottom, x_title.c_str());
    plotter->setAxisTitle(QwtPlot::yLeft, y_title.c_str());

    grid->setMajorPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->attach(plotter);

    plotter->setCanvasBackground(QColor(150, 150, 150));
    plotter->setGeometry(QRect(20, 20, 600, 220));
    plotter->resize(200, 200);

    plotter->setAxisScale(QwtPlot::xBottom, x_min, x_max);
    plotter->setAxisScale(QwtPlot::yLeft, y_min, y_max);

    panner->setMouseButton(Qt::MidButton);
}

void Plot::set_x_data(int i, double x)
{
    x_data[i] = x;
}

void Plot::set_y_data(int i, double y)
{
    y_data[i] = y;
}

void Plot::set_axis_scale(int x_min, int x_max, int y_min, int y_max)
{
    plotter->setAxisScale(QwtPlot::xBottom, x_min, x_max);
    plotter->setAxisScale(QwtPlot::yLeft, y_min, y_max);
}

void Plot::replot()
{
    curve->setSamples(x_data, y_data, data_size);
    plotter->replot();
}

} // CATE
