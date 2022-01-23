
#include <display.h>

Display::Display(colormap_t colors) : m_colors(colors) {}

Display::~Display() {}

void Display::mat(string title, alglib::real_2d_array v, ui_t nbrow, ui_t nbcol, ui_t maxrow)
{
    ui_t i, j;
    this->subtitle(title);
    this->print(m_colors.values, SVOID, false);
    const ui_t nbrows = (maxrow != 0) ? maxrow : nbrow;
    for (j = 0; j < nbrows; j++)
    {
        for (i = 0; i < nbcol; i++)
            this->printvalue(v[j][i]);
        this->print(SVOID, SVOID, false, true);
    }
    if (maxrow != 0)
    {
        cout << setw(DISP_PAD_LEFT) << left << TAB;
        this->print(SVOID, TFOLLOW, false, false);
    }
    this->print(SVOID, SVOID, true, true);
}

void Display::vec(string title, alglib::real_1d_array v, ui_t nbcol)
{
    ui_t i;
    this->subtitle(title);
    cout << m_colors.values;
    for (i = 0; i < nbcol; i++)
        this->printvalue(v[i]);
    this->print(SVOID, SVOID, true, true);
}

void Display::title(std::string title)
{
    this->print(m_colors.main_title, title);
}

void Display::subtitle(std::string title)
{
    this->print(m_colors.sub_title, TAB + title);
}

void Display::error(std::string title)
{
    this->print(m_colors.error, TAB + title);
}

void Display::print(std::string color, string line, bool rset, bool end)
{
    cout << color << line;
    if (rset)
        cout << m_colors.reset;
    if (end)
        cout << endl;
}

void Display::printvalue(double v)
{
    cout << setw(DISP_PAD_LEFT) << left << TAB << v;
}