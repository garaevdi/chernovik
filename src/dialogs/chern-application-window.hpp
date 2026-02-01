#pragma once

#include <peel/Gtk/Gtk.h>
#include <peel/class.h>

using namespace peel;

namespace Chern
{
class ApplicationWindow final : public Gtk::ApplicationWindow
{
  PEEL_SIMPLE_CLASS (ApplicationWindow, Gtk::ApplicationWindow);
  friend class Gtk::Window;

  inline void
  init (Class *);

  inline void
  vfunc_dispose ();

public:
  static ApplicationWindow *
  create (Gtk::Application *app)
  {
    return Object::create<ApplicationWindow> (prop_application (), app);
  }
};
} // namespace Chern
