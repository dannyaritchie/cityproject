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
import seaborn as sns

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
        if stdb[0] != 0:
            plt.errorbar(xs,ys,ms=2,yerr=stdb, fmt=".k", ecolor='red',capsize=5,elinewidth=2,markeredgewidth=2, label = 'SD')
        plt.xlabel(x_label)
        plt.ylabel(y_label)
        plt.title(title)
        plt.legend()
        if vlinebool == True:
            for line in vlines:
                plt.axvline(x=line,linestyle="--",linewidth=1)
      
        #show(p)


def custom_2d(dataframe,columna,columnb,title,x_range,y_range,bins,filename,show_plot,smoothed):
    plt.xlabel(columna)
    plt.ylabel(columnb)
    plt.title(title)
    if smoothed == True:
        data = np.vstack((dataframe[columna],dataframe[columnb]))
        kde = gaussian_kde(data)
        # evaluate on a regular grid
        xgrid = np.linspace(dataframe[columna].quantile(x_range[0]), dataframe[columna].quantile(x_range[1]), bins)
        ygrid = np.linspace(dataframe[columnb].quantile(y_range[0]), dataframe[columnb].quantile(y_range[1]), bins)
        Xgrid, Ygrid = np.meshgrid(xgrid, ygrid)
        Z = kde.evaluate(np.vstack([Xgrid.ravel(), Ygrid.ravel()]))
        plt.imshow(Z.reshape(Xgrid.shape),
           origin='lower', aspect='auto',
           extent=[dataframe[columna].quantile(x_range[0]),dataframe[columna].quantile(x_range[1]), dataframe[columnb].quantile(y_range[0]),dataframe[columnb].quantile(y_range[1])],
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

def all_2d(dataframe,columnas,columnb,folder,x_range=[0.1,0.9],y_range=[0.1,0.9],bins=10,show_plot=False,smoothed=False):
    for columna in columnas:
        title = "Density plot of success(" + columnb + ") vs custom metric"
        filename= folder + "/smoothed_2D.png"
        if smoothed == False:
            title = "Density plot of success(" + columnb + ") vs custom metric"
            filename= folder + "/hex_2D.png"
        custom_2d(dataframe,columna,columnb,title,x_range,y_range,bins,filename,show_plot,smoothed)

def return_binstatistic(df, binned_column, stat_column, number_of_bins, folder, vrange=[0,1],SE=False, cutq=True, show = True, include_frame = True, test="", discreet=0):
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
    stds= []
    stdbs = []
    disc = False
    vlinebool= False
    if binned_column == "Number unmarked within 20m":
        number_of_bins = len(df[binned_column].unique())-1
        cutq=False
    if discreet == 1:
        discreet = df[binned_column].max()
        number_of_bins = []
        edge = -0.5
        number_of_bins.append(edge)
        for i in range(discreet+1):
            edge = edge + 1
            number_of_bins.append(edge)
        cutq=False
    if discreet > 1:
        number_of_bins = discreet
    if stat_column == "Phase Type":
        err_str = "mean and standard error (B?)"
        err_strs = ""
        disc = True
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
            if disc == True:
                std = stats.sem(sub_df[stat_column])
                plot_error=1
                stdb = 0
            stdbs.append(stdb)
            lower_bounds.append(mean - std)
            stds.append(std)
            upper_bounds.append(mean + std) 
            if disc == False:
                lower_boundsb.append(mean-stdb)
                upper_boundsb.append(mean+stdb)
    all_lower.append(lower_bounds)
    all_upper.append(upper_bounds)
    if disc == False:
        all_lower.append(lower_boundsb)
        all_upper.append(upper_boundsb)
    if disc == False:
        if SE == True:
            err_strs = "SE_"
        else:
            err_strs = "SD_"
    title = binned_column + bin_str + " vs " + stat_column + "." + frame_strs
    filename = folder + "/" + err_strs + bin_strs + hotfixb + str(number_of_bins) + frame_str + test + '.png'
    #h.single_stat_scatter(centres,means,all_lower,all_upper,title, binned_column, stat_column,filename, show, plot_error)
    mat_scat(centres,means,stdbs,stds,title,binned_column,stat_column,filename, show, plot_error, bins, vlinebool)
    if show == True:
            plt.show()
    else:
            plt.savefig(filename, format = "png")
    plt.close()

def all_binstatistic(df, binned_columns, number_of_bins, folder, SE=False, cutq=True, show = False, include_frame = True, every=True,vrange=[0,1], test="", discreet = 0):
    if every == True:
        for cut in False,True:
    #        if cut == True:
        #        number_of_bins=number_of_bins-15
            for stat_column in ["Change in D Ball Goal/m","Phase Type"]:
                for binned_column in binned_columns:
                    if stat_column == "Change in D Ball Goal/m":
                        a = df[df['Phase Type'] == 'No Possession Change'].copy(deep = True)
                        return_binstatistic(a, binned_column, stat_column, number_of_bins, folder, vrange, SE=True, cutq=cut, include_frame = False, show=show,test=test,discreet=discreet)
                    else:
                        for frame in True,False:
                            a = df.copy(deep = True)
                            return_binstatistic(a, binned_column, stat_column,number_of_bins, folder, vrange, SE=False, cutq=cut, include_frame = frame, show = show,test=test,discreet=discreet)

def seperated_2D(df, columna, columnb,show_frame=True,foldername="",maxvala=1,maxvalb=1,plot_type = "kde"):
    show_plot = False
    #g = sns.PairGrid(df)
    #g.map_upper(plt.scatter)
    #g.map_lower(sns.kdeplot)
    #g.map_diag(sns.kdeplot, lw=3, legend=False);

    '''
    ordered_days = df."Phase Type".value_counts().index
    g = sns.FacetGrid(df, row="Phase Type", row_order=ordered_days,
                      height=1.7, aspect=4,)
    g.map(sns.distplot, "", hist=False, rug=True);
    '''

    df_pos = df[df['Phase Type']=="Possession Change"].copy(deep = True)
    m=(df["Phase Type"]=="Frame Jump")|(df["Phase Type"]=="Possession Change")
    df_posframe = df[df["Phase Type"] == "Frame Jump"].copy(deep = True).append(df[df["Phase Type"] == "Possession Change"])
    df_frame = df[df['Phase Type']== "Frame Jump"].copy(deep = True)
    df_npos = df[df['Phase Type']=="No Possession Change"].copy(deep = True)
    uppera = min(df_frame[columna].quantile(maxvala),df_pos[columna].quantile(maxvala),df_posframe[columna].quantile(maxvala),df_npos[columna].quantile(maxvala))
    upperb = min(df_frame[columnb].quantile(maxvalb),df_pos[columnb].quantile(maxvalb),df_posframe[columnb].quantile(maxvalb),df_npos[columnb].quantile(maxvalb))
    '''
    print(uppera)
    print(df_frame[columna].max())
    df_frame=df_frame[df_frame[columna]<=uppera]
    dfs = [df_posframe,df_frame,df_pos,df_npos]
    drop=df_frame[df_frame[columna]>= uppera].index
    df_frame.drop(drop, inplace=True)
    print(df_frame[columna].max())
    print(df_pos[columna].max())
    drop=df_pos[df_pos[columna]>= uppera].index
    df_pos.drop(drop, inplace=True)
    print(df_pos[columna].max())
    
    for dft in dfs:
        drop=dft[dft[columna]>= uppera].index
        dft.drop(drop, inplace=True)
    for dft in dfs:
        print("poo",dft[columna].max())
levels = [0,0.1,0.25,0.5,0.68, 0.95, 0.975,1]        '''
    #plot possession change
    fig, ax = plt.subplots(2,2,constrained_layout = True)
    selection = "Possession Change phases"
    title = selection
    if show_frame == True:
        plt.subplot(2,2,1)
    else:
        plt.subplot(1,2,1)
    plt.xlim(0, uppera)
    plt.ylim(0, upperb)
    plt.grid(True)
    seabornplot(df_pos,columna,columnb,plot_type,show_plot,title)
    #seabornplot(df_pos, columna,columnb,"kde",show_plot)
    #plot no possession change
    if show_frame == True:
        selection = "No Possession Change phases"
        title = selection
        plt.subplot(2,2,2)
        plt.xlim(0, uppera)
        plt.ylim(0, upperb)
        plt.grid(True)
        seabornplot(df_npos,columna,columnb,plot_type,show_plot,title)
    #seabornplot(df_npos, columna,columnb,"kde",show_plot)
    #plot possession change
    if show_frame == True:
        selection = "Frame Jump phases"
        title = selection
        plt.subplot(2,2,3)
        plt.xlim(0, uppera)
        plt.ylim(0, upperb)
        plt.grid(True)
        seabornplot(df_frame,columna,columnb,plot_type,show_plot,title)
    selection = "Frame Jump and Possession Change phases"
    title = selection
    if show_frame == True:
        plt.subplot(2,2,4)
    else:
        plt.subplot(1,2,2)
    plt.xlim(0, uppera)
    plt.ylim(0, upperb)
    plt.grid(True)
    seabornplot(df_posframe,columna,columnb,plot_type,show_plot,title)
    fig.suptitle(plot_type + " for occurences of given phase type(s)")
    #fomat nicely
    plt.subplots_adjust(wspace = 0.4, hspace = 0.4)
    #uniform scale

    if show_plot == True:
        plt.show()
    else:
        filename = foldername + columna + "_" + columnb + "_2D.png"
        plt.savefig(filename, format = "png")

def seabornplot(df,columna,columnb,plot_type, show_plot,title,filename = None,gridsize=20):
    stra = str(columna)
    strb = str(columnb)
    if plot_type == "kde_contour":
        ax=sns.kdeplot(df[columna], df[columnb])
    if plot_type == "hex":
        ax = plt.hexbin(df[columna],df[columnb],gridsize=gridsize)
    if plot_type == "kde_shaded":
        ax=sns.kdeplot(df[columna], df[columnb], cmap = "Reds",shade=True)
    plt.title(title)
    if columnb == "Summed attacker space within 100m":
        columnb = "A space in 100m"
    if columnb == "Summed attacker space within 25m":
        columnb = "A space in 25m"
    if columnb == "Summed attacker space within 50m":
        columnb = "A space in 50m"
    if columnb == "Summed attacker space within 15m":
        columnb = "A space in 15m"
    if columna == "Summed attacker space within 100m":
        columna = "A space in 100m"
    if columna == "Summed attacker space within 25m":
        columna = "A space in 25m"
    if columna == "Summed attacker space within 50m":
        columna = "A space in 50m"
    if columna == "Summed attacker space within 15m":
        columna = "A space in 15m"
    plt.xlabel(columna)
    plt.ylabel(columnb)
    if show_plot == True:
        plt.show()
    return ax

def columnDist(df,column,save_plot=False,filepath=None,maximum = 0,vrange=[0,1]):
    df = df[df[column]>=df[column].quantile(vrange[0])]
    df = df[df[column]<=df[column].quantile(vrange[1])]
    df_pos = df[df['Phase Type']=="Possession Change"].copy(deep = True)
    m=(df["Phase Type"]=="Frame Jump")|(df["Phase Type"]=="Possession Change")
    df_posframe = df[df["Phase Type"] == "Frame Jump"].copy(deep = True).append(df[df["Phase Type"] == "Possession Change"])
    df_frame = df[df['Phase Type']== "Frame Jump"].copy(deep = True)
    df_npos = df[df['Phase Type']=="No Possession Change"].copy(deep = True)
    if maximum != 0:
        df_posframe = df_posframe[df_posframe[column]<=maximum]
        df_npos = df_npos[df_npos[column]<=maximum]
    print(df_npos[column])
    sns.distplot( df_posframe[column] , color="skyblue", label="Successful (inc F)")
    sns.distplot( df_npos[column] , color="red", label="Unsuccessful`")
    plt.legend()
    if save_plot == True:
        plt.savefig(filepath + column + ".png")
        plt.close()
    else:
        plt.show()

#MAIN
#First read in the data to a panda dataframe
myDataFile = '../../c++/data/newdata/groupedsplit/17adata.json'
df = pd.read_json(myDataFile,orient='split')

#Then we want to check what each colum means, the following line will output the header of each column
for i,v in enumerate(list(df.columns)):
    print(i,",",v)

#Grouped Plotters
#Hopefully the following functions should allow multiple graphs to be generated, however individual parameters may need to be specified for aesthetics

#these variable are often required by the functions below
column_plotted = 10
folder_path = "../data/Summed_X/25/"
#folder_path = "../data/Combined-1D/rund/"

'''
#This function will plot both hexagon and gaussian smoothed for the provided columns and save in folder_path. Note if column plotted is changed to a slice (4:) would select all columns after and including 4, the [] around list must also be deleted as the current slice speciefier ofasingle integer returns a single element rather than a list of length one.
df = df[df['Phase Type'] == 'No Possession Change']
for i,graph_type in zip([100,20],[True,False]):
    all_2d(df,[list(df.columns)[column_plotted]],'Change in D Ball Goal/m',folder_path,bins=i,smoothed=graph_type,show_plot=False,vrange=[0,1])
    '''
all_2d(df,[list(df.columns)[column_plotted]],'Change in D Ball Goal/m',folder_path,bins=100,smoothed=True,show_plot=False,x_range=[0.01,0.99],y_range=[0.2,0.8])

#This function will plot histogram of column specified on x axis against both success ratio and change in D ball goal, for constant bin width and bin number, and for the case of success ratio including and not including frame jump phases. make sure df = df[df['Phase Type'] == 'No Possession Change'] has been commented out
#all_binstatistic(df, [list(df.columns)[column_plotted]], 10, folder_path,discreet=0)

'''
#This function is for instead binning change in D ball goal (will not work for success ratio)
for cut in True, False:
    a = df[df['Phase Type'] == 'No Possession Change'].copy(deep = True)
    return_binstatistic(a, "Change in D Ball Goal/m", list(df.columns)[column_plotted], 10, folder_path, vrange=[0.05,0.95], cutq=cut, show = False)
'''

'''
#this function will plot 2D densities for the unsuccessful and successful plots for proided columna and column b. Passing plot_type as a b or c returns countour, shaded or hexagon density plots respectively. maxvala specifies the percentile to plot - 0.8 would mean only bottom %80 is included, and 1 means all data isincluded. maxvala is for x axis, maxvalb is for y axis.
columna = 10
columnb = 30
a = "kde_contour"
b = "kde_shaded"
c = "hex"
print(df.iloc[14].isin([-np.inf,np.inf,np.nan]).sum())
df_in = df[df.iloc[:,columnb]!=-1].copy(deep = True)
seperated_2D(df_in,list(df.columns)[columna],list(df.columns)[columnb],foldername=folder_path,plot_type=a)
'''

'''
#This function creates a new column equal to distance away from a set point.
for i in range(10):
    df["3_10_cntr"] = (((2200*i-0.3*df.iloc[:,3])**2)*(1950-df.iloc[:,4])**2)**0.5 
    columnDist(df,"3_10_cntr",False,"../data/Combined-1D/")
#columnDist(df,"D Ball Nearest A",True,"../data/Combined-1D/")
#columnDist(df,"Summed attacker space within 25m",True,"../data/Combined-1D/")
'''

'''
for i in range(30,31):
    print(i)
    df_in = df[df.iloc[:,i]!=-1].copy(deep = True)
    df_out = df[df.iloc[:,i]==-1].copy(deep = True)
    print(i-27, "'th attacker not within 25m of ball")
    print("Succ",len(df_out[df_out["Phase Type"]=="Frame Jump"]) + len(df_out[df_out["Phase Type"]=="Possession Change"]))
    print("USucc",len(df_out[df_out["Phase Type"]=="No Possession Change"]))
    print(i-27, "'th attacker within 25m of ball")
    print("Succ",len(df_in[df_in["Phase Type"]=="Frame Jump"]) + len(df_in[df_in["Phase Type"]=="Possession Change"]))
    print("USucc",len(df_in[df_in["Phase Type"]=="No Possession Change"]))
    columnDist(df_in,list(df.columns)[i],True,folder_path,vrange = [0.004,0.996])
    '''

'''
#This function creates a new column equal to distance dba/Distance dbd, then plots kernel number density for both successful and unsuccessful phasetypes
df["dAOdD"] =(df.iloc[:,3]+0.1)**-1*df.iloc[:,4]**2 

df["CA"] = df["Summed attacker space within 100m"]*df["D Ball Nearest D"]
print(df["CA"])
columnDist(df,"dAOdD",True,"../data/Combined-1D/",0.01)
columnDist(df,"CA",True,"../data/Combined-1D/")
columnDist(df,"Summed attacker space within 100m",True,"../data/Combined-1D/")
columnDist(df,"D Ball Nearest A",True,"../data/Combined-1D/")
#for grouped groups-not implemented yet
'''
#columnDist(df,list(df.columns)[column_plotted],True,folder_path)

'''
group = [43,1,6,14,8,3,90,11,13,4,31,91,21,57,36,38,20,80,110,35]
groupA = [43,1,6,14,8,3]
groupB = [90,11,13,4,31,91,21]
groupC = [57,36,38,20,80,110,35]
labels = ["All","Top", "Middle", "Bottom"]
groups = [group,groupA,groupB,groupC]
grouped = zip(labels,groups)
'''
