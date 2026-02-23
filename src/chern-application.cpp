#include "chern-application.hpp"

#include "chern-application-window.hpp"

#include <clocale>
#include <libintl.h>

using namespace peel;

namespace Chern
{
PEEL_CLASS_IMPL (Application, "ChernApplication", Gtk::Application)

inline void
Application::Class::init ()
{
  Granite::init ();
  override_vfunc_activate<Application> ();
}

inline void
Application::vfunc_activate ()
{
  parent_vfunc_activate<Application> ();
  auto window = get_active_window ();
  if (window == NULL)
  {
    window = ApplicationWindow::create (this);
  }
  window->present ();
}
} // namespace Chern

int
main (int argc, char *argv[])
{
  setlocale(LC_ALL, "");
  bindtextdomain(GETTEXT_PACKAGE, DATADIR "/locale");
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain(GETTEXT_PACKAGE);
  RefPtr<Chern::Application> app = Chern::Application::create ();
  return app->run (argc, argv);
}
