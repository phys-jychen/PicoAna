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

### Plotting of NPE (Number of Photo-Electrons) Histogram
With the ROOT file created, we can plot the distribution of NPE. To do this, run
```shell
iAna -d -p [file] -pe [pedestal_end] -ib [integral_begin] -ie [integral_end]    # 'd' stands for 'draw'
```
where the values of `pedestal_end`, `integral_begin` and `integral_end` should be read from the waveforms.

Or, if you want to save the histogram to another ROOT file, run
```shell
iAna -d -p [file] -pe [pedestal_end] -ib [integral_begin] -ie [integral_end] -o [output_file]
```

**Notice:** after changing the configuration (especially the SiPM and pre-amplifier), remember to modify `ADC_constant` in file `Convert.cpp`! Besides, in some cases, you might also need to modify the lower and upper limits of the NPE histogram. This can also be done in `Convert.cpp`.

### Peak Finding and Global Fitting
This is mainly used in single-photon calibration of SiPMs, using the output file from the above step. This can be done with
```shell
iAna -f -p [file]    # 'f' stands for 'fit'
```
Then, two figures will be displayed, one for peak finding and Gaussian fitting (to be more precise, the sum of several Gaussian functions), and the other for linear fitting (to show the contribution per photon).

**Notice:** due to unknown issues, the results might be different every time you run the above command, and sometimes plotting even fails. This is a bug to be fixed in the future…

## Environment Set-up
This project requires CMake version >= 3.16. Notice that different ROOT versions lead to different shapes of the histograms!

With the environment set up, this project can be cloned from GitHub and compiled as usual:
```shell
git clone git@github.com:phys-jychen/PicoAna.git
cd PicoAna
mkdir build
cd build
cmake ..
make -j
source setup.sh
```

Every time you log in (be it to a cluster or your own machine), and right before the first time of running this program, remember to execute
```shell
source <build_dir>/setup.sh
```

By now, the compilation has finished. Prepare the data, and have fun! :relaxed:

## Change Log

### 30 July 2024

Modified the conversion process, and made sure that all CSV files are converted to ROOT.

### 5 August 2024

Added exception handling.

## To-Do
The fitting should be carefully dealt with and some more functions might be added to this framework.