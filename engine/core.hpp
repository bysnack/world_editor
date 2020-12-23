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
			_gui{ target }
		{
			systems.emplace<systems::render>(target, entities, resources);
			target.create({ config.width, config.height }, "Argentum Online C++ Basic Engine");
			target.setView(_camera);

			auto listBox = tgui::ListBox::create();
			listBox->setSize(120, 480);
			listBox->setItemHeight(14);
			listBox->setPosition(10, 340);
			for (const auto& res : resources.get<resources::atlas>()) {
				listBox->addItem(std::to_string(res.first));
			}

			target.setFramerateLimit(0);
			target.setVerticalSyncEnabled(true);

        	_gui.add(listBox);
		}

		void run()
		{
			systems.run();
			float elapsed_time = 0.f;

			// start the game loop
			while (target.isOpen()) {
				// listen for events
				sf::Event event;
				while (target.pollEvent(event)) {
					//systems.run(entities, event, static_cast<float>(elapsed_time));
					if (event.type == sf::Event::Closed) {
						target.close();
						return;
					}

					_gui.handleEvent(event);
				}

				target.clear(sf::Color::Black);

				// run on user update systems
				systems.run(_clock.restart().asSeconds());

				_gui.draw();
		
				target.display();
			}
		}
		sf::RenderWindow target;
		float _s;
		sf::View			_camera;
        const resources::manager 	resources;
	private:
		tgui::Gui 			_gui;
		sf::Clock 			_clock;
	public:
		entities::manager 	entities;
		systems::manager 	systems;
	};
}
