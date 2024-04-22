#include "PianoParasite.hpp"
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <memory>

void PianoParasite::testo2() {}
std::shared_ptr<godot::Dictionary> reverse_dict(godot::Dictionary & in) {
  auto keys     = in.keys();
  auto reversed = std::make_shared<godot::Dictionary>();
  for (int i = 0; i < keys.size(); i++) {
    (*reversed)[in[keys[i]]] = keys[i];
  }
  return reversed;
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
      "setKeyMap",
      "getKeyMap"
  );
}
