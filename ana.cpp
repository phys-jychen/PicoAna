#include<Ana.h>

Int_t main(Int_t argc, Char_t* argv[])
{
    string path;
    Int_t ana = 0;
    Int_t save = 0;
    Double_t pedestal_end = numeric_limits<Double_t>::quiet_NaN();
    Double_t integral_begin = numeric_limits<Double_t>::quiet_NaN();
    Double_t integral_end = numeric_limits<Double_t>::quiet_NaN();
    Int_t fit = 0;
    Int_t help = 0;

    for (Int_t i = 1; i < argc; ++i)
    {
        if (string(argv[i]) == "--help")
        {
            help = 1;
            cout << endl;
            cout << "****************************************************************" << endl;
            cout << "                        Help Information" << endl;
            cout << "****************************************************************" << endl;
            cout << " Convert CSV files to ROOT:" << endl;
            cout << "     iAna -a -p [path]" << endl;
            cout << " Save ADC and NPE information:" << endl;
            cout << "     iAna -s -p [file] -pe [pedestal_end] -ib [integral_begin] -ie [integral_end]" << endl;
            cout << " Fitting the Gaussian peaks:" << endl;
            cout << "     iAna -f -p [file]" << endl;
            cout << "****************************************************************" << endl << endl;
            break;
        }

        else if (string(argv[i]) == "-p")
            path = string(argv[i + 1]);

        else if (string(argv[i]) == "-a")
            ana = 1;

        else if (string(argv[i]) == "-s")
            save = 1;

        else if (string(argv[i]) == "-pe")
            pedestal_end = stod(argv[i + 1]);

        else if (string(argv[i]) == "-ib")
            integral_begin = stod(argv[i + 1]);

        else if (string(argv[i]) == "-ie")
            integral_end = stod(argv[i + 1]);

        else if (string(argv[i]) == "-f")
            fit = 1;
    }

    if (ana == 1 && !path.empty())
    {
        cout << "--> Converting CSV files to ROOT..." << endl;
        cout << "--> Path: " << path << endl;
        Ana::Convert(path);
        cout << "--> CSV conversion finished!" << endl;
    }

    else if (save == 1 && !path.empty() && !isnan(pedestal_end) && !isnan(integral_begin) && !isnan(integral_end))
    {
        cout << "---> Saving ADC and NPE information..." << endl;
        cout << "---> File: " << path << endl;
        Ana::Save(path, pedestal_end, integral_begin, integral_end);
        cout << "---> Information of ADC and NPE saved!" << endl;
    }

    else if (fit == 1 && !path.empty())
    {
        cout << "----> Fitting Gaussian peaks..." << endl;
        cout << "----> File: " << path << endl;
        Ana::GlobalFit(path);
    }

    else if (help == 0)
    {
        cout << "Invalid input." << endl;
        cout << "Run \'iAna --help\' to display help information." << endl << endl;
    }

    return 0;
}