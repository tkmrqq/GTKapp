#include <gtkmm.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class MyWindow : public Gtk::Window
{
public:
    MyWindow();

protected:
    void on_button_clicked();
    bool update_labels();
    std::string getBatteryData();

private:
    Gtk::Box vbox;
    Gtk::Button button;
    Gtk::Label label;

    sigc::connection m_timeout_connection;

    Glib::RefPtr<Gtk::CssProvider> m_refCssProvider;
    SYSTEM_POWER_STATUS sps;
};