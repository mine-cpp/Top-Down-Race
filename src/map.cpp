#include <map.hpp>


int tile_map::find_tile(const char id) const {

  for (unsigned i = 0; i < this->tiles.size(); ++i) {

    if (this->tiles[i].id == id)
      return i;

  }

  return -1;

}

vec2f tile_map::find_tile_xy(const char id) const {
  const int i = this->find_tile(id);
  if (i < 0)
    return { -1.0f, -1.0f };

  const int y = i / this->w,
    x = i - y * this->w;
  const float fx = x, fy = y;
  const vec2f tile_xy = { fx, fy };

  return tile_xy;
}

void tile_map::add_to_map(const std::string tiles) {
  this->map += tiles;
  this->h += tiles.size() / this->w;
}

void tile_map::update(const camera cam) const {
  const vec2f camera_pos = cam.get_camera_pos(),
    window_wh = cam.get_window_wh(),
    camera_wh = camera_pos + window_wh;
  SDL_Rect des = { 0, 0, this->tw, this->th };

  const int min_y = camera_pos.y / this->th,
    min_x = camera_pos.x / this->tw;

  int max_y = camera_wh.y / this->th,
    max_x = camera_wh.x / this->tw;



  if (++max_y > this->h)
    max_y = this->h;

  if (++max_x > this->w)
    max_x = this->w;



  for (int y = min_y; y < max_y; ++y) {

    for (int x = min_x; x < max_x; ++x) {

      const char id = this->map[x + y * this->w];
      int i = this->find_tile(id);

      if (i < 0)
	continue;

      const auto& t = this->tiles[i];
      des.x = x * des.w - camera_pos.x;
      des.y = y * des.h - camera_pos.y;
      const picture texture = { t.sprite.src, des };

      picture_system::add_picture(texture);

    }

  }

}
