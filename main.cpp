#include <iostream>
#include "mainWindow.cc"

int main(int argc, char *argv[])
{

    auto app = Gtk::Application::create("org.gtkmm.example");

    MyWindow window;

    auto actionGroup = Gio::SimpleActionGroup::create();

    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_path("style.css");

    Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Запуск приложения с использованием указателя на окно
    return app->make_window_and_run<MyWindow>(argc, argv);
}
