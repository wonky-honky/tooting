#include "furnace/src/engine/engine.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/char_string.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <rustex.h>
constexpr char const * default_filename =
    "/addons/tooting/testres/MegadriveOverdrive.fur";
struct WrappedFurnace {
  DivEngine e;
  bool was_initialized; // there is a private thing in furnace that flags this
                        // but no getter or anything
  unsigned char * cur_file;
};

class Toot : public godot::Node {
  GDCLASS(Toot, godot::Node)
protected:
  static void _bind_methods();
  void init();
  void deinit();
  void toot(godot::String path = default_filename);
  rustex::mutex<WrappedFurnace> _wf;

private:
public:
  Toot();
  ~Toot();
};
