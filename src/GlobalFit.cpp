#include "Ana.h"

Double_t Ana::FindPeaks(const Double_t* x, const Double_t* par)
{
    Double_t result = 0;
    for (Int_t p = 0; p < npeaks; ++p)
    {
        const Double_t norm = par[3 * p];
        const Double_t mean = par[3 * p + 1];
        const Double_t sigma = par[3 * p + 2];
        result += norm * Gaus(x[0], mean, sigma);
    }
    return result;
}

Int_t Ana::GlobalFit(const string& file)
{
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);

    TFile* rootfile = TFile::Open((TString) file);
    TH1D* h_QDC = rootfile->Get<TH1D>("h_QDC_Ch1");

    TApplication* app = new TApplication("app", nullptr, nullptr);
    TCanvas* c1 = new TCanvas("c1", "c1");
    c1->SetTickx();
    c1->SetTicky();

    TSpectrum* s = new TSpectrum(npeaks);
    const Int_t nfound = s->Search(h_QDC, 2, "", 0.05);
    TGraphErrors* g_peak = new TGraphErrors(nfound);
    Double_t* xpeaks = s->GetPositionX();
    sort(xpeaks, xpeaks + nfound);

    TF1* f_multiGaus = new TF1("f_multiGaus", FindPeaks, 0, 1000, 3 * nfound);
    Double_t par[3 * nfound];

    for (Int_t n = 0; n < nfound; ++n)
    {
        const Double_t xp = xpeaks[n];
        const Int_t bin = h_QDC->GetXaxis()->FindBin(xp);
        const Double_t yp = h_QDC->GetBinContent(bin);
        par[3 * n] = yp;    // Height
        par[3 * n + 1] = xp;    // Mean
        par[3 * n + 2] = 4;    // Sigma
        f_multiGaus->SetParLimits(3 * n + 1, xp - 20, xp + 20);
        f_multiGaus->SetParLimits(3 * n + 2, 0, 6);
    }

    h_QDC->SetTitle(";QDC [ns*mV];Entries");
    h_QDC->Draw();

    TVirtualFitter::Fitter(h_QDC);
    f_multiGaus->SetParameters(par);
    f_multiGaus->SetNpx(1000);
    h_QDC->Fit(f_multiGaus, "R", "", -100, 500);
    f_multiGaus->Draw("same");
    c1->Update();
    cout << "----> Gaussian fitting finished!" << endl;

    TRootCanvas* rc1 = (TRootCanvas*) c1->GetCanvasImp();
    rc1->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

    for (Int_t n = 0; n < nfound; ++n)
    {
        g_peak->SetPoint(n, n, par[3 * n + 1]);
        g_peak->SetPointError(n, 0, 0);
    }

    TCanvas* c2 = new TCanvas("c2", "c2");
    c2->SetLeftMargin(0.15);
    g_peak->SetMarkerStyle(20);
    g_peak->Fit("pol1");
    g_peak->SetTitle(";NPE;QDC [ns*mV]");
    g_peak->Draw("APE");
    c2->Update();
    cout << "----> Linear fitting finished!" << endl;

    TRootCanvas* rc2 = (TRootCanvas*) c2->GetCanvasImp();
    rc2->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

    app->Run();
    return 0;
}