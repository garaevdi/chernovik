#pragma once

#include <peel/GLib/GLib.h>
#include <peel/GObject/GObject.h>
#include <peel/Granite/Granite.h>
#include <peel/Gtk/Gtk.h>
#include <peel/Gio/Gio.h>
#include <peel/GtkSource/GtkSource.h>
#include <peel/String.h>
#include <peel/class.h>
#include <peel/widget-template.h>

namespace Chern
{
class Editor final : public peel::Granite::Bin
{
  PEEL_SIMPLE_CLASS (Editor, peel::Granite::Bin);

  peel::GtkSource::View *view;
  peel::GtkSource::Buffer *buffer;
  peel::Gtk::FileDialog *save_dialog;

  peel::RefPtr<peel::GtkSource::File> source_file;

  peel::RefPtr<peel::Gio::File> file;
  peel::String title;

  template <typename F>
  static void
  define_properties (F &f)
  {
    f.prop (prop_title (), nullptr)
      .get (&Editor::get_title)
      .set (&Editor::set_title);
    f.prop (prop_file ())
      .get (&Editor::get_file)
      .set (&Editor::set_file);
  }

  inline void
  init (Class *);

  inline void
  vfunc_dispose ();

  void
  set_title (const char *new_title)
  {
    title = new_title;
    notify (prop_title ());
  }

  void
  set_file (peel::RefPtr<peel::Gio::File> new_file)
  {
    if (file)
      return;
    file = new_file;
    notify (prop_file ());
  }

public:
  const char *
  get_title ()
  {
    return title.c_str ();
  }

  peel::Gio::File *
  get_file ()
  {
    return file;
  }

  PEEL_PROPERTY (const char *, title, "title");
  PEEL_PROPERTY (peel::Gio::File, file, "file");

  void
  save_file (bool save_as);

  void
  load_file (peel::Gio::Cancellable *cancellable);

  static peel::FloatPtr<Editor>
  create ()
  {
    return Object::create<Editor> ();
  }

  static peel::FloatPtr<Editor>
  create_with_file (peel::RefPtr<peel::Gio::File> file)
  {
    return Object::create<Editor> (prop_file (), std::move (file));
  }
};
} // namespace Chern
