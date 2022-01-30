
#include <main.h>
#include <data.h>
#include <colors.h>
#include <colormap.h>
#include <display.h>
#include <alglib/statistics.h>
#include <alglib/dataanalysis.h>

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

static bool traindebug(mat_dble inputData, int pDim, std::string outputFile, Display *disp)
{
    if (inputData.size() < 1 || inputData[0].size() < 1)
        return false;
    int numSamples = (int)inputData.size();
    int oDim = (int)inputData[0].size();
    double *dataMat = new double[numSamples * oDim];
    int i, j;
    for (i = 0; i < numSamples; i++)
        for (j = 0; j < oDim; j++)
            dataMat[i * oDim + j] = inputData[i][j];
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
    fout.close();

    return true;
}

static void testprojection(Display *disp)
{
    alglib::real_2d_array a = "[[1,2,3],[4,5,6]]";
    alglib::real_2d_array b = "[[7,8,9,10],[11,12,13,14],[15,16,17,18]]";
    alglib::real_2d_array result = "[[0,0,0,0],[0,0,0,0]]";
    // result => {{74, 80, 86, 92}, {173, 188, 203, 218}}
    alglib::ae_int_t m = a.rows();
    alglib::ae_int_t n = b.cols();
    alglib::ae_int_t k = a.cols();
    // std::cout << "n: " << n << "| k: " << k << "| m: " << m << std::endl;
    double alpha = 1.0;
    alglib::ae_int_t ia = 0;
    alglib::ae_int_t ja = 0;
    alglib::ae_int_t optypea = 0;
    alglib::ae_int_t ib = 0;
    alglib::ae_int_t jb = 0;
    alglib::ae_int_t optypeb = 0;
    double beta = 0.0;
    alglib::ae_int_t ic = 0;
    alglib::ae_int_t jc = 0;
    alglib::rmatrixgemm(
        m,
        n,
        k,
        alpha,
        a,
        ia,
        ja,
        optypea,
        b,
        ib,
        jb,
        optypeb,
        beta,
        result,
        ic,
        jc);
    disp->mat("Test projection matrix",result, m, n);
}

template <typename T, ui_t NC, ui_t NR>
static void pcadetail(fixt_s<T, NC, NR> fix, Display *disp)
{
    try
    {
        const ui_t r = fix.nbrow;
        const ui_t c = fix.nbcol;
        const ui_t maxrow = 5;
        ui_t i, j;
        alglib::real_2d_array ptInput, mcov, mcorr, mproj;
        ptInput.setcontent(r, c, (T *)&fix.values);
        // Informative covariance matrix
        disp->mat(FIXTURE_DATA_TITLE, ptInput, r, c, maxrow);
        alglib::covm(ptInput, r, c, mcov);
        // Informative correlation matrix
        disp->mat(COV_MAT_TITLE, mcov, c, c);
        alglib::pearsoncorrm(ptInput, r, c, mcorr);
        disp->mat(COR_MAT_TITLE, mcorr, c, c);
        // Pca => alglib::pcabuildbasis operates its own reduction from dataset
        // nor cov neither cor required
        alglib::ae_int_t info;
        alglib::real_1d_array eigValues;
        alglib::real_2d_array eigVectors;
        alglib::pcabuildbasis(ptInput, r, c, info, eigValues, eigVectors);
        disp->mat(PCA_EIGEN_VECTORS_TITLE, eigVectors, c, c);
        disp->vec(PCA_EIGEN_VALUES_TITLE, eigValues, c);
        // Explained variance
        T eigvaSum = 0;
        for (i = 0; i < c; i++)
            eigvaSum += eigValues[i];
        disp->subtitle(PCA_EXPLAINED_VARIANCE_TITLE);
        for (i = 0; i < c; i++)
            std::cout << TAB TAB << "C" << i << SPACE
                      << (eigValues[i] / eigvaSum)
                      << std::endl;
        // Calculate projection
        testprojection(disp);
        /*
        T *projMat = new T[c * r];
        mproj.setcontent(r, c, (T *)&projMat);
        alglib::ae_int_t m = r * c;
        alglib::ae_int_t n = c;
        alglib::ae_int_t k = c;
        double alpha = 1.0;
        alglib::ae_int_t ia = 0;
        alglib::ae_int_t ja = 0;
        alglib::ae_int_t optypea = 0;
        alglib::ae_int_t ib = 0;
        alglib::ae_int_t jb = 0;
        alglib::ae_int_t optypeb = 0;
        double beta = 0.0;
        alglib::ae_int_t ic = 0;
        alglib::ae_int_t jc = 0;
        rmatrixgemm(
            m,
            n,
            k,
            alpha,
            eigVectors,
            ia,
            ja,
            optypea,
            ptInput,
            ib,
            jb,
            optypeb,
            beta,
            mproj,
            ic,
            jc);

        disp->mat("Projected matrix", mproj, r, c, 10);
        delete projMat;*/

        /*
        alglib::real_1d_array w;
        alglib::fisherlda(ptInput, r, c, c, info, w);
        disp->vec("Lda", w, 2);
        */
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
    pcadetail(fixcsvspecies4x150, disp);
    traindebug(datas, fixcsvspecies4x150.nbrow, "pca.log", disp);
    delete (disp);
    return 0;
}
