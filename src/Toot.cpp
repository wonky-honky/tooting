#include "Toot.h"
#include "furnace/src/engine/engine.h"
#include "furnace/src/fileutils.h"
#include "furnace/src/ta-log.h"
#include "rustex.h"
#include <cstddef>
#include <cstdlib>
#include <fmt/printf.h>
#include <godot_cpp/classes/class_db_singleton.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/worker_thread_pool.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/char_string.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <utility>

Toot::Toot() { init(); }
Toot::~Toot() {
  godot::UtilityFunctions::print("Destroying furnace wrapper");
  deinit();
}
void Toot::init() {
  auto wf = _wf.lock_mut();
  if (wf->was_initialized)
    return;
  auto & e = wf->e;
  e.preInit(true);
  if (!e.init()) {
    logE("Couldn't init furnace engine");
  }
  wf->was_initialized = true;
}

void Toot::deinit() {
  auto wf = _wf.lock_mut();
  if (wf->was_initialized) {
    auto & e = wf->e;
    if (e.isPlaying())
      e.stop();
    e.quit();
    wf->was_initialized = false;
  }
}

void Toot::load_song(godot::String path) {
  auto wf         = _wf.lock_mut();
  auto & e        = wf->e;
  auto & cur_file = wf->cur_file;

  auto fff =
      godot::FileAccess::open("res://project.godot", godot::FileAccess::READ);
  auto fileName = (fff->get_path_absolute().get_base_dir() + path).utf8();
  logI(fmt::sprintf("trying to open %s", fileName.get_data()).c_str());

  FILE * f = ps_fopen(fileName, "rb");
  if (fseek(f, 0, SEEK_END) < 0) {
    logE(fmt::sprintf(
             "couldn't open file! (couldn't get file size: %s)",
             strerror(errno)
    )
             .c_str());
    fclose(f);
  }
  ssize_t len = ftell(f);

  cur_file    = new unsigned char[len];
  if (fseek(f, 0, SEEK_SET) < 0) {
    logE(fmt::sprintf("couldn't open file! (size error: %s)", strerror(errno))
             .c_str());
  }
  if (fread(cur_file, 1, (size_t)len, f) != (size_t)len) {
    logE(fmt::sprintf("couldn't open file! (read error: %s)", strerror(errno))
             .c_str());
  }
  fclose(f);
  if (!e.load(cur_file, (size_t)len, fileName)) {
    logE(fmt::sprintf("could not open file! (%s)", e.getLastError()).c_str());
  }

  e.dumpSongInfo();
}

void Toot::toot(godot::String path) {
  load_song(path);
  auto wf  = _wf.lock_mut();
  auto & e = wf->e;
  logI("why the fuck did I even enter this");

  logI("playing...");

  e.play();
  e.setLoops(1);
}
void Toot::play_note(ShittyNote n) {
  auto note = Note(n);
  doot(note);
}
// TODO: check out engine.nextTick
void Toot::doot(Note note) {
  auto wf  = _wf.lock_mut();
  auto & e = wf->e;
  //  e.setAutoNotePoly(true);
  //  e.autoNoteOffAll();
  e.synchronized([&] {
    e.autoNoteOff(-1, std::to_underlying(note));
    logI(fmt::sprintf("%d", e.autoNoteOn(-1, 2, std::to_underlying(note)))
             .c_str());
  });
}

void Toot::_bind_methods() {
  godot::ClassDB::bind_method(godot::D_METHOD("init"), &Toot::init);
  godot::ClassDB::bind_method(
      godot::D_METHOD("play_note", "n"),
      &Toot::play_note
  );
  godot::ClassDB::bind_method(
      godot::D_METHOD("toot", "path"),
      &Toot::toot,
      DEFVAL(default_filename)
  );
  godot::ClassDB::bind_method(
      godot::D_METHOD("load_song", "path"),
      &Toot::load_song,
      DEFVAL(default_filename)
  );
  godot::ClassDB::bind_method(godot::D_METHOD("deinit"), &Toot::deinit);
  BIND_ENUM_CONSTANT(C);
  BIND_ENUM_CONSTANT(Cis);
  BIND_ENUM_CONSTANT(D);
  BIND_ENUM_CONSTANT(Es);
  BIND_ENUM_CONSTANT(E);
  BIND_ENUM_CONSTANT(F);
  BIND_ENUM_CONSTANT(Fis);
  BIND_ENUM_CONSTANT(G);
  BIND_ENUM_CONSTANT(As);
  BIND_ENUM_CONSTANT(A);
  BIND_ENUM_CONSTANT(B);
  BIND_ENUM_CONSTANT(H);
}
