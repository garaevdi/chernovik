#include "chern-application-window.hpp"

#include "config.h"

namespace Chern
{
PEEL_CLASS_IMPL (
  ApplicationWindow, "ChernApplicationWindow", Gtk::ApplicationWindow)

inline void
ApplicationWindow::Class::init ()
{
  override_vfunc_dispose<ApplicationWindow> ();
  set_template_from_resource (APP_PATH "/ui/chern-application-window.ui");
}

inline void
ApplicationWindow::init (Class *)
{
  init_template ();
}

inline void
ApplicationWindow::vfunc_dispose ()
{
  dispose_template (Type::of<ApplicationWindow> ());
  parent_vfunc_dispose<ApplicationWindow> ();
}
} // namespace Chern
