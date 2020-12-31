# Tutorial for the background fitting of the red giant star KIC 12008916 using a three Harvey model

In this tutorial you will find an example of format for data and configuring files for one red giant star observed by the NASA’s Kepler mission.
The background model adopted for the test star comprises three Harvey profiles, two describing the granulation activity and one related to long-trend variations (at low frequency).
The figure below provides an example of the resulting fit (in red) to this star using the Background code extension of DIAMONDS.

**NOTE**: the adoption of a different background model simply requires that the input file containing the prior parameters is adapted according to the number of free parameters of the new background model. A list of implemented background models is readable inside the `source/Background.cpp` file, while their actual implementation can ben retrieved from the corresponding .cpp files in the `source/` folder of this extension code.

![Background fit](https://raw.githubusercontent.com/EnricoCorsaro/Background/master/tutorials/KIC012008916_Background.png)

To run the tutorial follow the procedure:

1. Move the file `KIC012008916/localPath.txt` into `Background/build/`
2. Edit the path inside `localPath.txt` to match your local working path
3. Move the file `KIC012008916/KIC012008916.txt` into `Background/data/`
4. Create an empty directory labeled `00` inside `KIC012008916/`
5. Move the folder `KIC012008916` (and all its content) under `Background/results/`
6. Go to `Background/build/`
7. Execute the code for this tutorial by using the command line 
`./background KIC 012008916 00 ThreeHarvey background_hyperParameters 0`
8. Once the computation is completed, you can plot the results with Python by using `plot_background.py` provided in the tutorials folder. Please make sure that all paths set inside the Python routines match correctly with your actual working paths for Background.

# Tutorial for generating new background priors and all the files needed to execute a fit

This tutorial allows you to generate all the required files for running a new background fit on any input star by starting from the dataset and a raw guess for nuMax. With this tutorial the user will be able to generate the uniform prior boundaries on each free parameter of the background model that is intended to fit. The user can choose any background model among those implemented in the Background code.

1. Move the file `set_background_priors.py` inside the folder `Background/results/python/` (create one if you don’t have one already)
2. Make sure you have the datafile of the star you want to analyze already placed inside the folder `Background/data`, having the same format of the one used in the first tutorial. To illustrate this tutorial we consider the star KIC 12008916
3. Go to `Background/results/python` and open a python prompt (e.g. IPython)
4. In the python prompt execute the commands

```python
from set_background_priors import *
set_background_priors('KIC','012008916',162,'ThreeHarvey’,1)
```

The call to the method `set_background_priors` requires 5 input parameters, namely the catalog ID and star ID, a guess for nuMax (in microHz), the name of the background model for which the priors are desired (among those provided in the Background code package), and an integer specifying the name of the subfolder where we want to store the results from the fit.

This tutorial will generate a new background_hyperParameters_01.txt file inside the star folder `Background/results/KIC012008916/`, as well as the required configuring files for DIAMONDS (i.e. `NSMC_configuringParameters.txt`, `Xmeans_configuringParameters.txt`, and `NyquistFrequency.txt`). It will also create the entire output star folder if none is present. You will be able to run the corresponding fit going to `Background/build/` and executing the command line `./background KIC 012008916 01 ThreeHarvey background_hyperParameters 0`

All the results from the fit will be stored in the folder `Background/results/KIC012008916/01/`.
