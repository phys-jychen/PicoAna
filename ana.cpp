#include<Ana.h>

Int_t main(Int_t argc, Char_t* argv[])
{
    string path, output;
    Int_t ana = 0;
    Int_t draw = 0;
    Double_t pedestal_end = numeric_limits<Double_t>::quiet_NaN();
    Double_t integral_begin = numeric_limits<Double_t>::quiet_NaN();
    Double_t integral_end = numeric_limits<Double_t>::quiet_NaN();
    Int_t histogram_nbins = 100;
    Double_t histogram_begin = 0;
    Double_t histogram_end = 0;
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
            cout << " Plotting the NPE histogram:" << endl;
            cout << "     iAna -d -p [file] -pe [pedestal_end] -ib [integral_begin] -ie [integral_end]" << endl;
            cout << "          Optional: -hn [histogram_bins] -hb [histogram_begin] -he [histogram_end] -o [output_file]" << endl;
            cout << " Fitting the Gaussian peaks:" << endl;
            cout << "     iAna -f -p [file]" << endl;
            cout << "****************************************************************" << endl << endl;
            break;
        }

        else if (string(argv[i]) == "-p")
            path = string(argv[i + 1]);

        else if (string(argv[i]) == "-a")
            ana = 1;

        else if (string(argv[i]) == "-d")
            draw = 1;

        else if (string(argv[i]) == "-pe")
            pedestal_end = stod(argv[i + 1]);

        else if (string(argv[i]) == "-ib")
            integral_begin = stod(argv[i + 1]);

        else if (string(argv[i]) == "-ie")
            integral_end = stod(argv[i + 1]);

        else if (string(argv[i]) == "-hn")
            histogram_nbins = stoi(argv[i + 1]);

        else if (string(argv[i]) == "-hb")
            histogram_begin = stod(argv[i + 1]);

        else if (string(argv[i]) == "-he")
            histogram_end = stod(argv[i + 1]);

        else if (string(argv[i]) == "-o")
            output = string(argv[i + 1]);

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

    else if (draw == 1 && !path.empty() && !isnan(pedestal_end) && !isnan(integral_begin) && !isnan(integral_end))
    {
        cout << "---> Plotting NPE histogram..." << endl;
        cout << "---> File: " << path << endl;
        Ana::Draw(path, pedestal_end, integral_begin, integral_end, histogram_nbins, histogram_begin, histogram_end, output);
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