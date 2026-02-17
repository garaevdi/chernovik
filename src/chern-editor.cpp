#include "chern-editor.hpp"

#include "config.h"

using namespace peel;

namespace Chern
{
PEEL_CLASS_IMPL (Editor, "ChernEditor", Granite::Bin)

inline void
Editor::Class::init ()
{
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
    this, prop_file (), source_file, source_file->prop_location (), peel::GObject::BindingFlags::BIDIRECTIONAL);
  connect_notify (
    [this] (peel::GObject::Object *obj, peel::GObject::ParamSpec *pspec)
    {
      if (strcmp (pspec->get_name (), prop_file ().get_name ()) == 0)
      {
        if (file)
        {
          set_title (file->get_basename ());
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
Editor::write_file (Gio::Cancellable *cancellable)
{
  RefPtr<GtkSource::FileSaver> saver = GtkSource::FileSaver::create (buffer, source_file);
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
Editor::save_file (bool save_as, Gio::Cancellable *cancellable)
{
  // FIXME figure out why it's impossible to change Gio::File
  // i.e. whenever user tries to change save location, it's still saves to the old file
  if (!file || save_as)
  {
    if (file)
    {
      save_dialog->set_initial_file (file);
    }
    save_dialog->save (this->get_root ()->cast<Gtk::Window> (), cancellable,
      [this, cancellable] (Object *source, Gio::AsyncResult *res)
      {
        UniquePtr<GLib::Error> err;
        set_file (source->cast<Gtk::FileDialog> ()->save_finish (res, &err));
        if (err)
        {
          GLib::log (
            APP_ID, GLib::LogLevelFlags::LEVEL_WARNING, "Couldn't select file's save location: %s", err->message);
          return;
        }
        write_file (cancellable);
      });
  }
  else
  {
    write_file (cancellable);
  }
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
}
} // namespace Chern
