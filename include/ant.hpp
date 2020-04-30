#pragma once

#include <list>
#include "marker.hpp"
#include "food.hpp"
#include "utils.hpp"
#include "world.hpp"
#include <iostream>


constexpr float max_reserve = 2000.0f;


const sf::Color ANT_COLOR(255, 73, 68);


struct Ant
{
	Ant() = default;

	Ant(float x, float y, float angle, uint32_t id_)
		: position(x, y)
		, direction(angle)
		, last_direction_update(getRandUnder(direction_update_period))
		, last_marker(0.0f)
		, phase(Marker::Type::ToFood)
		, colony(x, y)
		, target_marker(nullptr)
		, reserve(max_reserve)
		, id(id_)
	{}

	void update(const float dt, World& world)
	{
		updatePosition(dt);

		if (phase == Marker::ToFood) {
			checkFood(world);
		}
		else {
			checkColony();
		}


		last_direction_update += dt;
		if (last_direction_update > direction_update_period) {
			findNewDirection(world);
			float range = PI * 0.15f;
			direction += getRandRange(range);
			last_direction_update = 0.0f;
		}

		last_marker += dt;
		if (last_marker >= marker_period) {
			addMarker(world);
		}
	}

	void updatePosition(const float dt)
	{
		const float speed = 50.0f;
		position += (dt * speed) * sf::Vector2f(cos(direction), sin(direction));

		position.x = position.x < 0.0f ? 1920.0f : position.x;
		position.y = position.y < 0.0f ? 1080.0f : position.y;

		position.x = position.x > 1920.0f ? 0.0f : position.x;
		position.y = position.y > 1080.0f ? 0.0f : position.y;
	}

	void checkFood(World& world)
	{
		const std::list<Food*> food_spots = world.grid_food.getAllAt(position);
		for (Food* fp : food_spots) {
			if (getLength(position - fp->position) < fp->radius) {
				phase = Marker::ToHome;
				direction += PI;
				reserve = max_reserve;
				fp->destroy();
			}
		}
	}

	void checkColony()
	{
		const float colony_size = 20.0f;
		if (getLength(position - colony) < colony_size) {
			phase = Marker::ToFood;
			direction += PI;
			reserve = max_reserve;
		}
	}

	void findNewDirection(World& world)
	{
		findMarker(world);
	}

	void findMarker(World& world)
	{
		const std::list<Marker*> markers = world.grid_markers.getAllAt(position);
		const float max_dist = 40.0f;

		float total_intensity = 0.0f;
		sf::Vector2f point(0.0f, 0.0f);

		for (Marker* mp : markers) {
			Marker& m = *mp;
			const sf::Vector2f to_marker = m.position - position;
			const float length = getLength(to_marker);

			if (length < max_dist) {
				if (m.type == phase) {
					if (dot(to_marker, sf::Vector2f(cos(direction), sin(direction))) > 0.0f) {
						total_intensity += m.intensity;
						point += m.intensity * m.position;
					}
				}
			}
		}

		if (total_intensity) {
			direction = getAngle(point / total_intensity - position);
		}
	}

	void addMarker(World& world)
	{
		world.grid_markers.add(Marker(position, phase == Marker::ToFood ? Marker::ToHome : Marker::ToFood, reserve * 0.02f));
		reserve *= 0.98f;

		last_marker = 0.0f;
	}

	void render(sf::RenderTarget& target) const
	{
		if (target_marker) {
			sf::VertexArray to_target_va(sf::Lines, 2);
			to_target_va[0].position = position;
			to_target_va[0].color = sf::Color::Green;
			to_target_va[1].position = target_marker->position;
			to_target_va[1].color = sf::Color::Green;
			target.draw(to_target_va);
		}

		const float width = 2.0f;
		const float length = 7.0f;
		sf::RectangleShape body(sf::Vector2f(width, length));
		body.setOrigin(width * 0.5f, length * 0.5f);
		body.setPosition(position);
		body.setRotation(direction * 57.2958f + 90.0f);
		body.setFillColor(ANT_COLOR);

		if (phase == Marker::ToHome) {
			const float radius = 2.0f;
			sf::CircleShape circle(radius);
			circle.setOrigin(radius, radius);
			circle.setPosition(position + length * 0.5f * sf::Vector2f(cos(direction), sin(direction)));
			circle.setFillColor(sf::Color::White);
			target.draw(circle);
		}

		target.draw(body);
	}


	const sf::Vector2f colony;
	sf::Vector2f position;
	float direction;
	float last_direction_update;
	float last_marker;
	Marker::Type phase;
	Marker* target_marker;
	float reserve = 500.0f;
	const uint32_t id;

	const float direction_update_period = 0.25f;
	const float marker_period = 0.25f;
};