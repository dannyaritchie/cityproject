import pandas as pd
import numpy as np
#from bokeh.plotting import figure
#from bokeh.io import output_notebook, show, output_file, export_png
#from bokeh.models import ColumnDataSource, HoverTool, Panel
#from bokeh.models.widgets import Tabs
from scipy.stats import gaussian_kde
import matplotlib.pyplot as plt

plt.style.use('seaborn-white')

class BokehHistogram():

    def __init__(self, colors=["SteelBlue", "Tan"], height=600, width=600):
            self.colors = colors
            self.height = height
            self.width = width
'''
    def hist_hover(self, dataframe, column, bins=30, log_scale=False, show_plot=True, vrange=None):

        # build histogram data with Numpy
        hist, edges = np.histogram(dataframe[column], bins = bins, range=vrange)
        # dataframe[column] will return a series corresponding to the column in dataframe specified by column. (or a value if dataframe is a series).
        # hist will be of type array corresponding to value of histogram, edges will ColumnDataSourceColumnDataSourcebe bin edges. 
        # (Each element in hist will give number of elements in dataframe[column] between correspondingelement and element + 1 in edges
        
        hist_df = pd.DataFrame({column: hist,
                                 "left": edges[:-1],
                                 "right": edges[1:]})
        # uses a dictionary to specify a data frame 
        hist_df["interval"] = ["%d to %d" % (left, right) for left, 
                               right in zip(hist_df["left"], hist_df["right"])]
        # creates a new column called interval desribing interval

        # bokeh histogram with hover tool
        if log_scale == True:
            hist_df["log"] = np.log(hist_df[column])
            src = ColumnDataSource(hist_df)
            plot = figure(plot_height = self.height, plot_width = self.width,
                  title = "Histogram of {}".format(column.capitalize()),
                  x_axis_label = column.capitalize(),
                  y_axis_label = "Log Count")    
            plot.quad(bottom = 0, top = "log",left = "left", 
                right = "right", source = src, fill_color = self.colors[0], 
                line_color = "black", fill_alpha = 0.7,
                hover_fill_alpha = 1.0, hover_fill_color = self.colors[1])
        else:
            src = ColumnDataSource(hist_df)
            plot = figure(plot_height = self.height, plot_width = self.width,
                  title = "Histogram of {}".format(column.capitalize()),
                  x_axis_label = column.capitalize(),
                  y_axis_label = "Count")    
            #creates a bokah figure object
            plot.quad(bottom = 0, top = column,left = "left", 
                right = "right", source = src, fill_color = self.colors[0], 
                line_color = "black", fill_alpha = 0.7,
                hover_fill_alpha = 1.0, hover_fill_color = self.colors[1])
            #puts data on canvas

        # hover tool
        hover = HoverTool(tooltips = [('Interval', '@interval'),
                                  ('Count', '@column')])
        plot.add_tools(hover)
        # output
        if show_plot == True:
            show(plot)
        else:
            return plot

    def histotabs(self, dataframe, features, log_scale=False, show_plot=False):
        #for every elemnent in features select the column of dataframe corresponding to it, 
        #plot a histogram of it, and store all of these in a panel, tabs will then allow to switch between them  
        hists = []
        for f in features:
            h = self.hist_hover(dataframe, f, log_scale=log_scale, show_plot=show_plot)
            p = Panel(child=h, title=f.capitalize())
            hists.append(p)
        t = Tabs(tabs=hists)
        show(t)

    def filtered_histotabs(self, dataframe, feature, filter_feature, log_scale=False,show_plot=False):
        hists = []
        for col in dataframe[filter_feature].unique():
            # dataframe[filter_feature].unique will return a list of unique elemnts in the column of dataframe specified by the filter_feature
            # for every one of these elemnts find all rows in the data frame that has that element, and plot a histogram for the specified feature for these rows
            #tldr obtain a histogram of feature for every subset dataframe specified by dataframe[filter_feature] being unique
            #like before createapanelof these

            sub_df = dataframe[dataframe[filter_feature] == col]
            histo = self.hist_hover(sub_df, feature, log_scale=log_scale, show_plot=show_plot)
            p = Panel(child = histo, title=str(col))
            hists.append(p)
        t = Tabs(tabs=hists)
        show(t)

    def ratio_histo(self, dataframe, feature, filter_feature, numerator, denominator, log_scale=False,show_plot=True):
        sub_df = dataframe[dataframe['Phase Type'] == 'No Possession Change']
        hist, edges = np.histogram(sub_df['Pressure'], bins = 10,range=(0,50))
        sub_df = dataframe[dataframe['Phase Type'] == 'Possession Change']
        histb, edges = np.histogram(sub_df['Pressure'], bins = edges,range=(0,40))
        hist = np.add(hist,histb)
        for i,el in enumerate(hist):
            if el == 0:
                hist[i] = 1
        print(hist)
        histc = np.divide(histb,hist)
        hist_df = pd.DataFrame({'Pressure': histc,
                                 "left": edges[:-1],
                                 "right": edges[1:]})
        hist_df["interval"] = ["%d to %d" % (left, right) for left, 
                               right in zip(hist_df["left"], hist_df["right"])]
        src = ColumnDataSource(hist_df)
        plot = figure(plot_height = self.height, plot_width = self.width,
              title = "Histogram of pressure",
              x_axis_label = 'Pressure',
              y_axis_label = "Count")    
        #creates a bokah figure object
        plot.quad(bottom = 0, top = 'Pressure',left = "left", 
            right = "right", source = src, fill_color = self.colors[0], 
            line_color = "black", fill_alpha = 0.7,
            hover_fill_alpha = 1.0, hover_fill_color = self.colors[1])
        #puts data on canvas

        # hover tool
        hover = HoverTool(tooltips = [('Interval', '@interval'),
                                  ('Count', '@pressure')])
        plot.add_tools(hover)
        # output
        if show_plot == True:
            show(plot)
        else:
            return plot

    def group_filtered_histotabs(self, dataframe, feature, filter_feature, grouped_filter_feature, log_scale=False,show_plot=False):
        hists = []
        for label,elements in grouped_filter_feature:
            # dataframe[filter_feature].unique will return a list of unique elemnts in the column of dataframe specified by the filter_feature
            # for every one of these elemnts find all rows in the data frame that has that element, and plot a histogram for the specified feature for these rows
            #tldr obtain a histogram of feature for every subset dataframe specified by dataframe[filter_feature] being unique
            #like before createapanelof these 
            first = True
            for ind in elements:
                if first == True:
                    sub_df = dataframe[dataframe[filter_feature] == ind]
                    first = False
                else:
                    sub_df = sub_df.append(dataframe[dataframe[filter_feature] == ind])
                print(sub_df)
              #  print(sub_df)
            histo = self.ratio_histo(sub_df, feature, "poo" "poo", "poo",log_scale=log_scale, show_plot=show_plot)
            p = Panel(child = histo, title=label)
            hists.append(p)
        t = Tabs(tabs=hists)
        show(t)
       
    def hist_2d(self, dataframe, columna, columnb, title, smoothing, bins=100, show_plot=True, vrange=None):
        if columna == 'Change in D Ball Goal/m':
            if smoothing == True:
                print(dataframe[columna].quantile(0.05),dataframe[columna].quantile(0.95))
                
                plt.imshow(Z.reshape(Xgrid.shape),
                           origin='lower', aspect='auto',
                           extent=[dataframe[columna].quantile(0.05), dataframe[columna].quantile(0.95),dataframe[columnb].quantile(0.05), dataframe[columnb].quantile(0.95)],
                           cmap='Blues')
                cb = plt.colorbar()
                cb.set_label("density")
                
            else:
                dataframe=dataframe[dataframe['Phase Type']=='No Possession Change']
                hist, xe, ye = np.histogram2d(dataframe[columna], dataframe[columnb], bins = bins, range = [[dataframe[columna].quantile(0.05), dataframe[columna].quantile(0.95)], [dataframe[columnb].quantile(0.05), dataframe[columnb].quantile(0.95)]])
                p = figure(x_range=(min(xe), max(xe)), y_range=(min(ye), max(ye)), title=title, x_axis_label=columna,y_axis_label=columnb)
                print(xe[0],xe[-1])
                p.image(image=[np.transpose(hist)], x=xe[0], y=ye[0], dw=xe[-1] - xe[0], dh=ye[-1] - ye[0], palette="Spectral11")
                if show_plot == True:
                    show(p)
                else:
                    return p
        if columna == 'Phase Type':
            hist_1d, xedges = np.histogram(dataframe[columnb])


    def group_hist_2d(self, dataframe, columna, columnb, grouped_filter_feature, bins=100, show_plot=True, vrange=None, smoothing=False):
        hists= []
        for label,elements in grouped_filter_feature:
            first = True
            for ind in elements:
                if first == True:
                    sub_df = dataframe[dataframe['Group'] == ind]
                    first = False
                else:
                    sub_df = sub_df.append(dataframe[dataframe['Group'] == ind])
            histo = self.hist_2d(sub_df, columna, columnb, label, smoothing, bins=bins, show_plot=False, vrange=vrange)
            p = Panel(child = histo, title=label)
            hists.append(p)
            t = Tabs(tabs=hists)
            show(t)
            
    def plot_scatter(self, xs , ys , width, height, title, x_label, y_label):
        p = figure(title=title, width=width, height=height)
        p.xaxis.axis_label = x_label
        p.yaxis.axis_label = y_label
        p.circle(xs, ys, color='red', size=5, line_alpha=0)
        return p
    def plot_scatter_add_err(self,p,centre, lower, upper, filename, show_plot, colour):
        err_xs = []
        err_ys = []
        for x, yl, yu in zip(centre,lower, upper):
            err_xs.append((x,x))
            err_ys.append((yl, yu))
        p.multi_line(err_xs, err_ys, color = colour)
        return p
    def single_stat_scatter(self,xs,ys, lower, upper,title,x_label,y_label,filename,show_plot, err_bar):
        p = self.plot_scatter(xs,ys,800,400,title,x_label,y_label)
        if err_bar == 1:
            p = self.plot_scatter_add_err(p,xs,lower,upper,filename,show_plot,'red')
        if err_bar == 2:
            p = self.plot_scatter_add_err(p,xs,lower[0],upper[0],filename,show_plot, 'red')
            p = self.plot_scatter_add_err(p,xs,lower[1],upper[1],filename,show_plot, 'blue')
        if show_plot == True:
            show(p)
        else:
            export_png(p, filename = filename + ".png")
'''
    def mat_scat(self,xs,ys,stdb,std,title,x_label,y_label,filename,show_plot,err_bar,vlines,vlinebool):
        plt.errorbar(xs,ys,ms=2,yerr=std, fmt=".k", ecolor='blue',capsize=5,elinewidth=2,markeredgewidth=2, label = 'SE')
        if stdb != 0:
            plt.errorbar(xs,ys,ms=2,yerr=stdb, fmt=".k", ecolor='red',capsize=5,elinewidth=2,markeredgewidth=2, label = 'SD')
        plt.xlabel(x_label)
        plt.ylabel(y_label)
        plt.title(title)
        plt.legend()
        if vlinebool == True:
            for line in vlines:
                plt.axvline(x=line,linestyle="--",linewidth=1)
        plt.show()
        #show(p)





