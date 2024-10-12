#include <gtkmm.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class MyWindow : public Gtk::Window
{
public:
    MyWindow();

protected:
    void on_button_clicked();
    bool update_labels();
    std::string getBatteryData();
    void create_battery_tab();
    void create_device_table_tab();
    void load_device_data();

    struct DeviceData {
      int bus;
      int device;
      int function;
      int vendor_id;
      int device_id;
      std::string vendorName;
      std::string deviceName;
    };

    std::vector<DeviceData> readDataFromFile(const std::string& filename) {
      std::vector<DeviceData> devices;
      std::ifstream file(filename);
      std::string line;

      while (std::getline(file, line)) {
        std::stringstream ss(line);
        DeviceData device;
        ss >> device.bus >> device.device >> device.function
            >> device.vendor_id >> device.device_id;

        std::string description;
        std::getline(ss, description);

        size_t pos = description.find('$');
        if (pos != std::string::npos) {
          device.vendorName = description.substr(0, pos);
          device.deviceName = description.substr(pos + 1);
        } else {
          device.vendorName = description;
          device.deviceName = "";
        }

        devices.push_back(device);
      }

      return devices;
    }

private:
    Gtk::Box vbox;
    Gtk::Button button;
    Gtk::Label label;
    Gtk::Label h1;
    Glib::RefPtr<Gtk::CssProvider> css_provider;

    Gtk::Notebook* notebook;
    Gtk::TreeView* device_view;
    Glib::RefPtr<Gtk::ListStore> list_store;
    Glib::RefPtr<Gtk::TreeModelSort> sort_model;

    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
      ModelColumns() {
        add(col_bus);
        add(col_device);
        add(col_function);
        add(col_vendor_id);
        add(col_device_id);
        add(col_vendorName);
        add(col_deviceName);
      }

      Gtk::TreeModelColumn<int> col_bus;
      Gtk::TreeModelColumn<int> col_device;
      Gtk::TreeModelColumn<int> col_function;
      Gtk::TreeModelColumn<int> col_vendor_id;
      Gtk::TreeModelColumn<int> col_device_id;
      Gtk::TreeModelColumn<std::string> col_vendorName;
      Gtk::TreeModelColumn<std::string> col_deviceName;
    };

    ModelColumns columns;

    sigc::connection m_timeout_connection;
};