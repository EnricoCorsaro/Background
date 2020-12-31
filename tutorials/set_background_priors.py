import os
import numpy as np

def smooth(x, window_len=11, window='hanning'):
    """
	Author: Jean McKeever
	email: jean.mckeever@yale.edu
	Created: December 2020
    
    Smooth the data using a window with requested size.
    
    This method is based on the convolution of a scaled window with the signal.
    The signal is prepared by introducing reflected copies of the signal 
    (with the window size) in both ends so that transient parts are minimized
    in the begining and end part of the output signal.
    
    :param x: The input signal 
    :type x: array
    :param window_len: The dimension of the smoothing window; should be an odd
                       integer
    :type window_len: int
    :param window: The type of window from 'flat', 'hanning', 'hamming', 
                   'bartlett', 'blackman'. Flat window will produce a moving 
                   average smoothing.
    :type window: str
    :return The smoothed signal  
    :rtype array
    """
    if x.ndim != 1:
        raise ValueError("smooth only accepts 1 dimension arrays.")
    if x.size < window_len:
        raise ValueError("Input vector needs to be bigger than window size.")
    if window_len<3:
        return x
    if not window in ['flat', 'hanning', 'hamming', 'bartlett', 'blackman']:
        raise ValueError("Window is one of 'flat', 'hanning', 'hamming', 'bartlett', 'blackman'")
    
    s=np.r_[x[window_len-1:0:-1],x,x[-2:-window_len-1:-1]]
    
    if window == 'flat': #moving average
        w=np.ones(window_len,'d')
    else:
        w=eval('np.'+window+'(window_len)')

    y=np.convolve(w/w.sum(),s,mode='valid')
    
    return y[window_len//2:-window_len//2+1]


def set_background_priors(catalog_id,star_id,numax,modelname,dir_flag=0):
	"""
	Author: Enrico Corsaro
	email: enrico.corsaro@inaf.it
	Created: January 2020
	INAF-OACT

	This method creates all the files needed for the background fitting of a star using DIAMONDS.

	:param catalog_id: the Catalog name of the star (e.g. KIC, TIC, etc.)
	:type catalog_id: str

	:param star_id: the ID number of the star
	:type star_id: str

	:param numax: a raw guess for the value of nuMax
	:type numax: float

	:param modelname: the background model for which priors are desired
	:type modelname: str

	:param dir_fglag: the number for the subfolder to contain the fitting results
	:type dir_flag: int

	""" 
	local_path = str(np.loadtxt('../../build/localPath.txt',dtype='str'))
	input_dir = local_path + 'data/' 
	output_dir = local_path + 'results/' + catalog_id + star_id + '/'

	print(' ---------------------------------------------- ')
	print(' Creating DIAMONDS priors for ' + catalog_id + star_id)
	print(' ---------------------------------------------- ')
	
	freq,psd = np.loadtxt(input_dir + catalog_id + star_id + '.txt').T

	numax_range = numax*0.1
	lower_numax = numax - numax_range
	upper_numax = numax + numax_range
	dnu = 0.267*numax**0.760
	sigma = 2.0 * dnu
	sigma_range = sigma*0.4
	lower_sigma = sigma - sigma_range*1.5
	upper_sigma = sigma + sigma_range

	freqbin = freq[1] - freq[0]
	smth_bins = int(dnu/freqbin)
	psd_smth = smooth(psd, window_len=smth_bins, window='flat')

	interesting_zone = np.where((freq > numax - 3*dnu) & (freq < numax + 3*dnu))[0]
	height = np.max(psd_smth[interesting_zone])
	lower_height = 0.1 * height
	upper_height = 1.40 * height


	# Define the priors for the white noise
	
	if np.max(freq) < 300 & numax > 200:
		tmp_w = np.where(freq > 200.)[0]
	else: 
		tmp_w = np.where(freq > numax+2*dnu)[0]
	
	if len(tmp_w) != 0:
		white_noise_array = psd[tmp_w]
	else:
		tmp_w = np.where(freq > numax)[0]
		white_noise_array = psd[tmp_w]
	
	white_noise = np.mean(white_noise_array)
	lower_white_noise = 0.5 * white_noise 
	upper_white_noise = 1.50 * white_noise 


	# Define the priors for the meso-granulation

	nu_g1 = 0.317 * numax**0.970
	lower_nu_g1 = 0.6 * nu_g1 
	upper_nu_g1 = 1.4 * nu_g1
	amp_g1 = 3383 * numax**-0.609
	
	g1_range = nu_g1 * 0.1
	tmp_g1 = np.where((freq >= nu_g1 - g1_range) & (freq <= nu_g1 + g1_range))[0]
	psd_g1 = np.max(psd[tmp_g1])
	amp_g1_data = np.sqrt(psd_g1*nu_g1)/(2*np.sqrt(2))*np.pi
	
	if amp_g1_data > amp_g1:
		amp_g1 = amp_g1_data
	
	lower_amp_g1 = 0.3 * amp_g1
	upper_amp_g1 = 1.5 * amp_g1 


	# Define the priors for the granulation 

	nu_g2 = 0.948 * numax**0.992
	lower_nu_g2 = 0.6 * nu_g2 
	upper_nu_g2 = 1.4 * nu_g2

	amp_g2 = 3383 * numax**-0.609
	lower_amp_g2 = 0.2 * amp_g1
	upper_amp_g2 = 1.5 * amp_g1 


	# Define the priors for the rotation
	# Make sure this is not overlapping with the meso-granulation

	nu_rot = nu_g1/2. 
	lower_nu_rot = np.min(freq)
	upper_nu_rot = 0.9 * nu_g1
	
	rot_range = nu_rot * 0.1
	tmp_rot = np.where((freq >= nu_rot - rot_range) & (freq <= nu_rot + rot_range))[0]
	psd_rot = np.max(psd[tmp_rot])
	amp_rot = np.sqrt(psd_rot*nu_rot)/(2*np.sqrt(2))*np.pi
	lower_amp_rot = 0.0
	upper_amp_rot = 1.5 * amp_rot


	# Define the priors for the colored noise

	nu_color = nu_rot*1.5
	lower_nu_color = np.min(freq)
	upper_nu_color = nu_color*1.5 
	amp_color = 0.0
	lower_amp_color = 0.0
	upper_amp_color = 2.0 * amp_rot


	# Checks for no overlap between prior ranges

	if upper_nu_g1 > lower_nu_g2 * 1.2:
		difference = upper_nu_g1 - lower_nu_g2
		upper_nu_g1 -= 0.5 * difference
		lower_nu_g2 += 0.5 * difference
		#print('difference g1-g2 frequency = ', difference)
	elif upper_nu_g1 < lower_nu_g2:
		# Make prior ranges to be contiguous
		upper_nu_g1 = (upper_nu_g1 + lower_nu_g2) / 2.0
		lower_nu_g2 = upper_nu_g1

	if upper_nu_rot > lower_nu_g1 * 1.3:
		difference = upper_nu_rot - lower_nu_g1
		upper_nu_rot -= 0.5 * difference
		lower_nu_g1 += 0.5 * difference
		#print('difference rot-g1 frequency = ', difference)
	elif upper_nu_rot < lower_nu_g1:
		# Make prior ranges to be contiguous
		upper_nu_rot = (upper_nu_rot + lower_nu_g1) / 2.0 
		lower_nu_g1 = upper_nu_rot


	# Resume in two arrays
	#white_noise_prior = [lower_white_noise,upper_white_noise]
	#amp_color_prior = [lower_amp_color,upper_amp_color]
	#nu_color_prior = [lower_nu_color,upper_nu_color]
	#amp_rot_prior = [lower_amp_rot,upper_amp_rot]
	#nu_rot = [lower_nu_rot,upper_nu_rot]
	#amp_g1_prior = [lower_amp_g1,upper_amp_g1]
	#nu_g1_prior = [lower_nu_g1,upper_nu_g1]
	#amp_g2_prior = [lower_amp_g2,upper_amp_g2]
	#nu_g2_prior = [lower_nu_g2,upper_nu_g2]
	#height_prior = [lower_height,upper_height]
	#numax_prior = [lower_numax,upper_numax]
	#sigma_prior = [lower_sigma,upper_sigma]

	if modelname == 'FlatNoGaussian':
		boundaries = [lower_white_noise,upper_white_noise]
	
	if modelname == 'OneHarveyNoGaussian':
		lower_nu_g1 = np.min(freq)
		boundaries = [lower_white_noise, upper_white_noise, lower_amp_g1, upper_amp_g1, lower_nu_g1, upper_nu_g1]
	
	if modelname == 'TwoHarveyNoGaussian':
		lower_nu_g1 = np.min(freq)
		boundaries = [lower_white_noise, upper_white_noise, lower_amp_g1, upper_amp_g1, lower_nu_g1, upper_nu_g1, lower_amp_g2, upper_amp_g2, 
						lower_nu_g2, upper_nu_g2]
	
	if modelname == 'ThreeHarveyNoGaussian':
		boundaries = [lower_white_noise, upper_white_noise, lower_amp_rot, upper_amp_rot, lower_nu_rot, upper_nu_rot, lower_amp_g1, upper_amp_g1, 
						lower_nu_g1, upper_nu_g1, lower_amp_g2, upper_amp_g2, lower_nu_g2, upper_nu_g2]
	
	if modelname == 'Flat':
		boundaries = [lower_white_noise, upper_white_noise, lower_height, upper_height, lower_numax, upper_numax, lower_sigma, upper_sigma]
	
	if modelname == 'OneHarvey':
		lower_nu_g1 = np.min(freq)
		boundaries = [lower_white_noise, upper_white_noise, lower_amp_g1, upper_amp_g1, lower_nu_g1, upper_nu_g1, 
						lower_height, upper_height, lower_numax, upper_numax, lower_sigma, upper_sigma]
	
	if modelname == 'OneHarveyColor':
		lower_nu_g1 = np.min(freq)
		boundaries = [lower_white_noise, upper_white_noise, lower_amp_color, upper_amp_color, lower_nu_color, upper_nu_color, 
						lower_amp_g1, upper_amp_g1, lower_nu_g1, upper_nu_g1, 
						lower_height, upper_height, lower_numax, upper_numax, lower_sigma, upper_sigma]
	
	if modelname == 'TwoHarvey':
		lower_nu_g1 = np.min(freq)
		boundaries = np.array([lower_white_noise, upper_white_noise, lower_amp_g1, upper_amp_g1, lower_nu_g1, upper_nu_g1, lower_amp_g2, upper_amp_g2, 
						lower_nu_g2, upper_nu_g2, lower_height, upper_height, lower_numax, upper_numax, lower_sigma, upper_sigma])
	
	if modelname == 'TwoHarveyColor':
		lower_nu_g1 = np.min(freq)
		boundaries = [lower_white_noise, upper_white_noise, lower_amp_color, upper_amp_color, lower_nu_color, upper_nu_color, 
						lower_amp_g1, upper_amp_g1, lower_nu_g1, upper_nu_g1, lower_amp_g2, upper_amp_g2, 
						lower_nu_g2, upper_nu_g2, lower_height, upper_height, lower_numax, upper_numax, lower_sigma, upper_sigma]

	if modelname == 'ThreeHarvey':
		boundaries = [lower_white_noise, upper_white_noise, lower_amp_rot, upper_amp_rot, lower_nu_rot, upper_nu_rot, 
						lower_amp_g1, upper_amp_g1, lower_nu_g1, upper_nu_g1, lower_amp_g2, upper_amp_g2, 
						lower_nu_g2, upper_nu_g2, lower_height, upper_height, lower_numax, upper_numax, lower_sigma, upper_sigma]
	
	if modelname == 'ThreeHarveyColor':
		boundaries = [lower_white_noise, upper_white_noise, lower_amp_color, upper_amp_color, lower_nu_color, upper_nu_color, 
						lower_amp_rot, upper_amp_rot, lower_nu_rot, upper_nu_rot, lower_amp_g1, upper_amp_g1, lower_nu_g1, upper_nu_g1, 
						lower_amp_g2, upper_amp_g2, lower_nu_g2, upper_nu_g2, lower_height, upper_height, lower_numax, upper_numax, lower_sigma, upper_sigma]


	# Write an ASCII file for the priors
	
	if dir_flag < 10:
		subdir_str = '0' + str(dir_flag)
	else:
		subdir_str = str(dir_flag)

	if not os.path.isdir(output_dir + subdir_str):
		os.mkdir(output_dir + subdir_str)

	filename = output_dir + 'background_hyperParameters_' + subdir_str + '.txt'

	header="""
	Hyper parameters used for setting up uniform priors.
	Each line corresponds to a different free parameter (coordinate).
	Column #1: Minima (lower boundaries)
	Column #2: Maxima (upper boundaries)
	"""

	boundaries = np.reshape(np.array(boundaries),(int(len(boundaries)/2),2))
	np.savetxt(filename, boundaries,fmt='%.3f',header=header)


	# Write an ASCII file for the nested sampler configuring parameters.

	filename = output_dir + 'NSMC_configuringParameters.txt'
	NSMC_array = np.array([500, 500, 50000, 1000, 50, 1.5363, 0.0, 1.0])
	np.savetxt(filename, NSMC_array.T,fmt='%.1f')


	# Write a file with the value of the Nyquist

	filename = output_dir + 'NyquistFrequency.txt'
	os.makedirs(os.path.dirname(filename), exist_ok=True)
	with open(filename, 'w') as f:
		f.write('{}'.format(np.max(freq)))

	#np.savetxt(filename, np.array(np.max(freq)),fmt='%.6f')


	# Write a file with the parameters for the X means clustering algorithm.

	filename = output_dir + 'Xmeans_configuringParameters.txt'
	xmeans_array = np.array([3, 6])
	np.savetxt(filename, xmeans_array.T,fmt='%d')