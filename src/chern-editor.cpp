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
}

inline void
Editor::init (Class *)
{
  init_template ();
  set_title ("New file");
}

inline void
Editor::vfunc_dispose ()
{
  dispose_template (Type::of<Editor> ());
  parent_vfunc_dispose<Editor> ();
}

void
Editor::open_file (Gio::File *file)
{
}

void
Editor::save_file ()
{
}
} // namespace Chern
