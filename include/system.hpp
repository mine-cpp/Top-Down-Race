#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <functional>
#include <bitset>
#include <map>
#include <memory>
#include <typeinfo>

#include <cstddef>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <picture.hpp>



class render_system {
private:
  render_system() { }
  ~render_system() { }

  static SDL_Window* window__;
  static SDL_Renderer* renderer__;
  static int r__, g__, b__, a__;

  static std::queue<picture> render_queue__;

  static void clear();
  static void update() { SDL_RenderPresent(renderer__); }

public:
  static void add_to_queue(const picture pic) { render_queue__.push(pic); }

  static SDL_Window* window() { return window__; }
  static SDL_Renderer* renderer() { return renderer__; }

  static void render(const picture& pic);
  static void quit();

  static void init(
		   const char title[],
		   const int x, const int y,
		   const int w, const int h,
		   const unsigned window_flags,

		   const int index,
		   const unsigned renderer_flags,

		   const int r, const int g, const int b, const int a
		   );
  static void work();

};

class texture_system {
private:
  texture_system() { }
  ~texture_system() { }

  static std::string path__;
  static SDL_Texture* main_textures__;

  static std::stack<picture> render_textures__;

public:
  static SDL_Texture* main_textures() { return main_textures__; }

  static void add_texture(const picture texture) { render_textures__.push(texture); }
  static void quit() { SDL_DestroyTexture(main_textures__); }

  static void init();
  static void work();

};

class time_system {
private:
  time_system() { }
  ~time_system() { }

  static float current_time__,
    next_time__,
    delta_time__;

public:
  static float delta_time() { return delta_time__; }
  static float time() { return SDL_GetTicks() / 1000.0f; }

  static void init();
  static void work();

};

class ECS {
public:
  using entity = std::uint8_t;
  static constexpr entity max_entities = __UINT8_MAX__;

  using component_id = std::uint8_t;
  static constexpr component_id max_components = __UINT8_MAX__;
  using component_bits = std::bitset<max_components>;

private:
  ECS() { }
  ~ECS() { }

  class entity_manager {
  public:
    using function = std::function<void(const entity&, const component_bits&)>;

  private:
    entity_manager() { }
    ~entity_manager() { }

    static unsigned index__;
    static std::map<entity, component_bits> entities__;

    static std::vector<function> observers__;

    static void call_observers(const entity ent);

  public:
    static void add_observer(function& observer) { observers__.push_back(observer); }

    static entity add_entity(const component_bits ent_bits = 0);
    static void destroy_entity(const entity ent);

    static void bits(const entity ent, const component_bits ent_bits) { entities__[ent] = ent_bits; call_observers(ent); }
    static component_bits bits(const entity ent) { return entities__[ent]; }

  };



  class base_component_container {
  public:
    virtual ~base_component_container() { }

    virtual void remove_component(const entity ent) = 0;

  };

  template<class T>
  class component_container : public base_component_container {
  private:
    std::map<entity, T> components__;

    bool max_size() const { return (this->components__.size() >= max_entities); }
    bool found(const entity ent) const { return (this->components__.find(ent) != this->components__.cend()); }

  public:
    void component(const entity ent, const T data);
    T* component(const entity ent);
    void remove_component(const entity ent) override;

  };

  class component_manager {
  private:
    component_manager() { }
    ~component_manager() { }

    static component_id current_id__;
    static std::map<const char*, component_id> ids__;
    static std::map<component_id, std::shared_ptr<base_component_container>> containers__;

    static bool max_size() { return (containers__.size() >= max_components); }
    static bool found(const char* name) { return (ids__.find(name) != ids__.cend()); }

  public:
    template<class T>
    static void register_component();

    template<class T>
    static component_id id();

    template<class T>
    static void component(const entity ent, const T data);

    template<class T>
    static T& component(const entity ent);

  };

};
