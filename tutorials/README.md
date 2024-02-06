# Tutorial for the background fitting of the red giant star KIC 12008916 using a three-Harvey model

In this tutorial you will find an example of format for data and configuring files for one red giant star observed by the NASA’s Kepler mission.
The background model adopted for the test star comprises three Harvey profiles, two describing the granulation activity and one related to long-trend variations (at low frequency) and it can be identified within the code as the `ThreeHarvey` background model.
The figure below provides an example of the resulting fit (in red) to this star using the Background code extension of DIAMONDS.

**NOTE**: the adoption of a different background model simply requires that the input file containing the prior parameters is adapted according to the number of free parameters of the new background model. A list of implemented background models is readable inside the `source/Background.cpp` file, while their actual implementation can ben retrieved from the corresponding .cpp files in the `source/` folder of this extension code. See also [here](https://famed.readthedocs.io/en/latest/background_models.html) for a summary description of all the available background models that incorporate a Gaussian envelope.

![Background fit](https://raw.githubusercontent.com/EnricoCorsaro/Background/master/tutorials/KIC012008916_Background_Plot.png)

To run the tutorial follow the procedure (if you have installed the Background code using the install.sh script, then go directly to step # 6):

1. Move the file `KIC012008916/localPath.txt` into `Background/build/`
2. Edit the path inside `localPath.txt` to match your local working path
3. Move the file `KIC012008916/KIC012008916.txt` into `Background/data/`
4. Create an empty directory labeled `00` inside `KIC012008916/`
5. Move the folder `KIC012008916` (and all its content) under `Background/results/`
6. Go to `Background/build/`
7. Execute the code for this tutorial by using the command line 
```bash
./background KIC 012008916 00 ThreeHarvey background_hyperParameters 0.0 0
```

In this format, the command line specifies the following inputs:
1. The Catalog_ID (here KIC).
2. The Star_ID (here 012008916), which coupled to the Catalog_ID has to match exactly the filename given to the dataset.
3. The subfolder where the outputs from the fit will be stored (here `00`).
4. The name of the background chosen for the fit (here `ThreeHarvey`). See also [here](https://famed.readthedocs.io/en/latest/background_models.html) for a list and description of the different background models implemented in Background.
5. The prefix string of the prior hyper-parameters file (typically `background_hyperParameters`). Before running a fit, the user has to make sure that a prior filename ending with the same name of the subfolder name given as input #3 is already placed inside the stellar folder under `Background/results/KIC012008916/`). In this example, the prior file is `background_hyperParameters_00.txt`.
6. An input threshold for the low-frequency region. This can be used as a high-pass filter frequency in case the user is not interested in fitting all the data below this given frequency. It can be useful for cases where a filtering is applied in the low frequency part of the dataset (e.g. to remove rotational harmonics). It has to be specified in muHz. The default input is 0.0, meaning that no threshold is used.
7. The flag for the Principal Component Analysis (PCA). PCA is a dimensional projection technique to help in reducing the effective number of dimensions (or free parameters) during the fitting process. For activating this option, set this flag to 1. The default value that we recommend for the background fitting is 0, meaning that PCA is not activated. This is because the background parameters exhibit strong correlations and a reduction in the effective number of dimensions can be dangerous and yield bad fitting results.

Once the computation is completed, you can plot the results with Python by using the suite provided in the file `background.py` of the tutorials folder. Please make sure that all paths set inside the Python routines match correctly with your actual working paths for Background. For producing the actual plot of the background fit follow the guidelines below:
1. Move the file `Background/tutorials/background.py` into `Background/results/python/` (create this folder if you don’t have one already)
2. Go to `Background/results/python` and open a python prompt (e.g. IPython)
3. In the python prompt execute the commands

```python
from background import *
background_plot('KIC','012008916','00')
```
Note that the routine automatically recognizes what type of background model was fitted by the Background code, so that the user has not to take care of specifying the name of the model as input. If you also want to plot the Marginal Probability Distributions (MPD) for each free parameter (see figure below) then from the same python prompt execute the command

```python
background_mpd('KIC','012008916','00')
```

![Background MPD](https://raw.githubusercontent.com/EnricoCorsaro/Background/master/tutorials/KIC012008916_Background_MPD.png)

# Tutorial for generating new background priors and all the files needed to execute a fit

This tutorial allows you to generate all the required files for running a new background fit on any input star by starting from the dataset and a raw guess for nuMax. With this tutorial the user will be able to generate the uniform prior boundaries on each free parameter of the background model that is intended to fit. The user can choose any background model among those implemented in the Background code. To run the tutorial follow the steps below. If you have already run the first tutorial, then go directly to step # 4.

1. Move the file `Background/tutorials/background.py` inside the folder `Background/results/python/` (create this folder if you don’t have one already)
2. Make sure you have the datafile of the star you want to analyze already placed inside the folder `Background/data`, having the same format of the one used in the first tutorial. To illustrate this tutorial we consider again the star KIC 12008916
3. Go to `Background/results/python` and open a python prompt (e.g. IPython)
4. In the python prompt execute the commands

```python
from background import *
set_background_priors('KIC','012008916',162,'ThreeHarvey',1)
```

The call to the method `set_background_priors` requires 5 input parameters, namely the catalog ID and star ID, a guess for nuMax (in microHz), the name of the background model for which the priors are desired (among those provided in the Background code package), and an integer specifying the name of the subfolder where we want to store the results from the fit.

This tutorial will generate a new `background_hyperParameters_01.txt` file inside the star folder `Background/results/KIC012008916/`, as well as the required configuring files for DIAMONDS (i.e. `NSMC_configuringParameters.txt`, `Xmeans_configuringParameters.txt`, and `NyquistFrequency.txt`, if they do not exist already). It will also create the entire output star folder if none is present. You will be able to run the corresponding fit going to `Background/build/` and executing the command line 
```bash
./background KIC 012008916 01 ThreeHarvey background_hyperParameters 0.0 0
```
All the results from the fit will be stored in the folder `Background/results/KIC012008916/01/`.

# Tutorial for correcting the uniform prior boundaries in case of bad fits or incomplete executions

When a fit is not performed correctly the background fit level will not match the smoothed power spectrum (black line in the background plot figure). Most likely the cause of this result relies on our choice of the prior boundaries, which could be wrong for at least one of the free parameters. In more severe cases, the Background code is not even able to converge to a solution, so that one cannot check the output plot of the fit. This produces a "segmentation fault" or "assertion failed" error taking place in Results.cpp of the DIAMONDS code. If this happens, the marginal distributions of the corresponding parameters cannot be computed and the process stops without generating the parameter summary file that contains all the estimates (the `background_parameterSummary.txt` file will not be present).

A good practice in this case is to always plot the sampling evolution of all the free parameters (all the files of the type `background_parameterXXX.txt`). These files will be available even if the computation stops with failures. Each of these files contains one column, which has the value of the parameter at each nested iteration (from the first line, corresponding to iteration 0, to the last line corresponding to the last iteration). As you approach to the end of the nested sampling process, a good sampling evolution (resulting in a good fit) should in principle converge to a specific region well within the prior range.

In this tutorial we show how to intervene in such situations and therefore how to properly correct our prior boundaries in order to repeat the fit. To run the tutorial make sure you have run the first tutorial at least once, so that the output folder of the Background code contains the results of the fit. Then, follow the steps below.

1. Go to `Background/results/python` and open a python prompt (e.g. IPython)
2. In the python prompt execute the commands

```python
from background import *
parameter_evolution('KIC','012008916','00')
```

The routine will generate a plot like then one shown in the figure below. This is the sampling evolution within the nested sampling process for each of the free parameters of the fit (here numbered from 0 up to the last one, and listed in the same order they are defined in both the prior list file and in the model function). This plot can be generated independently of whether a fit has been produced or not, i.e. the sampling evolution can be inspected even if the fit has failed. The y-axis of each sub-plot indicates the prior range adopted for the corresponding free parameter.
![Background MPD](https://raw.githubusercontent.com/EnricoCorsaro/Background/master/tutorials/KIC012008916_Parameter_Evolution.png)

This figure shows an example of a good convergence for all the free parameters. When a bad convergence is present, instead, there can be be one or more free parameters evolving toward the boundary of the prior range, or one or more free parameters that follow an unclear evolution path with multiple outcomes. Concerning the first case of the parameters evolving toward the prior boundaries, this implies that the prior range is not adequate for allowing the parameter to evolve to a final solution. The prior ranges of these parameters must then be corrected to allow more freedom in the parameter evolution. Concerning the second case, the ill-defined parameters (those that appear to follow multiple paths during the evolution) do normally fix up alone when you correct the priors for those parameters that evolve toward the prior edges, so we suggest not to take action on them before repeating the fit. This is generally caused by a bad convergence of the whole fitting process, which does not allow some of the parameters to have a clear evolution.

Once you have identified the "faulty" priors, the second aspect to consider is by how much the priors should be changed. This could be somewhat difficult to grasp, especially if you are not familiar with this type of fits. For this purpose one can adopt a simple method that can be effective. Take the sampling evolution of the parameter whose prior has to be corrected and compute a simple average and standard deviation out of its sampling. Then correct the prior range by building it as centered around the parameter average and with an extent proportional to the obtained standard deviation, e.g. 3 times the standard deviation on each side of the parameter average (this is generally well enough). For inspecting the parameter evolution of an individual parameter, e.g. the first one (#0), the user can exploit an additional method provided in the `background.py` suite through the command

```python
single_parameter_evolution('KIC','012008916','00',0)
```

Therefore the number of the input parameter to inspect has to be specified as an integer. When the priors are corrected within the `background_hyperParameters_XX.txt` file, the fit can be repeated. The whole process can be repeated if you still get an error at the end of the computation. All the faulty priors should be fixed before repeating the whole fit.

**NOTE**: when using the `set_background_priors` method presented in the previous tutorial, most often the parameters that can yield an error at the end of the sampling process are the free parameters #1 and #2, i.e. the amplitude and characteristic frequency of the Harvey-like profile having the lowest frequency (assuming that the background model is incorporating this component). This is because this profile is generally associated to some long-trend variation in the photometric signal, which arises from different contributions such as activity, rotational modulation, super-granulation and instrumental effects, that are difficult to be identified and modeled in a proper way. 
