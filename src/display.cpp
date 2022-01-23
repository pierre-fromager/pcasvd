
#include <display.h>

using namespace std;

Display::Display(colormap_t colors) : m_colors(colors) {}

Display::~Display() {}

void Display::mat(string title, alglib::real_2d_array v, ui_t nbrow, ui_t nbcol)
{
    ui_t i, j;
    this->subtitle(title);
    cout << m_colors.values;
    for (j = 0; j < nbrow; j++)
    {
        for (i = 0; i < nbcol; i++)
            cout << setw(DISP_PAD_LEFT) << left << TAB << v[j][i];
        cout << endl;
    }
    cout << m_colors.reset;
}

void Display::vec(string title, alglib::real_1d_array v, ui_t nbcol)
{
    ui_t i;
    this->subtitle(title);
    cout << m_colors.values;
    for (i = 0; i < nbcol; i++)
        cout << setw(DISP_PAD_LEFT) << left << TAB << v[i];
    cout << m_colors.reset << endl;
}

void Display::title(std::string title)
{
    cout << m_colors.main_title << title << m_colors.reset << endl;
}

void Display::subtitle(std::string title)
{
    cout << TAB << m_colors.sub_title << title << m_colors.reset << endl;
}

void Display::error(std::string title)
{
    cout << TAB << m_colors.error << title << m_colors.reset << endl;
}