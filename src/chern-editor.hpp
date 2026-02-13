#pragma once

#include <peel/GObject/GObject.h>
#include <peel/Granite/Granite.h>
#include <peel/Gtk/Gtk.h>
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

  peel::GtkSource::FileLoader *loader;
  peel::GtkSource::FileSaver *saver;

  peel::String title;

  template <typename F>
  static void
  define_properties (F &f)
  {
    f.prop (prop_title (), nullptr)
      .get (&Editor::get_title)
      .set (&Editor::set_title);
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

public:
  const char *
  get_title ()
  {
    return title.c_str ();
  }

  PEEL_PROPERTY (const char *, title, "title");

  void
  open_file (peel::Gio::File *file);

  void
  save_file ();

  static peel::FloatPtr<Editor>
  create ()
  {
    return Object::create<Editor> ();
  }
};
} // namespace Chern
