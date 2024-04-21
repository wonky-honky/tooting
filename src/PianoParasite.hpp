
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/variant.hpp>
class PianoParasite : public godot::Node3D {
  GDCLASS(PianoParasite, godot::Node3D)
protected:
  static void _bind_methods();

private:
  void testo2();
  godot::Dictionary key_map;

public:
  void setKeyMap(godot::Dictionary const & new_key_map);
  godot::Dictionary getKeyMap() const;
};
