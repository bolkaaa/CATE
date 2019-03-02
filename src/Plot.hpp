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

using std::string;

class Plot
{
public:
    Plot(uint16_t data_size);

    ~Plot();

    /* Set the appearance of the plot. */
    void set_appearance(string x_title, string y_title, int x_min, int x_max,
                        int y_min, int y_max, string text_font, uint8_t text_size);

    /* Set value of specified index of x_data array. */
    void set_x_data(uint16_t i, double x);

    /* Set value of specified index of y_data array. */
    void set_y_data(uint16_t i, double y);

    /* Set the scale of plot's x and y axes. */
    void set_axis_scale(int x_min, int x_max, int y_min, int y_max);

    /* Given changes to x_data and y_data, replot the plot. */
    void replot();

    QwtPlot *get_plotter() { return plotter; }

private:
    QwtPlot *plotter;
    QwtPlotCurve *curve;
    QwtPlotZoomer *zoomer;
    QwtPlotGrid *grid;
    QwtPlotPanner *panner;

    uint16_t data_size;
    double *x_data;
    double *y_data;
};

#endif
