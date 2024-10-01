#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>

namespace automata {

namespace music {

class GameMusic {
 public:
  void Play();
  void Stop();
  void Next();

 private:
  sf::Music music_;
};

}  // namespace music

}  // namespace automata
