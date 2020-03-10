import pandas as pd
import numpy as np
#from bokeh.plotting import figure
#from bokeh.io import output_notebook, show, output_file
#from bokeh.models import ColumnDataSource, HoverTool, Panel
#from bokeh.models.widgets import Tabs
#from bokehhistogram import BokehHistogram 
from scipy.stats import gaussian_kde
from scipy import stats
import matplotlib.pyplot as plt

'''
def return_binstatistics(df, binned_column, stat_columns, constant_number_bins, number_of_bins):
    df[binned_column + "_binnedk"], bins = pd.qcut(df[binned_column], number_of_bins, retbins=True )
    bind = {'Bins' : }
    summary = pd.DataFrame(Data = bind)

    for binit in df[binned_column + "_binnedk"].unique():
        sub_df = df[df[binned_column + "_binnedk"] == binit]
        means = []
        lower_bounds = []
        upper_bounds = []
        for stat_column in stat_columns:
            mean = sub_df[stat_column].mean()
            std = sub_df[stat_column].std()
            means.append(mean)
            lower_bounds.append(mean - std)
            upper_bounds.append(mean + std)
        d = {'Mean': means, 'std': std, 'Column_type': stat_columns}
        binned_summary = pd.DataFrame(data = d)
        summary[summary['Bins'] == binit] = binned_summary
'''
def mat_scat(xs,ys,stdb,std,title,x_label,y_label,filename,show_plot,err_bar,vlines,vlinebool):
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
      
        #show(p)


def custom_2d(dataframe,columna,columnb,title,vrange,bins,filename,show_plot,smoothed):
    plt.xlabel(columna)
    plt.ylabel(columnb)
    plt.title(title)
    if smoothed == True:
        data = np.vstack((dataframe[columna],dataframe[columnb]))
        kde = gaussian_kde(data)
        # evaluate on a regular grid
        xgrid = np.linspace(dataframe[columna].quantile(vrange[0]), dataframe[columna].quantile(vrange[1]), bins)
        ygrid = np.linspace(dataframe[columnb].quantile(vrange[0]), dataframe[columnb].quantile(vrange[1]), bins)
        Xgrid, Ygrid = np.meshgrid(xgrid, ygrid)
        Z = kde.evaluate(np.vstack([Xgrid.ravel(), Ygrid.ravel()]))
        plt.imshow(Z.reshape(Xgrid.shape),
           origin='lower', aspect='auto',
           extent=[dataframe[columna].quantile(vrange[0]),dataframe[columna].quantile(vrange[1]), dataframe[columnb].quantile(vrange[0]),dataframe[columnb].quantile(0.95)],
           cmap='Blues')
        cb = plt.colorbar()
        cb.set_label("Density, smoothed by gaussian kernels")
    else:
        sub_df = dataframe[dataframe[columnb]>=dataframe[columnb].quantile(0.05)]
        sub_df = sub_df[sub_df[columnb]<=sub_df[columnb].quantile(0.95)]
        sub_df = sub_df[sub_df[columna]<=sub_df[columna].quantile(0.95)]
        sub_df = sub_df[sub_df[columna]>=sub_df[columna].quantile(0.05)]
        plt.hexbin(sub_df[columna],sub_df[columnb], gridsize=bins, cmap='Blues')
        #plt.hexbin(dataframe[columna].quantile([vrange[0],vrange[1]]),dataframe[columnb].quantile([vrange[0],vrange[1]]), gridsize=bins, cmap='Blues')
        #plt.hist2d(dataframe[columna],dataframe[columnb], bins=bins, cmap='Blues',range = [[dataframe[columna].quantile(vrange[0]),dataframe[columna].quantile(vrange[1])],[dataframe[columnb].quantile(vrange[0]),dataframe[columnb].quantile(vrange[1])]])
        cb = plt.colorbar()
        cb.set_label('Density')
    if show_plot==True:
        plt.show()
    else:
        plt.savefig(filename,format="png")
    plt.close()

def all_2d(dataframe,columnas,columnb,folder,vrange=[0.1,0.9],bins=10,show_plot=False,smoothed=False):
    for columna in columnas:
        title = "Density plot of success(" + columnb + ") vs custom metric"
        filename= folder + "/smoothed_2D.png"
        if smoothed == False:
            title = "Density plot of success(" + columnb + ") vs custom metric"
            filename= folder + "/hex_2D.png"
        custom_2d(dataframe,columna,columnb,title,vrange,bins,filename,show_plot,smoothed)

def return_binstatistic(df, binned_column, stat_column, number_of_bins, folder, vrange=[0,1],SE=False, cutq=True, show = True, include_frame = True, test=""):
    hotfix = binned_column
    if binned_column == "Change in D Ball Goal/m":
        hotfix = "DeltaX"
    hotfixb = stat_column
    if stat_column == "Change in D Ball Goal/m":
        hotfixb = "DeltaX"
    plot_error = 2
    bin_str="A"
    bin_strs=""
    err_str= "C"
    err_strs="D"
    frame_str = ""
    frame_strs = ""
    std = 0
    stdb = 0
    discreet = False
    vlinebool= False
    if binned_column == "Number unmarked within 20m":
        number_of_bins = len(df[binned_column].unique())-1
        cutq=False
    if stat_column == "Phase Type":
        err_str = "mean and standard error (B?)"
        err_strs = ""
        discreet = True
        if include_frame == True:
            frame_str = "_OOP"
            frame_strs = " (including frame jump)"
            m=(df["Phase Type"]=="Frame Jump")|(df["Phase Type"]=="Possession Change")
            df.loc[m, "Phase Type"] = 1
        else:
            frame_str = "_NoOOP"
            frame_strs = " (not including frame jump)"
            m=(df["Phase Type"]=="Possession Change")
            df.loc[m, "Phase Type"] = 1
            drop=df[df["Phase Type"]== "Frame Jump"].index
            df.drop(drop, inplace=True)
        m= (df["Phase Type"]=="No Possession Change")
        df.loc[m, "Phase Type"] = 0
    if cutq==True:
        vlinebool = True
        bin_str = " binned with constant bin number"
        bin_strs = "cn_"
        df = df[df[binned_column]<=df[binned_column].quantile(vrange[1])]
        df = df[df[binned_column]>=df[binned_column].quantile(vrange[0])]
        df['Bin'], bins = pd.qcut(df[binned_column], q = number_of_bins, retbins= True)
    else:
        bin_str = " binned with constant bin width"
        bin_strs = "cw_"
        df = df[df[binned_column]<=df[binned_column].quantile(vrange[1])]
        df = df[df[binned_column]>=df[binned_column].quantile(vrange[0])]
        df['Bin'], bins = pd.cut(df[binned_column], bins = number_of_bins, retbins= True)
    centres = []
    means = []
    lower_boundsb =[]
    upper_boundsb = []
    lower_bounds =[]
    upper_bounds = []
    all_lower = []
    all_upper = []
    for binit in df['Bin'].unique():
        sub_df = df[df['Bin'] == binit]
        mean = sub_df[stat_column].mean()
        stdb = sub_df[stat_column].std()
        if(len(sub_df.index)>5):
            centres.append(binit.mid)
            means.append(mean)
            std = stdb/np.sqrt(len(sub_df.index))
            if discreet == True:
                print("poos")
                std = stats.sem(sub_df[stat_column])
                plot_error=1
                stdb = 0
            lower_bounds.append(mean - std)
            upper_bounds.append(mean + std) 
            if discreet == False:
                print("poo")
                lower_boundsb.append(mean-stdb)
                upper_boundsb.append(mean+stdb)
    all_lower.append(lower_bounds)
    all_upper.append(upper_bounds)
    if discreet == False:
        all_lower.append(lower_boundsb)
        all_upper.append(upper_boundsb)
    if discreet == False:
        if SE == True:
            err_strs = "SE_"
        else:
            err_strs = "SD_"
    title = binned_column + bin_str + " vs " + stat_column + "." + frame_strs
    filename = folder + "/" + err_strs + bin_strs + hotfixb + str(number_of_bins) + frame_str + test + '.png'
    #h.single_stat_scatter(centres,means,all_lower,all_upper,title, binned_column, stat_column,filename, show, plot_error)
    mat_scat(centres,means,stdb,std,title,binned_column,stat_column,filename, show, plot_error, bins, vlinebool)
    if show == True:
            plt.show()
    else:
            plt.savefig(filename, format = "png")
    plt.close()

def all_binstatistic(df, binned_columns, number_of_bins, folder, SE=False, cutq=True, show = False, include_frame = True, every=True,vrange=[0,1], test=""):
    if every == True:
        for cut in False,True:
            if cut == True:
                number_of_bins=number_of_bins-15
            for stat_column in ["Change in D Ball Goal/m","Phase Type"]:
                for binned_column in binned_columns:
                    if stat_column == "Change in D Ball Goal/m":
                        a = df[df['Phase Type'] == 'No Possession Change'].copy(deep = True)
                        return_binstatistic(a, binned_column, stat_column, number_of_bins, folder, vrange, SE=True, cutq=cut, include_frame = False, show=show,test=test)
                    else:
                        for frame in True,False:
                            a = df.copy(deep = True)
                            return_binstatistic(a, binned_column, stat_column,number_of_bins, folder, vrange, SE=False, cutq=cut, include_frame = frame, show = show,test=test)

#MAIN
#First read in the data to a panda dataframe
myDataFile = '../../c++/data/groupedsplit/17adata.json'
df = pd.read_json(myDataFile,orient='split')

#Then we want to check what each colum means, the following line will output the header of each column
print(list(df.columns))

#Grouped Plotters
#Hopefully the following functions should allow multiple graphs to be generated, however individual parameters may need to be specified for aesthetics

#This function will plot both hexagon and gaussian smoothed for the provided columns and save in folder_path. Note if column plotted is changed to a slice (4:) would select all columns after and including 4 the [] around list must be deleted.

'''
column_plotted = 7
folder_path = "../data/test"
df = df[df['Phase Type'] == 'No Possession Change']
for i,graph_type in zip([100,20],[True,False]):
    all_2d(df,[list(df.columns)[column_plotted]],'Change in D Ball Goal/m',folder_path,bins=i,smoothed=graph_type,show_plot=False)
'''

#This function will plot histogram of column specified on x axis against both success ratio and chnage in D ball goal, for constant bin width and bin number, and for the case of success ratio including and not including frame jump phases. make sure df = df[df['Phase Type'] == 'No Possession Change'] has been commented out

'''
column_plotted = 7
folder_path = "../data/test"
all_binstatistic(df, [list(df.columns)[column_plotted]], 25, folder_path)
'''

#This function is for instead binning change in D ball goal (will not work for success ratio)

column_plotted = 7
folder_path = "../data/test"
for cut in True, False:
    a = df[df['Phase Type'] == 'No Possession Change'].copy(deep = True)
    return_binstatistic(a, "Change in D Ball Goal/m", list(df.columns)[column_plotted], 10, folder_path, vrange=[0.05,0.95], cutq=cut, show = False)

#for grouped groups-not implemented yet
'''
group = [43,1,6,14,8,3,90,11,13,4,31,91,21,57,36,38,20,80,110,35]
groupA = [43,1,6,14,8,3]
groupB = [90,11,13,4,31,91,21]
groupC = [57,36,38,20,80,110,35]
labels = ["All","Top", "Middle", "Bottom"]
groups = [group,groupA,groupB,groupC]
grouped = zip(labels,groups)
'''
