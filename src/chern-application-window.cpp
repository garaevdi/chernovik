#include "chern-application-window.hpp"

#include "chern-editor.hpp"
#include "config.h"

using namespace peel;

namespace Chern
{
PEEL_CLASS_IMPL (
  ApplicationWindow, "ChernApplicationWindow", Adw::ApplicationWindow)

inline void
ApplicationWindow::Class::init ()
{
  override_vfunc_dispose<ApplicationWindow> ();
  set_template_from_resource (APP_PATH "/ui/chern-application-window.ui");
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (ApplicationWindow, tabview);
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (ApplicationWindow, stack);
  PEEL_WIDGET_TEMPLATE_BIND_CHILD (ApplicationWindow, placeholder);

  install_action ("win.new-tab", nullptr,
    [] (Gtk::Widget *widget, const char *action_name, GLib::Variant *param)
      {
        (void)action_name;
        (void)param;
        widget->cast<ApplicationWindow> ()->create_new_buffer ();
      });
}

inline void
ApplicationWindow::init (Class *)
{
  init_template ();

  auto new_tab_icon = Gio::ThemedIcon::create ("document-new");
  auto new_tab_btn = placeholder->append_button (
    new_tab_icon, _ ("New tab"), _ ("Create new tab with an empty buffer"));
  new_tab_btn->set_action_name ("win.new-tab");

  tabview->connect_notify (
    [this] (peel::GObject::Object *object, peel::GObject::ParamSpec *pspec)
      {
        if (GLib::str_equal (pspec->get_name (), "n-pages"))
          {
            if (this->tabview->get_n_pages () > 0)
              {
                this->stack->set_visible_child_name ("content");
              }
            else
              {
                this->stack->set_visible_child_name ("placeholder");
              }
          }
      });
}

void
ApplicationWindow::create_new_buffer ()
{
  FloatPtr<Editor> editor = Editor::create ();
  FloatPtr<Adw::TabPage> page = tabview->append (editor->cast<Gtk::Widget> ());
  Object::bind_property (
    editor, Editor::prop_title (),
    page, Adw::TabPage::prop_title (),
    peel::GObject::BindingFlags::SYNC_CREATE
  );
}

inline void
ApplicationWindow::vfunc_dispose ()
{
  dispose_template (Type::of<ApplicationWindow> ());
  parent_vfunc_dispose<ApplicationWindow> ();
}
} // namespace Chern
