
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/object.hpp>
class PianoParasite : public godot::Node3D {
  GDCLASS(PianoParasite, godot::Node3D)
protected:
  static void _bind_methods();

private:
  void testo2();
};
