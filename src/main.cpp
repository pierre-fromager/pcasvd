
#include <main.h>
#include <data.h>
#include <colors.h>
#include <colormap.h>
#include <display.h>
#include <alglib/statistics.h>
#include <alglib/dataanalysis.h>

typedef std::vector<std::vector<double>> mat_dble;

template <typename T, ui_t NC, ui_t NR>
struct fixt_s
{
    ui_t nbcol = NC;
    ui_t nbrow = NR;
    std::array<T, NC * NR> values;
};

static void fix_2x12(fixt_s<double, 2, 12> *fix)
{
    fix->nbcol = 2;
    fix->nbrow = 12;
    fix->values = {33.0, 80.0,
                   33.0, 82.5,
                   34.0, 100.8,
                   42.0, 90.0,
                   29.0, 67.0,
                   19.0, 60.0,
                   50.0, 77.0,
                   55.0, 77.0,
                   31.0, 87.0,
                   46.0, 70.0,
                   36.0, 57.0,
                   48.0, 64.0};
}

static void fix_csv_4x12(std::string filename, fixt_s<double, 4, 12> *fix)
{
    Data::File::Csv<double> *csv = new Data::File::Csv<double>(SEMICOLON);
    csv->load(filename, 1);
    Data::File::metas_t metas = csv->metas();
    fix->nbcol = metas.cols;
    fix->nbrow = metas.rows;
    const ui_t nbItems = fix->nbcol * fix->nbrow;
    std::copy_n(csv->buffer().begin(), nbItems, fix->values.begin());
    delete (csv);
}

static void fix_csv_bovin_6x23(std::string filename, fixt_s<double, 6, 23> *fix)
{
    Data::File::Csv<double> *csv = new Data::File::Csv<double>(SEMICOLON);
    csv->load(filename, 1);
    Data::File::metas_t metas = csv->metas();
    fix->nbcol = metas.cols;
    fix->nbrow = metas.rows;
    const ui_t nbItems = fix->nbcol * fix->nbrow;
    std::copy_n(csv->buffer().begin(), nbItems, fix->values.begin());
    delete (csv);
}

static void fix_csv_species_4x150(std::string filename, fixt_s<double, 4, 150> *fix)
{
    Data::File::Csv<double> *csv = new Data::File::Csv<double>(COMA);
    csv->load(filename, 1);
    Data::File::metas_t metas = csv->metas();
    fix->nbcol = metas.cols;
    fix->nbrow = metas.rows;
    const ui_t nbItems = fix->nbcol * fix->nbrow;
    std::copy_n(csv->buffer().begin(), nbItems, fix->values.begin());
    delete (csv);
}

static bool train(mat_dble inputData, int pDim, std::string outputFile, Display *disp)
{
    if (inputData.size() < 1 || inputData[0].size() < 1)
        return false;
    int numSamples = (int)inputData.size();
    int oDim = (int)inputData[0].size();
    double *dataMat = new double[numSamples * oDim];
    double *aveVec = new double[oDim];
    int i, j;
    for (i = 0; i < oDim; i++)
        aveVec[i] = 0.0;
    for (i = 0; i < numSamples; i++)
        for (j = 0; j < oDim; j++)
        {
            dataMat[i * oDim + j] = inputData[i][j];
            aveVec[j] += inputData[i][j];
        }
    for (i = 0; i < oDim; i++)
        aveVec[i] /= (double)numSamples;
    //disp->vec("Debug pseudo cov", dataMat, oDim * oDim);
    for (i = 0; i < oDim * oDim; i++)
    {
        if (i % oDim == 0)
            std::cout << TAB << std::endl;
        std::cout << dataMat[i] << "   ";
    }
    std::cout << std::endl;

    alglib::real_2d_array ptInput;
    ptInput.setcontent(numSamples, oDim, dataMat);
    alglib::ae_int_t info;
    alglib::real_1d_array eigValues;
    alglib::real_2d_array eigVectors;
    pcabuildbasis(ptInput, numSamples, oDim, info, eigValues, eigVectors);
    disp->mat("Debug eig Vectors", eigVectors, 4, 4);
    disp->vec("Debug eig Values", eigValues, 4);
    delete[] dataMat;
    alglib::ae_int_t cols = (int)eigVectors.cols();
    if (cols < pDim)
        pDim = cols;
    double *projMat = new double[pDim * oDim];
    double *eigVec = new double[pDim];
    for (i = 0; i < pDim; i++)
    {
        eigVec[i] = 1 / sqrt(eigValues[i]);
        for (j = 0; j < oDim; j++)
            projMat[i * oDim + j] = eigVectors[j][i];
    }
    // save to file
    ofstream fout;
    fout.open(outputFile.c_str());
    if (!fout.is_open())
    {
        cout << "Cannot save projection matrix to " << outputFile << endl;
        return false;
    }
    fout << oDim << " " << pDim << std::endl;
    fout << projMat[0];
    for (i = 1; i < oDim * pDim; i++)
        fout << " " << projMat[i];
    fout << endl;
    fout << aveVec[0];
    for (i = 1; i < oDim; i++)
        fout << " " << aveVec[i];
    fout << endl;
    fout << eigVec[0];
    for (i = 1; i < pDim; i++)
        fout << " " << eigVec[i];
    fout << endl;
    fout.close();

    return true;
}

template <typename T, ui_t NC, ui_t NR>
static void pca(fixt_s<T, NC, NR> fix, Display *disp, bool oncov)
{
    try
    {
        const ui_t r = fix.nbrow;
        const ui_t c = fix.nbcol;
        const ui_t maxrow = 5;
        alglib::real_2d_array ptInput, mcov, mcorr;
        ptInput.setcontent(r, c, (T *)&fix.values);
        disp->mat(FIXTURE_DATA_TITLE, ptInput, r, c, maxrow);
        alglib::covm(ptInput, r, c, mcov);
        disp->mat(COV_MAT_TITLE, mcov, c, c);
        alglib::pearsoncorrm(ptInput, r, c, mcorr);
        disp->mat(COR_MAT_TITLE, mcorr, c, c);

        alglib::ae_int_t info;
        alglib::real_1d_array eigValues;
        alglib::real_2d_array eigVectors;
        const std::string runonmsg = (oncov) ? "cov" : "cor";
        std::cout << TAB << "Running on " << runonmsg << std::endl;
        // in contrary of python pcabuildbasis operates reduction from raw datas
        if (oncov)
            alglib::pcabuildbasis(ptInput, r, c, info, eigValues, eigVectors);
            //alglib::pcabuildbasis(mcov, c, c, info, eigValues, eigVectors);
        else
            alglib::pcabuildbasis(mcorr, c, c, info, eigValues, eigVectors);
        disp->mat(PCA_EIGEN_VECTORS_TITLE, eigVectors, c, c);
        disp->vec(PCA_EIGEN_VALUES_TITLE, eigValues, c);

        //train()
        /*
        T eigvaSum = 0;
        for (ui_t i = 0; i < c; i++)
            eigvaSum += eigValues[i];

        disp->subtitle("Explained variance (%)");
        for (ui_t i = 0; i < c; i++)
            std::cout << TAB TAB << "P" << i << SPACE
                      << (eigValues[i] * 100 / eigvaSum)
                      << std::endl;

        alglib::real_1d_array w;
        alglib::fisherlda(ptInput, r, c, c, info, w);
        disp->vec("Lda", w, c);*/
    }
    catch (alglib::ap_error e)
    {
        disp->error(ALGLIB_ERR_MSG + e.msg);
    }
}

static void init_colormap(colormap_t *colormap)
{
    const Colors::Define c_title(Colors::Id::FG_GREEN);
    const Colors::Define c_sub_title(Colors::Id::FG_CYAN);
    const Colors::Define c_values(Colors::Id::FG_WHITE);
    const Colors::Define c_error(Colors::Id::FG_RED);
    const Colors::Define c_reset(Colors::Id::RESET);
    Colors::Define &ctitle = const_cast<Colors::Define &>(c_title);
    Colors::Define &csubtitle = const_cast<Colors::Define &>(c_sub_title);
    Colors::Define &cvalues = const_cast<Colors::Define &>(c_values);
    Colors::Define &cerror = const_cast<Colors::Define &>(c_error);
    Colors::Define &creset = const_cast<Colors::Define &>(c_reset);
    colormap->main_title = ctitle.to_str();
    colormap->sub_title = csubtitle.to_str();
    colormap->values = cvalues.to_str();
    colormap->reset = creset.to_str();
}

int main(int argc, char **argv)
{

    mat_dble datas = {
        {5.1, 3.5, 1.4, 0.2},
        {4.9, 3.0, 1.4, 0.2},
        {4.7, 3.2, 1.3, 0.2},
        {4.6, 3.1, 1.5, 0.2},
        {5.0, 3.6, 1.4, 0.2},
        {5.4, 3.9, 1.7, 0.4},
        {4.6, 3.4, 1.4, 0.3},
        {5.0, 3.4, 1.5, 0.2},
        {4.4, 2.9, 1.4, 0.2},
        {4.9, 3.1, 1.5, 0.1},
        {5.4, 3.7, 1.5, 0.2},
        {4.8, 3.4, 1.6, 0.2},
        {4.8, 3.0, 1.4, 0.1},
        {4.3, 3.0, 1.1, 0.1},
        {5.8, 4.0, 1.2, 0.2},
        {5.7, 4.4, 1.5, 0.4},
        {5.4, 3.9, 1.3, 0.4},
        {5.1, 3.5, 1.4, 0.3},
        {5.7, 3.8, 1.7, 0.3},
        {5.1, 3.8, 1.5, 0.3},
        {5.4, 3.4, 1.7, 0.2},
        {5.1, 3.7, 1.5, 0.4},
        {4.6, 3.6, 1.0, 0.2},
        {5.1, 3.3, 1.7, 0.5},
        {4.8, 3.4, 1.9, 0.2},
        {5.0, 3.0, 1.6, 0.2},
        {5.0, 3.4, 1.6, 0.4},
        {5.2, 3.5, 1.5, 0.2},
        {5.2, 3.4, 1.4, 0.2},
        {4.7, 3.2, 1.6, 0.2},
        {4.8, 3.1, 1.6, 0.2},
        {5.4, 3.4, 1.5, 0.4},
        {5.2, 4.1, 1.5, 0.1},
        {5.5, 4.2, 1.4, 0.2},
        {4.9, 3.1, 1.5, 0.2},
        {5.0, 3.2, 1.2, 0.2},
        {5.5, 3.5, 1.3, 0.2},
        {4.9, 3.6, 1.4, 0.1},
        {4.4, 3.0, 1.3, 0.2},
        {5.1, 3.4, 1.5, 0.2},
        {5.0, 3.5, 1.3, 0.3},
        {4.5, 2.3, 1.3, 0.3},
        {4.4, 3.2, 1.3, 0.2},
        {5.0, 3.5, 1.6, 0.6},
        {5.1, 3.8, 1.9, 0.4},
        {4.8, 3.0, 1.4, 0.3},
        {5.1, 3.8, 1.6, 0.2},
        {4.6, 3.2, 1.4, 0.2},
        {5.3, 3.7, 1.5, 0.2},
        {5.0, 3.3, 1.4, 0.2},
        {7.0, 3.2, 4.7, 1.4},
        {6.4, 3.2, 4.5, 1.5},
        {6.9, 3.1, 4.9, 1.5},
        {5.5, 2.3, 4.0, 1.3},
        {6.5, 2.8, 4.6, 1.5},
        {5.7, 2.8, 4.5, 1.3},
        {6.3, 3.3, 4.7, 1.6},
        {4.9, 2.4, 3.3, 1.0},
        {6.6, 2.9, 4.6, 1.3},
        {5.2, 2.7, 3.9, 1.4},
        {5.0, 2.0, 3.5, 1.0},
        {5.9, 3.0, 4.2, 1.5},
        {6.0, 2.2, 4.0, 1.0},
        {6.1, 2.9, 4.7, 1.4},
        {5.6, 2.9, 3.6, 1.3},
        {6.7, 3.1, 4.4, 1.4},
        {5.6, 3.0, 4.5, 1.5},
        {5.8, 2.7, 4.1, 1.0},
        {6.2, 2.2, 4.5, 1.5},
        {5.6, 2.5, 3.9, 1.1},
        {5.9, 3.2, 4.8, 1.8},
        {6.1, 2.8, 4.0, 1.3},
        {6.3, 2.5, 4.9, 1.5},
        {6.1, 2.8, 4.7, 1.2},
        {6.4, 2.9, 4.3, 1.3},
        {6.6, 3.0, 4.4, 1.4},
        {6.8, 2.8, 4.8, 1.4},
        {6.7, 3.0, 5.0, 1.7},
        {6.0, 2.9, 4.5, 1.5},
        {5.7, 2.6, 3.5, 1.0},
        {5.5, 2.4, 3.8, 1.1},
        {5.5, 2.4, 3.7, 1.0},
        {5.8, 2.7, 3.9, 1.2},
        {6.0, 2.7, 5.1, 1.6},
        {5.4, 3.0, 4.5, 1.5},
        {6.0, 3.4, 4.5, 1.6},
        {6.7, 3.1, 4.7, 1.5},
        {6.3, 2.3, 4.4, 1.3},
        {5.6, 3.0, 4.1, 1.3},
        {5.5, 2.5, 4.0, 1.3},
        {5.5, 2.6, 4.4, 1.2},
        {6.1, 3.0, 4.6, 1.4},
        {5.8, 2.6, 4.0, 1.2},
        {5.0, 2.3, 3.3, 1.0},
        {5.6, 2.7, 4.2, 1.3},
        {5.7, 3.0, 4.2, 1.2},
        {5.7, 2.9, 4.2, 1.3},
        {6.2, 2.9, 4.3, 1.3},
        {5.1, 2.5, 3.0, 1.1},
        {5.7, 2.8, 4.1, 1.3},
        {6.3, 3.3, 6.0, 2.5},
        {5.8, 2.7, 5.1, 1.9},
        {7.1, 3.0, 5.9, 2.1},
        {6.3, 2.9, 5.6, 1.8},
        {6.5, 3.0, 5.8, 2.2},
        {7.6, 3.0, 6.6, 2.1},
        {4.9, 2.5, 4.5, 1.7},
        {7.3, 2.9, 6.3, 1.8},
        {6.7, 2.5, 5.8, 1.8},
        {7.2, 3.6, 6.1, 2.5},
        {6.5, 3.2, 5.1, 2.0},
        {6.4, 2.7, 5.3, 1.9},
        {6.8, 3.0, 5.5, 2.1},
        {5.7, 2.5, 5.0, 2.0},
        {5.8, 2.8, 5.1, 2.4},
        {6.4, 3.2, 5.3, 2.3},
        {6.5, 3.0, 5.5, 1.8},
        {7.7, 3.8, 6.7, 2.2},
        {7.7, 2.6, 6.9, 2.3},
        {6.0, 2.2, 5.0, 1.5},
        {6.9, 3.2, 5.7, 2.3},
        {5.6, 2.8, 4.9, 2.0},
        {7.7, 2.8, 6.7, 2.0},
        {6.3, 2.7, 4.9, 1.8},
        {6.7, 3.3, 5.7, 2.1},
        {7.2, 3.2, 6.0, 1.8},
        {6.2, 2.8, 4.8, 1.8},
        {6.1, 3.0, 4.9, 1.8},
        {6.4, 2.8, 5.6, 2.1},
        {7.2, 3.0, 5.8, 1.6},
        {7.4, 2.8, 6.1, 1.9},
        {7.9, 3.8, 6.4, 2.0},
        {6.4, 2.8, 5.6, 2.2},
        {6.3, 2.8, 5.1, 1.5},
        {6.1, 2.6, 5.6, 1.4},
        {7.7, 3.0, 6.1, 2.3},
        {6.3, 3.4, 5.6, 2.4},
        {6.4, 3.1, 5.5, 1.8},
        {6.0, 3.0, 4.8, 1.8},
        {6.9, 3.1, 5.4, 2.1},
        {6.7, 3.1, 5.6, 2.4},
        {6.9, 3.1, 5.1, 2.3},
        {5.8, 2.7, 5.1, 1.9},
        {6.8, 3.2, 5.9, 2.3},
        {6.7, 3.3, 5.7, 2.5},
        {6.7, 3.0, 5.2, 2.3},
        {6.3, 2.5, 5.0, 1.9},
        {6.5, 3.0, 5.2, 2.0},
        {6.2, 3.4, 5.4, 2.3},
        {5.9, 3.0, 5.1, 1.8}};

    colormap_t colors;
    init_colormap(&colors);
    Display *disp = new Display(colors);
    /*
    disp->title("Fixture 2x12");
    fixt_s<double, 2, 12> fix2x12;
    fix_2x12(&fix2x12);
    pca(fix2x12, disp);
 
    fixt_s<double, 4, 12> fixcsv4x12;
    fix_csv_4x12(FIXT_CSV_FILE_GSAW, &fixcsv4x12);
    disp->title("Fixture csv gsaw 4x12");
    pca(fixcsv4x12, disp);

    fixt_s<double, 6, 23> fixcsvbovin6x23;
    fix_csv_bovin_6x23(FIXT_CSV_FILE_BOVIN, &fixcsvbovin6x23);
    disp->title("Fixture csv bovin 6x23");
    pca(fixcsvbovin6x23, disp);*/

    
    fixt_s<double, 4, 150> fixcsvspecies4x150;
    fix_csv_species_4x150(FIXT_CSV_FILE_SPECIES, &fixcsvspecies4x150);
    disp->title("Fixture csv species 4x150");
    pca(fixcsvspecies4x150, disp, true);
    train(datas, 150, "pca.log", disp);
    delete (disp);
    return 0;
}
