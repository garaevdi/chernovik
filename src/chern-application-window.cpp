#include "chern-application-window.hpp"

#include "chern-editor.hpp"
#include "config.h"

using namespace peel;

namespace Chern
{
PEEL_CLASS_IMPL (ApplicationWindow, "ChernApplicationWindow", Adw::ApplicationWindow)

inline void
ApplicationWindow::Class::init ()
{
  override_vfunc_dispose<ApplicationWindow> ();
  set_template_from_resource (APP_PATH "/ui/chern-application-window.ui");
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (ApplicationWindow, start_revealer);
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (ApplicationWindow, tabview);
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (ApplicationWindow, stack);
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (ApplicationWindow, placeholder);
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (ApplicationWindow, open_dialog);

  install_action (
    "win.new-tab", nullptr,
    [] (Gtk::Widget *widget, const char *action_name, GLib::Variant *param)
    {
      (void)action_name;
      (void)param;
      widget->cast<ApplicationWindow> ()->create_new_tab ();
    }
  );
  install_action (
    "win.close-tab", nullptr,
    [] (Gtk::Widget *widget, const char *action_name, GLib::Variant *param)
    {
      (void)action_name;
      (void)param;
      widget->cast<ApplicationWindow> ()->close_active_tab ();
    }
  );
  install_action (
    "win.open-file", nullptr,
    [] (Gtk::Widget *widget, const char *action_name, GLib::Variant *param)
    {
      (void)action_name;
      (void)param;
      widget->cast<ApplicationWindow> ()->open_file ();
    }
  );
  install_action (
    "win.save-file", "bool",
    [] (Gtk::Widget *widget, const char *action_name, GLib::Variant *param)
    {
      (void)action_name;
      (void)param;
      widget->cast<ApplicationWindow> ()->save_file (param->get<bool> ());
    }
  );

  // clang-format off
  add_binding_action (GDK_KEY_T, Gdk::ModifierType::CONTROL_MASK, "win.new-tab", nullptr);
  add_binding_action (GDK_KEY_W, Gdk::ModifierType::CONTROL_MASK, "win.close-tab", nullptr);
  add_binding_action (GDK_KEY_O, Gdk::ModifierType::CONTROL_MASK, "win.open-file", nullptr);
  add_binding_action (GDK_KEY_S, Gdk::ModifierType::CONTROL_MASK, "win.save-file", "b", false);
  add_binding_action (GDK_KEY_S, Gdk::ModifierType::CONTROL_MASK | Gdk::ModifierType::SHIFT_MASK, "win.save-file", "b", true);
  // clang-format on
}

inline void
ApplicationWindow::init (Class *)
{
  init_template ();

  cancellable = Gio::Cancellable::create ();

  action_set_enabled ("win.save-file", false);

  settings = Gio::Settings::create (APP_ID);
  settings->bind ("window-width", this, "default-width", Gio::Settings::BindFlags::DEFAULT);
  settings->bind ("window-height", this, "default-height", Gio::Settings::BindFlags::DEFAULT);

  auto new_icon = Gio::ThemedIcon::create ("document-new");
  auto new_btn = placeholder->append_button (new_icon, _ ("New empty file"), _ ("Create new tab with an empty buffer"));
  new_btn->set_action_name ("win.new-tab");
  auto open_icon = Gio::ThemedIcon::create ("document-open");
  auto open_btn = placeholder->append_button (open_icon, _ ("Open file"), _ ("Open new tab with an existing file"));
  open_btn->set_action_name ("win.open-file");

  tabview->connect_notify (Adw::TabView::prop_n_pages (), this, &ApplicationWindow::n_pages_cb);
  tabview->connect_notify (Adw::TabView::prop_selected_page (), this, &ApplicationWindow::selected_page_cb);
}

void
ApplicationWindow::create_new_tab ()
{
  FloatPtr<Editor> editor = Editor::create ();
  append_editor (editor);
}

void
ApplicationWindow::close_active_tab ()
{
  if (tabview->get_n_pages () != 0)
  {
    tabview->close_page (tabview->get_selected_page ());
  }
}

void
ApplicationWindow::open_file ()
{
  open_dialog->open (
    this, cancellable,
    [this] (Object *source, Gio::AsyncResult *res)
    {
      UniquePtr<GLib::Error> err;
      RefPtr<Gio::File> file = source->cast<Gtk::FileDialog> ()->open_finish (res, &err);
      if (err)
      {
        if (err->code == (int)Gtk::DialogError::FAILED)
        {
          GLib::log (APP_ID, GLib::LogLevelFlags::LEVEL_WARNING, "Couldn't load file: %s", err->message);
        }
        return;
      }

      FloatPtr<Editor> editor = Editor::create_with_file (std::move (file));
      editor->load_file (cancellable);
      append_editor (editor);
    }
  );
}

void
ApplicationWindow::save_file (bool save_as)
{
  if (tabview->get_n_pages () == 0)
  {
    return;
  }
  FloatPtr<Adw::TabPage> page = tabview->get_selected_page ();
  FloatPtr<Editor> editor = page->get_child ()->cast<Editor> ();
  editor->save_file (save_as, cancellable);
}

void
ApplicationWindow::append_editor (Editor *editor)
{
  FloatPtr<Adw::TabPage> page = tabview->append (editor->cast<Gtk::Widget> ());
  tabview->set_selected_page (page);
  Object::bind_property (
    editor, Editor::prop_title (), page, Adw::TabPage::prop_title (), peel::GObject::BindingFlags::SYNC_CREATE
  );
  Object::bind_property (
    editor, Editor::prop_dirty (), page, Adw::TabPage::prop_indicator_icon (), peel::GObject::BindingFlags::DEFAULT,
    [] (bool dirty) -> RefPtr<Gio::Icon>
    {
      if (dirty)
      {
        RefPtr<Gio::Icon> icon = Gio::ThemedIcon::create ("panel-modified-symbolic");
        return icon;
      }
      else
      {
        return nullptr;
      }
    }
  );
}

void
ApplicationWindow::n_pages_cb (peel::GObject::Object *obj, peel::GObject::ParamSpec *pspec)
{
  if (tabview->get_n_pages () > 0)
  {
    stack->set_visible_child_name ("content");
    action_set_enabled ("win.save-file", true);
    start_revealer->set_reveal_child (true);
  }
  else
  {
    stack->set_visible_child_name ("placeholder");
    action_set_enabled ("win.save-file", false);
    start_revealer->set_reveal_child (false);
  }
}

void
ApplicationWindow::selected_page_cb (peel::GObject::Object *obj, peel::GObject::ParamSpec *pspec)
{
  RefPtr<Adw::TabPage> page = tabview->get_selected_page ();
  if (page)
  {
    set_title (page->get_child ()->cast<Editor> ()->get_title ());
  }
  else
  {
    set_title ("");
  }
}

inline void
ApplicationWindow::vfunc_dispose ()
{
  dispose_template (Type::of<ApplicationWindow> ());
  parent_vfunc_dispose<ApplicationWindow> ();
}
} // namespace Chern
