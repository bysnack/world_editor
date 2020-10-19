#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <memory>
#include<list>
#include "TGUI/TGUI.hpp"
#include "systems/systems.h"
#include "entities/entities.h"
#include "./config.h"
#include "./resources/resources.h"
#include "./resources/atlas.h"

namespace engine {

	class core
	{
	public:
		core(config config) :
			_s{ static_cast<float>(sf::Texture::getMaximumSize()) },
			//_camera{ { _s / 2.f, _s /2.f }, { _s,_s } },
			_camera{ { 960.f, 540.f }, { 1920.f, 1080.f } },
            resources{ config },
			_gui{ target },
            systems{
				systems::render{ target }
			}
		{
			target.create({ config.width, config.height }, "Argentum Online C++ Basic Engine");
			target.setView(_camera);
		}

		void run()
		{
			systems.run(entities);
			float elapsed_time = 0.f;

			const auto& atlas = resources.get<resources::atlas>();
			sf::Sprite test{ atlas };
			sf::Sprite test2{ atlas };

			test2.setTextureRect(atlas[100]);

			// start the game loop
			while (target.isOpen()) {
				// listen for events
				sf::Event event;
				while (target.pollEvent(event)) {
					systems.run(entities, event, static_cast<float>(elapsed_time));
					if (event.type == sf::Event::Closed) {
						target.close();
						return;
					}
				}

				target.clear(sf::Color::Black);

				// run on user update systems
				systems.run(entities, static_cast<float>(elapsed_time));

				elapsed_time = _clock.restart().asSeconds();

				_gui.draw();
		
				target.display();
			}
		}
		sf::RenderWindow target;
		float _s;
		sf::View			_camera;
        resources::manager 	resources;
	private:
		tgui::Gui 			_gui;
		sf::Clock 			_clock;
	public:
		systems::manager 	systems;
		entities::manager 	entities;
	};
}
