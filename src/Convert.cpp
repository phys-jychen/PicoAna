#include "Ana.h"

// ADC constant, determined by the SiPM and pre-amplifier
const Double_t ADC_constant = 124.6 / 19.41;

Double_t Ana::GetADC(const vector<Double_t>& Time, const vector<Double_t>& Ch, const Double_t& pedestal_end, const Double_t& integral_begin, const Double_t& integral_end)
{
    assert(Time.size() == Ch.size());

    Double_t ADC = 0;
    Double_t BaseLine = 0;
    Int_t Baseline_count = 0;

    for (Int_t i = 0; i < Time.size(); i++)
    {
        if (Time.at(i) < pedestal_end)
        {
            BaseLine += Ch.at(i);
            ++Baseline_count;
        }
        else if (Time.at(i) >= integral_begin && Time.at(i) < integral_end)
            ADC += Ch.at(i) * (Time.at(i) - Time.at(i - 1));
    }

    ADC -= BaseLine * (integral_end - integral_begin) / Baseline_count;
    return ADC;
}

Int_t Ana::Convert(const string& path)
{
    string line;
    ifstream DataFile;

    DIR* dp;
    struct dirent* dirp;
    size_t found = path.find_last_of('/');
    const string file_path = (found == path.length() - 1) ? path.substr(0, found) : path;
    const TString rootname = file_path + ".root";

    if (!(dp = opendir(path.c_str())))
    {
        cout << "*** Path " << path << " does not exist!" << endl;
        throw;
    }

    TFile* rootfile = TFile::Open(rootname, "RECREATE");
    TTree* DataInput = new TTree("DataInput", "Waveform");
    TTree* TimeInput = new TTree("TimeInput", "Time information");

    vector<Double_t> Time;
    Int_t EventID;
    vector<Double_t> Ch1;
    vector<Double_t> Ch2;
    vector<Double_t> Ch3;
    vector<Double_t> Ch4;
    vector<Double_t> unit_Volt(4, 0.0);
    Double_t unit_Time = 0;

    TimeInput->Branch("Time", &Time);
    DataInput->Branch("EventID", &EventID);
    DataInput->Branch("Ch1", &Ch1);
    DataInput->Branch("Ch2", &Ch2);
    DataInput->Branch("Ch3", &Ch3);
    DataInput->Branch("Ch4", &Ch4);

    const Int_t skip_line = 3;
    string temp_string;
    Double_t temp;

    // Time and waveform
    for (Int_t i = 0; i < 3; ++i)
    {
        dirp = readdir(dp);

        if ((Int_t) dirp->d_type == DT_REG)
        {
            DataFile.clear();
            DataFile.open(file_path + "/" + dirp->d_name);
            if (!DataFile)
                continue;

            const Int_t pos_id_begin = string(dirp->d_name).find_last_of('_') + 1;
            const Int_t pos_id_end = string(dirp->d_name).find_last_of('.');
            EventID = stoi(string(dirp->d_name).substr(pos_id_begin, pos_id_end - pos_id_begin));

            for (Int_t j = 0; j < skip_line; ++j)
            {
                getline(DataFile, line);

                if (j == 1)
                {
                    istringstream DataLine(line);
                    istringstream is;
                    string str_column;

                    getline(DataLine, temp_string, ',');
                    is.str(temp_string);
                    is >> str_column;
                    unit_Time = (str_column == "(ns)") ? 1 : (str_column == "(us)") ? 1000 : 0;
                    is.clear();

                    for (Int_t k = 0; k < unit_Volt.size(); ++k)
                    {
                        if (k < unit_Volt.size() - 1)
                            getline(DataLine, temp_string, ',');
                        else
                            getline(DataLine, temp_string);
                        is.str(temp_string);
                        is >> str_column;
                        unit_Volt.at(k) = (str_column == "(mV)") ? 1 : (str_column == "(V)") ? 1000 : 0;
                        is.clear();
                    }
                }
            }

            while (getline(DataFile, line))
            {
                istringstream DataLine(line);
                istringstream is;

                getline(DataLine, temp_string, ',');
                is.str(temp_string);
                is >> temp;
                Time.emplace_back(temp * unit_Time);
                getline(DataLine, temp_string);
                is.clear();

                // Channel 1
                getline(DataLine, temp_string, ',');
                is.str(temp_string);
                is >> temp;
                Ch1.emplace_back(temp * unit_Volt.at(0));
                is.clear();

                // Channel 2
                getline(DataLine, temp_string, ',');
                is.str(temp_string);
                is >> temp;
                Ch2.emplace_back(temp * unit_Volt.at(1));
                is.clear();

                // Channel 3
                getline(DataLine, temp_string, ',');
                is.str(temp_string);
                is >> temp;
                Ch3.emplace_back(temp * unit_Volt.at(2));
                is.clear();

                // Channel 4
                getline(DataLine, temp_string);
                is.str(temp_string);
                is >> temp;
                Ch4.emplace_back(temp * unit_Volt.at(3));
                is.clear();
            }

            TimeInput->Fill();
            Time.clear();
            DataInput->Fill();
            Ch1.clear();
            Ch2.clear();
            Ch3.clear();
            Ch4.clear();
            DataFile.close();
        }
    }

    rootfile->cd();
    TimeInput->Write("", TObject::kWriteDelete);
    delete TimeInput;

    // Waveform
    while ((dirp = readdir(dp)) != nullptr)
        if ((Int_t) dirp->d_type == DT_REG)
        {
            DataFile.clear();
            DataFile.open(file_path + "/" + dirp->d_name);
            if (!DataFile)
                continue;

            const Int_t pos_id_begin = string(dirp->d_name).find_last_of('_') + 1;
            const Int_t pos_id_end = string(dirp->d_name).find_last_of('.');
            EventID = stoi(string(dirp->d_name).substr(pos_id_begin, pos_id_end - pos_id_begin));

            for (Int_t i = 0; i < skip_line; ++i)
                getline(DataFile, line);

            while (getline(DataFile, line))
            {
                istringstream DataLine(line);
                istringstream is;
                getline(DataLine, temp_string, ',');

                // Channel 1
                getline(DataLine, temp_string, ',');
                is.str(temp_string);
                is >> temp;
                Ch1.emplace_back(temp * unit_Volt.at(0));
                is.clear();

                // Channel 2
                getline(DataLine, temp_string, ',');
                is.str(temp_string);
                is >> temp;
                Ch2.emplace_back(temp * unit_Volt.at(1));
                is.clear();

                // Channel 3
                getline(DataLine, temp_string, ',');
                is.str(temp_string);
                is >> temp;
                Ch3.emplace_back(temp * unit_Volt.at(2));
                is.clear();

                // Channel 4
                getline(DataLine, temp_string);
                is.str(temp_string);
                is >> temp;
                Ch4.emplace_back(temp * unit_Volt.at(3));
                is.clear();
            }

            DataInput->Fill();
            Ch1.clear();
            Ch2.clear();
            Ch3.clear();
            Ch4.clear();
            DataFile.close();
        }

    rootfile->cd();
    DataInput->Write("", TObject::kWriteDelete);
    delete DataInput;
    rootfile->Close();

    return 0;
}

Int_t Ana::Draw(const string& file, const Double_t& pedestal_end, const Double_t& integral_begin, const Double_t& integral_end, const Double_t& histogram_begin, const Double_t& histogram_end, const string& output)
{
    if (integral_begin > integral_end)
    {
        cout << "Error in integral range! Make sure that end > begin!" << endl;
        throw;
    }

    if (histogram_begin > histogram_end)
    {
        cout << "Error in histogram range! Make sure that end > begin!" << endl;
        throw;
    }

    gStyle->SetOptStat(1111);

    TFile* inputfile = new TFile((TString) file, "READ");
    if (inputfile->IsZombie())
    {
        inputfile->Close();
        throw;
    }

    TTree* timetree = inputfile->Get<TTree>("TimeInput");
    TTree* datatree = inputfile->Get<TTree>("DataInput");

    vector<Double_t>* Time = nullptr;
    vector<Double_t>* Ch1 = nullptr;
    timetree->SetBranchAddress("Time", &Time);
    datatree->SetBranchAddress("Ch2", &Ch1);

    timetree->GetEntry(0);

    const Int_t EventNum = datatree->GetEntriesFast();

    TH1D* h_QDC_Ch1 = new TH1D("h_QDC_Ch1", ";Number of PE;Entries", 200, histogram_begin, histogram_end);

    for (Int_t i = 0; i < EventNum; ++i)
    {
//        if (i % 1000 == 0)
//            cout << "Analysis of event " << setw(5) << i << " begins" << endl;
        datatree->GetEntry(i);
        const Double_t ADC_Ch1 = GetADC(*Time, *Ch1, pedestal_end, integral_begin, integral_end);
        h_QDC_Ch1->Fill(ADC_Ch1 / ADC_constant);
    }

    TApplication* app = new TApplication("app", nullptr, nullptr);
    TCanvas* c1 = new TCanvas("c1", "c1");
    h_QDC_Ch1->SetTitle(";NPE;Entries");
    h_QDC_Ch1->Draw();
//    c1->Print((TString) file.substr(0, file.find_last_of('.')) + ".pdf");
    c1->Update();
    cout << "---> NPE histogram plotted!" << endl;

    TRootCanvas* rc = (TRootCanvas*) c1->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

    if (!output.empty())
    {
        TFile* rootfile = new TFile((TString) output, "RECREATE");
        rootfile->cd();
        h_QDC_Ch1->Write("", TObject::kWriteDelete);
        rootfile->Close();
        cout << "---> ROOT file " << output << " saved!" << endl;
    }

    app->Run();
    return 0;
}