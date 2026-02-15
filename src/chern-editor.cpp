#include "chern-editor.hpp"

#include "config.h"

using namespace peel;

namespace Chern
{
PEEL_CLASS_IMPL (Editor, "ChernEditor", Granite::Bin)

inline void
Editor::Class::init ()
{
  // override_vfunc_constructed<Editor> ();
  override_vfunc_dispose<Editor> ();
  set_template_from_resource (APP_PATH "/ui/chern-editor.ui");
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (Editor, buffer);
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (Editor, view);
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (Editor, save_dialog);
}

inline void
Editor::init (Class *)
{
  init_template ();
  source_file = GtkSource::File::create ();
  Object::bind_property (
    this, prop_file (), source_file, GtkSource::File::prop_location (), peel::GObject::BindingFlags::SYNC_CREATE);
  connect_notify(
    [this] (peel::GObject::Object *obj, peel::GObject::ParamSpec *pspec)
    {
      if (GLib::str_equal(pspec->get_name(), file))
      {
        if (file) {
          set_title(file->get_basename());
        }
      }
    });
  set_title ("New file");
}

inline void
Editor::vfunc_dispose ()
{
  dispose_template (Type::of<Editor> ());
  parent_vfunc_dispose<Editor> ();
}

void
Editor::save_file (bool save_as)
{
  bool cancel = false;
  if (!file || save_as)
  {
    // TODO: fix saveing as another file
    // RefPtr<Gio::Cancellable> cancellable = Gio::Cancellable::create ();
    // save_dialog->save (this->get_root ()->cast<Gtk::Window> (), cancellable,
    //   [this, cancel] (Object *source, Gio::AsyncResult *res) mutable
    //   {
    //     UniquePtr<GLib::Error> err;
    //     file = source->cast<Gtk::FileDialog> ()->save_finish (res, &err);
    //     if (err)
    //     {
    //       GLib::log (APP_ID, GLib::LogLevelFlags::LEVEL_WARNING, "Couldn't save file: %s", err->message);
    //       cancel = true;
    //       return;
    //     }
    //   });
  }

  if (cancel)
  {
    return;
  }

  RefPtr<GtkSource::FileSaver> saver = GtkSource::FileSaver::create (buffer, source_file);
  RefPtr<Gio::Cancellable> cancellable = Gio::Cancellable::create ();
  GLib::log (APP_ID, GLib::LogLevelFlags::LEVEL_DEBUG, "Trying to save file %s", file->get_path ().c_str ());
  saver->save_async (G_PRIORITY_DEFAULT, cancellable, nullptr,
    [] (Object *source, Gio::AsyncResult *res)
    {
      UniquePtr<GLib::Error> err;
      source->cast<GtkSource::FileSaver> ()->save_finish (res, &err);
      if (err)
      {
        GLib::log (APP_ID, GLib::LogLevelFlags::LEVEL_WARNING, "Couldn't save file: %s", err->message);
      }
    });
}

void
Editor::load_file (Gio::Cancellable *cancellable)
{
  RefPtr<GtkSource::FileLoader> loader = GtkSource::FileLoader::create (buffer, source_file);
  loader->load_async (G_PRIORITY_DEFAULT, cancellable, nullptr,
    [] (Object *source, Gio::AsyncResult *res)
    {
      UniquePtr<GLib::Error> err;
      source->cast<GtkSource::FileLoader> ()->load_finish (res, &err);
      if (err)
      {
        GLib::log (APP_ID, GLib::LogLevelFlags::LEVEL_WARNING, "Couldn't load file: %s", err->message);
      }
    });
  set_title (file->get_basename ());
}
} // namespace Chern
