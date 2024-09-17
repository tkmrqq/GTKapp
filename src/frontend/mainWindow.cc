#include "mainWindow.h"

MyWindow::MyWindow()
{
    set_title("Hello GTKmm");
    set_default_size(800, 400);

    vbox.set_orientation(Gtk::Orientation::VERTICAL);
    vbox.set_spacing(10);
    set_child(vbox);

    label.set_label("");
    vbox.append(label);

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
    std::string btData = getBatteryData();
    label.set_text(btData);

    return true;
}

void MyWindow::on_button_clicked()
{
    update_labels();
}

std::string MyWindow::getBatteryData() {
    std::string filename = LOG_DIR "/battery_log.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::string line;
    std::string batteryData;
    
    while (std::getline(file, line)) {
        batteryData += line + "\n"; // добавляем каждую строку в строку с результатом
    }

    file.close();
    return batteryData;
}
