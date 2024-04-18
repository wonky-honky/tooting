#include "furnace/src/engine/engine.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/char_string.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <rustex.h>

constexpr char const * default_filename =
    "/addons/tooting/testres/MegadriveOverdrive.fur";

enum class Note {
  C,
  Cis,
  D,
  Es,
  E,
  F,
  Fis,
  G,
  As,
  A,
  B,
  H,
};

struct WrappedFurnace {
  DivEngine e;
  bool was_initialized; // there is a private thing in furnace that flags this
                        // but no getter or anything
  unsigned char * cur_file;
};

class Toot : public godot::Node {
  GDCLASS(Toot, godot::Node)
public:
  enum ShittyNote {
    C,
    Cis,
    D,
    Es,
    E,
    F,
    Fis,
    G,
    As,
    A,
    B,
    H,
  };

protected:
  static void _bind_methods();
  void init();
  void deinit();
  void toot(godot::String path = default_filename);
  void load_song(godot::String path = default_filename);
  void play_note(Toot::ShittyNote n);
  void doot(Note note);
  rustex::mutex<WrappedFurnace> _wf;

private:
public:
  Toot();
  ~Toot();
};
VARIANT_ENUM_CAST(Toot::ShittyNote);
