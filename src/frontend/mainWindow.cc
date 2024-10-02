#include "mainWindow.h"

MyWindow::MyWindow() {
  set_title("Hello GTKmm");
  set_default_size(640, 800);

  // pages
  notebook = Gtk::make_managed<Gtk::Notebook>();

  vbox.set_orientation(Gtk::Orientation::VERTICAL);
  vbox.set_spacing(10);
  vbox.set_name("body");
  set_child(vbox);
  vbox.append(*notebook);

  create_battery_tab();
  create_device_table_tab();

  m_timeout_connection = Glib::signal_timeout().connect(
      sigc::mem_fun(*this, &MyWindow::update_labels),
      1000); // Update every 1000 ms (1 second)

  css_provider = Gtk::CssProvider::create();
  try {
    css_provider->load_from_path("C:/GitHub/GTKapp/src/frontend/style.css");
    std::cout << "CSS load successfully" << std::endl;
  } catch (const Glib::Error &ex) {
    std::cerr << "Failed to load CSS: " << ex.what() << std::endl;
  }

  Gtk::StyleContext::add_provider_for_display(
      Gdk::Display::get_default(), css_provider,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void MyWindow::create_battery_tab() {
  auto battery_box =
      Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 10);

  h1.set_label("Battery Information:");
  h1.set_name("h1");
  battery_box->append(h1);

  label.set_label("");
  label.set_name("my_label");
  battery_box->append(label);

  button.set_label("Update!");
  button.signal_clicked().connect(
      sigc::mem_fun(*this, &MyWindow::on_button_clicked));
  button.set_name("my_button");

  battery_box->append(button);

  notebook->append_page(*battery_box, "Battery Info");
}

void MyWindow::create_device_table_tab() {
  auto device_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 10);

  device_view = Gtk::make_managed<Gtk::TreeView>();

  list_store = Gtk::ListStore::create(columns);
  device_view->set_model(list_store);

  device_view->append_column("Bus", columns.col_bus);
  device_view->append_column("Device", columns.col_device);
  device_view->append_column("Function", columns.col_function);
  device_view->append_column("Vendor ID", columns.col_vendor_id);
  device_view->append_column("Device ID", columns.col_device_id);
  device_view->append_column("Vendor Name/Device Name", columns.col_description);

  auto scrolled_window = Gtk::make_managed<Gtk::ScrolledWindow>();
  scrolled_window->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC); // Прокрутка по обеим осям
  scrolled_window->set_expand(true);

  scrolled_window->set_child(*device_view);

  load_device_data();

  device_box->append(*scrolled_window);

  notebook->append_page(*device_box, "Device Info");
}

void MyWindow::load_device_data()
{
  std::vector<DeviceData> devices = readDataFromFile(LOG_DIR "/pci_log.txt");

  for (const auto& device : devices) {
    Gtk::TreeModel::Row row = *(list_store->append());
    row[columns.col_bus] = device.bus;
    row[columns.col_device] = device.device;
    row[columns.col_function] = device.function;
    row[columns.col_vendor_id] = device.vendor_id;
    row[columns.col_device_id] = device.device_id;
    row[columns.col_description] = device.description;
  }
}

bool MyWindow::update_labels() {
  std::string btData = getBatteryData();
  label.set_text(btData);

  return true;
}

void MyWindow::on_button_clicked() { update_labels(); }

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
    batteryData +=
        line + "\n"; // добавляем каждую строку в строку с результатом
  }

  file.close();
  return batteryData;
}
