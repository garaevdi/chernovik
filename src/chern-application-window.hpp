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

#include "chern-editor.hpp"

namespace Chern
{
class ApplicationWindow final : public peel::Adw::ApplicationWindow
{
  PEEL_SIMPLE_CLASS (ApplicationWindow, peel::Adw::ApplicationWindow);

  peel::Gtk::Revealer *start_revealer;
  peel::Adw::TabView *tabview;
  peel::Gtk::Stack *stack;
  peel::Granite::Placeholder *placeholder;
  peel::Gtk::FileDialog *open_dialog;

  peel::RefPtr<peel::Gio::Cancellable> cancellable;
  peel::RefPtr<peel::Gio::Settings> settings;

  inline void
  init (Class *);

  void
  create_new_tab ();

  void
  close_active_tab ();

  void
  open_file ();

  void
  save_file (bool save_as);

  void
  append_editor (Editor *editor);

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
