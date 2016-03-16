#include <Elementary.hh>

#include "src/version.h"

#include "lib/db.hpp"

EAPI_MAIN int elm_main (int argc, char *argv[]) {
    q4wine::lib::DBEngine* db =
            q4wine::lib::DBEngine::getInstance();
    db->open(":memory:");

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_HIDDEN);

   ::elm::win_standard win;
   win.title_set("Hello, World!");
   win.autohide_set(true);

   ::elm::bg bg(efl::eo::parent = win);
   bg.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   win.resize_object_add(bg);
   bg.visible_set(true);

   ::elm::button btn(efl::eo::parent = win);
   btn.text_set("elm.text","Hello World!");
   btn.size_set(120, 30);
   btn.position_set(60, 15);
   btn.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   btn.size_hint_align_set(EVAS_HINT_FILL, EVAS_HINT_FILL);
   btn.visible_set(true);

   auto on_click = std::bind([] () { elm_exit(); });

   btn.callback_clicked_add( on_click );

   win.size_set(240, 60);
   win.visible_set(true);

   elm_run();
   return 0;
}
ELM_MAIN()
