#pragma once

#include <glib/gi18n.h>
#include <peel/Adw/Adw.h>
#include <peel/GLib/functions.h>
#include <peel/Gio/Gio.h>
#include <peel/Granite/Granite.h>
#include <peel/Gtk/Gtk.h>
#include <peel/GtkSource/GtkSource.h>
#include <peel/class.h>
#include <peel/widget-template.h>

namespace Chern
{
class ApplicationWindow final : public peel::Adw::ApplicationWindow
{
  PEEL_SIMPLE_CLASS (ApplicationWindow, peel::Adw::ApplicationWindow);

  peel::Adw::TabView *tabview;
  peel::Gtk::Stack *stack;
  peel::Granite::Placeholder *placeholder;

  inline void
  init (Class *);

  void
  create_new_buffer ();

  inline void
  vfunc_dispose ();

public:
  static ApplicationWindow *
  create (peel::Gtk::Application *app)
  {
    return Object::create<ApplicationWindow> (prop_application (), app);
  }
};
} // namespace Chern
