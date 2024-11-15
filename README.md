# PicoAna, a Simple Tool for PicoScope Data Analysis

## Authors
Zhiyu ZHAO (TDLI/SJTU; <zhiyuzhao@sjtu.edu.cn>; the main author)

Ji-Yuan CHEN (SJTU; <jy_chen@sjtu.edu.cn>; the author of modifications and refactoring)

## Description
This program is designed for analysing data generated by PicoScope 6. By extracting the waveforms and integrating, we can obtain various properties of the photo-electric devices (e.g. APDs, SiPMs) and scintillators (e.g. crystals, plastic scintillators). Specifically, dark count and single-photon response of SiPMs, light yield and time resolution of crystals, etc.

## Usage
At first, you can run
```shell
iAna --help
```
to display help information. For more detail, please refer to the following instructions. :stuck_out_tongue:

### Data Preparation
The PSDATA files should be converted to CSV files before analysis, using the interface provided by PicoScope; meanwhile, a directory with the same name will be created.

### Data Conversion
In the directory mentioned above, thousands of CSV files can be found. Each file contains the waveform of an event, and all the waveforms can be transferred into a ROOT file by running
```shell
iAna -a -p [path]    # 'a' stands for 'analyse'
```
where `path` is the path to this directory. Then, a ROOT file with the same file name will be created in the same directory as the directory that contains all the CSV files. For event checking, the event ID has also been saved.

**Notice:** this step is time-consuming, which requires a few minutes.

### Saving information of ADC and NPE (Number of Photo-Electrons)
With the ROOT file created, we can calculate the ADC value and convert it to NPE. To do this, run
```shell
iAna -s -p [file] -pe [pedestal_end] -ib [integral_begin] -ie [integral_end]    # 's' stands for 'save'
```
where the values of `pedestal_end`, `integral_begin` and `integral_end` are mandatory and should be read from the waveforms. Open the PSDATA file to have a look.

After this, a ROOT file with prefix ‘ADC’ will be created in the same directory as the input ROOT file.

**Notice:** after changing the configuration (especially the SiPM and pre-amplifier), remember to modify `ADC_constant` in file `Convert.cpp`!

### Peak Finding and Global Fitting
This is mainly used in single-photon calibration of SiPMs, using the output file from the above step. This can be done with
```shell
iAna -f -p [file]    # 'f' stands for 'fit'
```
Then, two figures will be displayed, one for peak finding and Gaussian fitting (to be more precise, the sum of several Gaussian functions), and the other for linear fitting (to show the contribution per photon).

**Notice:** due to unknown issues, the results might be different every time you run the above command, and sometimes plotting even fails. This is a bug to be fixed in the future…

## Environment Set-up
This project requires CMake version >= 3.16. Notice that different ROOT versions lead to different shapes of the histograms!

If you are working on the cluster of INPAC, IHEP, etc., the environment can be easily set up by simply executing
```shell
source source /cvmfs/sft.cern.ch/lcg/views/LCG_103/x86_64-centos7-gcc11-opt/setup.sh
```
(This command has been commented out in `setup.sh`, but feel free to uncomment it while necessary.)

With the environment set up, this project can be cloned from GitHub and compiled as usual:
```shell
git clone git@github.com:phys-jychen/PicoAna.git
cd PicoAna
mkdir build
cd build
cmake ..
make -j    # Just do it!
source setup.sh
```

Every time you log in (be it to a cluster or your own machine), and right before the first time of running this program, remember to execute
```shell
source <build_dir>/setup.sh
```

By now, the compilation has finished. Prepare the data, and have fun! :relaxed:

## Change Log

### 30 July 2024 (Version 1.0.0)

Modified the conversion process, and made sure that all CSV files are converted to ROOT.

### 5 August 2024 (Version 1.0.1)

- Added exception handling.
- Added two options for setting the range of the NPE histogram.

### 15 November 2024

- **Version 1.0.2**:
  - Added a flag for setting the bins in filling the NPE histogram.
- **Version 2.0.0**:
  - Removed the step of displaying NPE histogram, and replaced it with saving the ADC and NPE values in a new ROOT file.

## To-Do
- Skip the empty channels.
- The fitting should be carefully dealt with.
- Replace plotting histograms and fitting with saving data to a ROOT file (in progress).
- Some more functions might be added to this framework.