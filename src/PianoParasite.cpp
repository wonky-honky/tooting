#include "PianoParasite.hpp"
#include <godot_cpp/classes/mesh_instance3d.hpp>

void PianoParasite::testo2() {
  // hmm no maybe it would be better to hook the parent node
  godot::MeshInstance3D * a;
  if ((a = cast_to<godot::MeshInstance3D>(this->get_parent())) == nullptr)
    return;
  // or just change this to a subclass of Node3D!
  auto b = this->get_children(true);
}
