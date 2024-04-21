#include "PianoParasite.hpp"
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/property_info.hpp>

void PianoParasite::testo2() {
  // hmm no maybe it would be better to hook the parent node
  godot::MeshInstance3D * a;
  if ((a = cast_to<godot::MeshInstance3D>(this->get_parent())) == nullptr)
    return;
  // or just change this to a subclass of Node3D!
  auto b = this->get_children(true);
}

void PianoParasite::setKeyMap(godot::Dictionary const & new_key_map) {
  key_map = new_key_map;
}
godot::Dictionary PianoParasite::getKeyMap() const { return key_map; }

void PianoParasite::_bind_methods() {
  godot::ClassDB::bind_method(
      godot::D_METHOD("getKeyMap"),
      &PianoParasite::getKeyMap
  );

  godot::ClassDB::bind_method(
      godot::D_METHOD("setKeyMap", "new_key_map"),
      &PianoParasite::setKeyMap
  );
  ADD_PROPERTY(
      godot::PropertyInfo(godot::Variant::DICTIONARY, "key_map"),
      "getKeyMap",
      "setKeyMap"
  );
}
