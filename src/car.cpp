#include <car.hpp>



void car::update(const Uint8* key, const float delta_time) {
  float booster = 1.0f;
  const float friction = 1.05f,
    acceleration = 250.0f * delta_time,
    deceleration = acceleration * 2.0f;
  const double angle_modifier = 180.0f * delta_time;

  const vec2f origin = { 0.0f, 0.0f };

  this->update_data(key, booster, angle_modifier, origin);
  this->update_physics(acceleration, deceleration, friction, booster, delta_time);
  this->update_sprite();

}

void car::render(SDL_Renderer* renderer) const {
  SDL_RenderCopyEx(renderer, this->sprite.tex, &(this->sprite.src), &(this->sprite.des), this->angle, nullptr, this->flip);
}

void car::update_data(
		      const Uint8* key,
		      float& booster, const double angle_modifier,
		      const vec2f origin
		      ) {
  const bool up = key[SDL_SCANCODE_UP],
    down = key[SDL_SCANCODE_DOWN],
    left = key[SDL_SCANCODE_LEFT],
    right = key[SDL_SCANCODE_RIGHT],

    n = key[SDL_SCANCODE_N],
    z = key[SDL_SCANCODE_Z],

    up_xor_down = ((up || down) && !(up && down));



  if (n)
    booster = 1.75f;

  if (up_xor_down) {
    if (up)
      this->goal_speed = this->max_speed * booster;

    if (down)
      this->goal_speed = this->max_speed / -4.0f;

  } else
    this->goal_speed = 0.0f;

  if (left)
    this->angle -= angle_modifier;

  if (right)
    this->angle += angle_modifier;

  if (z)
    this->pos = origin;
}

void car::update_physics(
			 const float acceleration, const float deceleration,
			 const float friction, const float booster,
			 const float delta_time
			 ) {  
  auto to_radians = [] (const double angle) {
    const double pi = 3.141592654;
    return angle * pi / 180;
  };

  if (this->speed < this->goal_speed)
      this->speed += acceleration * friction * booster;

  if (this->speed > this->goal_speed)
      this->speed -= deceleration * friction * booster;

  this->pos.x += this->speed * delta_time * std::sin(to_radians(this->angle));
  this->pos.y -= this->speed * delta_time * std::cos(to_radians(this->angle));

  this->cam.update(this->pos);

}

void car::update_sprite() {
  this->sprite.des.x = this->pos.x - this->cam.get_camera_pos().x;
  this->sprite.des.y = this->pos.y - this->cam.get_camera_pos().y;
}
