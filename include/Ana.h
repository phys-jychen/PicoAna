#ifndef ANA_HH
#define ANA_HH

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <dirent.h>
#include <limits>
#include <cassert>
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TObject.h"
#include "TQObject.h"
#include "TMath.h"
#include "TString.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TH1D.h"
#include "TFitResultPtr.h"
#include "TSpectrum.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TVirtualFitter.h"

using namespace std;
using namespace ROOT;
using namespace TMath;

class Ana
{
public:
    static Int_t Convert(const string& path);

    static Int_t Draw(const string& file, const Double_t& pedestal_end, const Double_t& integral_begin, const Double_t& integral_end, const string& output = "");

    static Int_t GlobalFit(const string& file);

private:
    static Double_t GetADC(const vector<Double_t>& Time, const vector<Double_t>& Ch1, const Double_t& pedestal_end, const Double_t& integral_begin, const Double_t& integral_end);

    static Double_t FindPeaks(const Double_t* x, const Double_t* par);

    static const Int_t npeaks = 10;
};

#endif