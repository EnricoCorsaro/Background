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
`./background KIC 012008916 00 ThreeHarvey 0`
