#include <gtkmm.h>
#include <windows.h>
#include <iostream>

class MyWindow : public Gtk::Window
{
public:
    MyWindow();

protected:
    void on_button_clicked();
    void on_action_file_new();
    bool update_labels();

private:
    Gtk::Box vbox;
    Gtk::Button button;
    Gtk::Label labelAC, labelBtPercent, labelBtTime, labelBtStatus, labelErr;

    sigc::connection m_timeout_connection;

    Glib::RefPtr<Gtk::CssProvider> m_refCssProvider;
    SYSTEM_POWER_STATUS sps;
};