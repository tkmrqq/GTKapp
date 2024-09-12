#include "mainWindow.h"

MyWindow::MyWindow()
{
    set_title("Hello GTKmm");
    set_default_size(800, 400);

    vbox.set_orientation(Gtk::Orientation::VERTICAL);
    vbox.set_spacing(10);
    set_child(vbox);

    labelAC.set_label("AC Line Status:");
    labelBtPercent.set_label("Battery Life Percent:");
    labelBtTime.set_label("Battery Life Time:");
    labelBtStatus.set_label("Battery Status");
    labelErr.set_label("");

    vbox.append(labelAC);
    vbox.append(labelBtPercent);
    vbox.append(labelBtTime);
    vbox.append(labelBtStatus);
    vbox.append(labelErr);

    button.set_label("Update!");
    button.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_button_clicked));
    button.set_name("my_button");

    vbox.append(button);

    m_timeout_connection = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &MyWindow::update_labels), 1000); // Update every 1000 ms (1 second)

    // CSS
    m_refCssProvider = Gtk::CssProvider::create();
    m_refCssProvider->load_from_path("style.css");
    Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), m_refCssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

bool MyWindow::update_labels()
{
    std::string status;
    std::string btFlag;
    if (GetSystemPowerStatus(&sps))
    {
        sps.ACLineStatus == 0 ? (status = "Offline") : (status = "Online");
        labelAC.set_text("AC Line Status: " + status);

        labelBtPercent.set_text("Battery Life Percent: " + std::to_string((int)sps.BatteryLifePercent) + '%');

        if (sps.BatteryLifeTime != (DWORD)-1)
        {
            int hours = sps.BatteryLifeTime / 3600;
            int minutes = (sps.BatteryLifeTime % 3600) / 60;
            labelBtTime.set_text("Battery Life Time: " + std::to_string(hours) + "h " + std::to_string(minutes) + "m");
        }
        else
        {
            labelBtTime.set_text("Battery Life Time: N/A");
        }
        sps.BatteryFlag & 1 ? btFlag = "High" : (sps.BatteryFlag & 2 ? btFlag = "Low" : btFlag = "Unknown");
        labelBtStatus.set_text("Battery Status: " + btFlag);
    }
    else
    {
        labelErr.set_text("Error: Unable to retrieve power status");
    }
    return true;
}

void MyWindow::on_button_clicked()
{
    update_labels();
}

void MyWindow::on_action_file_new()
{
    std::cout << "W W W W" << std::endl;
}
