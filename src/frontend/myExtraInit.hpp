#ifndef GTKMM_EXAMPLE_MYEXTRAINIT_HPP
#define GTKMM_EXAMPLE_MYEXTRAINIT_HPP

#include <glibmm/extraclassinit.h>
#include <glibmm/ustring.h>

// Calls gtk_widget_class_set_css_name() in the class init function.
class MyExtraInit : public Glib::ExtraClassInit
{
public:
  MyExtraInit(const Glib::ustring& css_name);

private:
  Glib::ustring m_css_name;
};

#endif //GTKMM_EXAMPLE_MYEXTRAINIT_HPP