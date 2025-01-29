#include "src/window.hh"

#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "src/tilemap.hh"
#include "src/tilemap_woodland.hh"
#include "utils/coordinate.hh"

namespace {

constexpr auto WINDOW_TITLE = "Path Finder";

}  // namespace

namespace path_finder {

Window::Window(const tilemap::Info& map_info)
    : tile_{sf::Vector2f{static_cast<float>(map_info.tile_size.x),
                         static_cast<float>(map_info.tile_size.y)}},
      window_{sf::VideoMode{static_cast<uint32_t>(map_info.canvas_size.x),
                            static_cast<uint32_t>(map_info.canvas_size.y)},
              WINDOW_TITLE} {
  texture_.loadFromFile(fmt::format("assets/{}", map_info.texture_filename));
  window_.setFramerateLimit(5);
}

auto Window::tileAt(Utils::Coordinate coordinate) const -> sf::IntRect {
  return {coordinate.x * static_cast<int>(tile_.x),
          coordinate.y * static_cast<int>(tile_.y), static_cast<int>(tile_.x),
          static_cast<int>(tile_.y)};
}

auto Window::isOpen() const -> bool { return window_.isOpen(); }

auto Window::handleEvents() -> std::optional<Event> {
  auto event = sf::Event{};
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed) window_.close();

    if (event.type == sf::Event::KeyPressed) {
      switch (event.key.code) {
        case sf::Keyboard::Key::Escape:
          window_.close();
          break;
        case sf::Keyboard::Key::R:
          return Event::Reset;
        case sf::Keyboard::Key::P:
          return Event::PauseResume;
        default:
          break;
      }
    }
  }
  return std::nullopt;
}

void Window::draw(const tilemap::Grid& map) {
  window_.clear(sf::Color::White);

  auto sprite = sf::Sprite(texture_);
  for (const auto& coordinate : map.coordinates()) {
    sprite.setPosition(tile_.x * static_cast<float>(coordinate.x),
                       tile_.y * static_cast<float>(coordinate.y));

    // For aesthetic reasons, draw grass background first :)
    // NOTE(AE) - Background should be a separate map layer
    sprite.setTextureRect(tileAt(tilemap::woodland::GRASS));
    window_.draw(sprite);

    // Now draw the sprite
    sprite.setTextureRect(tileAt(map[coordinate]));
    window_.draw(sprite);
  }

  window_.display();
}

}  // namespace path_finder
