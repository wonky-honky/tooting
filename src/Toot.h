#include "furnace/src/engine/engine.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/char_string.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <gsl/pointers>
#include <rustex.h>
#include <utility>
constexpr char const * default_filename =
    "/addons/tooting/testres/MegadriveOverdrive.fur";
constexpr char const OCTAVE = 12;

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
enum class Octave {
  //  DblContra,
  SubContra,
  Contra,
  Great,
  Small,
  OnceStricken, // C4
  TwiceStricken,
  ThriceStricken,
  FourX,
  FiveX
};

struct Tone;
constexpr unsigned char WF_ALIGNMENT = 128;
struct WrappedFurnace {
  WrappedFurnace()                                           = default;
  WrappedFurnace(WrappedFurnace const &)                     = delete;
  WrappedFurnace(WrappedFurnace &&)                          = delete;
  auto operator=(WrappedFurnace const &) -> WrappedFurnace & = delete;
  auto operator=(WrappedFurnace &&) -> WrappedFurnace &      = delete;
  DivEngine e;
  bool was_initialized = false; // there is a private thing in furnace that
                                // flags this but no getter or anything
  gsl::owner<unsigned char *> cur_file{};

public:
  ~WrappedFurnace() { delete cur_file; };
} __attribute__((aligned(WF_ALIGNMENT)));

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
  enum ShittyOctave {
    //    DblContra,
    SubContra,
    Contra,
    Great,
    Small,
    OnceStricken, // C4
    TwiceStricken,
    ThriceStricken,
    FourX,
    FiveX
  };

protected:
  static void _bind_methods();
  void init();
  void deinit();
  //  void tick();
  void toot(godot::String const & path = default_filename);
  void load_song(godot::String const & path = default_filename);
  rustex::mutex<WrappedFurnace> _wf;

private:
  void dedoot(Tone t);
  void doot(Tone t);
  //  ShittyNote parse_note(String s) { ShittyNote out; };

public:
  Toot();
  Toot(Toot const &)                = delete;
  Toot(Toot &&)                     = delete;
  auto operator=(Toot &&) -> Toot & = delete;
  ~Toot() override;
  void play_note_anglo(godot::String const & n);
  void play_note(ShittyNote n, ShittyOctave o);
  void depress_note(ShittyNote n, ShittyOctave o);
};
VARIANT_ENUM_CAST(Toot::ShittyNote);
VARIANT_ENUM_CAST(Toot::ShittyOctave);
constexpr char TONE_ALIGNMENT = 8;
struct Tone {
  Note note;
  Octave octave;
  Tone(Note n, Octave o)
      : note(n)
      , octave(o) {};
  Tone(Toot::ShittyNote n, Toot::ShittyOctave o)
      : note(Note(n))
      , octave(Octave(o)) {

      };
  // I'm 90% sure furnace does this wrong and C4 should be midi 60 but is
  // actually midi 48
  [[nodiscard]] auto toMidi() const -> int {
    return std::to_underlying(note) + std::to_underlying(octave) * OCTAVE;
  };
  explicit Tone(int midi)
      : note(Note(midi % OCTAVE))
      , octave(Octave(midi / OCTAVE)) {}
} __attribute__((aligned(TONE_ALIGNMENT)));
