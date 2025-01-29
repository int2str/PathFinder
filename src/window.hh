#ifndef PATH_FINDER_WINDOW_HH
#define PATH_FINDER_WINDOW_HH

#include <SFML/Graphics.hpp>
#include <optional>

#include "src/tilemap.hh"
#include "utils/coordinate.hh"

namespace path_finder {

//
// Event defines application events triggered by window events, such as key
// presses etc.
//
enum class Event : uint8_t { Reset, PauseResume };

//
// Window defines a graphical window that renders a given tile map to the
// screen.
//
class Window {
  sf::Vector2f tile_;
  sf::RenderWindow window_;

  sf::Texture texture_{};

  //
  // Maps a tile coordinate to a specific coordinate in the tilemap texture
  //
  [[nodiscard]] auto tileAt(Utils::Coordinate coordinate) const -> sf::IntRect;

 public:
  explicit Window(const tilemap::Info& map_info);

  auto isOpen() const -> bool;

  auto handleEvents() -> std::optional<Event>;
  void draw(const tilemap::Grid& map);
};

}  // namespace path_finder

#endif  // PATH_FINDER_WINDOW_HH
