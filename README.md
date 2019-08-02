# Background extension to DIAMONDS
<p align="center">
<a href="https://github.com/EnricoCorsaro/Background"><img src="https://img.shields.io/badge/GitHub-Background-yellow"/></a>
<a href="https://github.com/EnricoCorsaro/Background/blob/master/LICENSE.txt"><img src="https://img.shields.io/badge/license-CC%20BY--SA-blue"/></a>
<a href='https://diamonds.readthedocs.io/en/latest/?badge=latest'><img src='https://readthedocs.org/projects/diamonds/badge/?version=latest' alt='Documentation Status' /></a>
<a href="https://github.com/EnricoCorsaro/Background/issues"><img src="https://img.shields.io/github/issues-closed/EnricoCorsaro/Background"/></a>
<img width="500" src="https://raw.githubusercontent.com/EnricoCorsaro/DIAMONDS/master/docs/figures/DIAMONDS_LOGO_WHITE.png"/>
</p>

### Authors
- [Enrico Corsaro](mailto:enrico.corsaro@inaf.it)
- [Joris De Ridder](mailto:joris.deridder@kuleuven.be)

### Short description
<div align="justify">
The Background extension to DIAMONDS is an extension of the Bayesian inference code DIAMONDS for the fitting of the background signal in the power spectrum of solar-like oscillators. It currently implements many different background models that can be chosen by the user through an input reference name at runtime.
</div>

### Download & Installation
The procedure to retrieve the Background extension is identical to that of DIAMONDS (see [diamonds.readthedocs.io](http://diamonds.readthedocs.io/) for detailed information), so you can either clone the repository or simply download it as a ZIP file. In this second option, by unpacking the Background-master.zip file you will find a folder labeled Background-master and containing a structure similar to that of the folder Diamonds. First you need to rename the folder as **Background**, and place it in the same working directory of Diamonds (not inside the Diamonds folder!). This extension needs to be compiled separately from Diamonds, but only after you have compiled Diamonds first. Diamonds is used as a library for this extension. The compilation commands are the same as for Diamonds.  

**IMPORTANT**: Before proceeding with the compilation of the Background code make sure you put the Background folder at the same path level of that of Diamonds. This means that the Background folder has not to be placed inside the Diamonds folder, but inside the parent directory where you placed Diamonds.

### Documentation
Please make sure you read the documentation at [diamonds.readthedocs.io](http://diamonds.readthedocs.io/) before installing and using the code. This extension requires that the DIAMONDS code is first installed in your system. The installation of the Background extension is the same as that done for DIAMONDS.

### Tutorials
To run the tutorials provided in the package, please follow the guidelines presented in [tutorials/README.md](https://github.com/EnricoCorsaro/Background/blob/master/tutorials/README.md)
