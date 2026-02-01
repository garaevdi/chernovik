#pragma once

#include "config.h"

#include <peel/Adw/Adw.h>
#include <peel/Gio/Gio.h>
#include <peel/Gtk/Gtk.h>
#include <peel/Granite/Granite.h>
#include <peel/class.h>

using namespace peel;

namespace Chern
{
class Application final : public Gtk::Application
{
  PEEL_SIMPLE_CLASS (Application, Gtk::Application);
  friend class Gio::Application;

  inline void
  vfunc_activate ();

public:
  static RefPtr<Application>
  create ()
  {
    return Object::create<Application> (prop_application_id (), APP_ID,
      prop_flags (), Gio::Application::Flags::DEFAULT_FLAGS);
  }
};
} // namespace Chern
