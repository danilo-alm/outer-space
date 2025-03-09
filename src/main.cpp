#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <algorithm>
#include <cstdint>
#include <random>
#include "events.hpp"
#include "configuration.hpp"
#include "star.hpp"

std::vector<Star> createStars(uint32_t count, float scale)
{
	std::vector<Star> stars;
	stars.reserve(count);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	// Define a star free zone
	sf::Vector2f const window_world_size = conf::window_size_f * conf::near;
	sf::FloatRect const star_free_zone = {-window_world_size * 0.5f, window_world_size};

	for (uint32_t i{count}; i--;)
	{
		float const x = (dis(gen) - 0.5f) * conf::window_size_f.x * scale;
		float const y = (dis(gen) - 0.5f) * conf::window_size_f.y * scale;
		float const z = dis(gen) * (conf::far - conf::near) + conf::near;

		// Discard any star that falls in the zone
		if (star_free_zone.contains({x, y}))
		{
			++i;
			continue;
		};

		stars.push_back({{x, y}, z});
	}

	// Depth ordering
	std::sort(stars.begin(), stars.end(), [](Star const& s_1, Star const s_2) {
		return s_1.z > s_2.z;
	});
	
	return stars;
}

void updateGeometry(uint32_t idx, Star const& s, sf::VertexArray& va)
{
	float const scale = 1.0f / s.z;
	float const depth_ratio = (s.z - conf::near) / (conf::far - conf::near);
	float const color_ratio = 1.0f - depth_ratio;
	auto const c = static_cast<uint8_t>(color_ratio * 255.0f);

	sf::Vector2f const p = s.position * scale;
	float const r = conf::radius * scale;
	uint32_t const i = 6 * idx;

	va[i + 0].position = {p.x - r, p.y + r};
	va[i + 1].position = {p.x - r, p.y - r};
	va[i + 2].position = {p.x + r, p.y + r};

	va[i + 3].position = {p.x + r, p.y + r};
	va[i + 4].position = {p.x - r, p.y - r};
	va[i + 5].position = {p.x + r, p.y - r};

	sf::Color const color{c, c, c};
	va[i + 0].color = color;
	va[i + 1].color = color;
	va[i + 2].color = color;
	va[i + 3].color = color;
	va[i + 4].color = color;
	va[i + 5].color = color;
}

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({ conf::window_size.x, conf::window_size.y }), "CMake SFML Project", sf::State::Fullscreen);
    window.setFramerateLimit(conf::max_framerate);
	window.setMouseCursorVisible(false);

	sf::Texture texture;
	if (!texture.loadFromFile("res/star.png"))
	{
		throw std::runtime_error("Coudn't load star texture");
	}
	texture.setSmooth(true);
	if (!texture.generateMipmap())
	{
		throw std::runtime_error("Couldn't generate texture minmap");
	}

	std::vector<Star> stars = createStars(conf::count, conf::far);

	sf::VertexArray va{sf::PrimitiveType::Triangles, 6 * conf::count};

	// Pre fill texture coords, since they will remain constant
	auto const texture_size_f = static_cast<sf::Vector2f>(texture.getSize());
	for (uint32_t idx{conf::count}; --idx;)
	{
		uint32_t const i = 6 * idx;
		va[i + 0].texCoords = {0.0f, 0.0f};
		va[i + 1].texCoords = {0.0f, texture_size_f.y};
		va[i + 2].texCoords = {texture_size_f.x, 0.0f};
		va[i + 3].texCoords = {texture_size_f.x, 0.0f};
		va[i + 4].texCoords = {0.0f, texture_size_f.y};
		va[i + 5].texCoords = {texture_size_f.x, texture_size_f.y};
	}

	uint32_t first = 0;
    while (window.isOpen())
    {
		processEvents(window);

		// Fake travel increasing Z
		for (uint32_t i{conf::count}; i--;)
		{
			Star& s = stars[i];
			s.z -= conf::speed * conf::dt;
			if (s.z < conf::near)
			{
				// Offset the star by the excess travel it made behind near to keep spacing constant
				s.z = conf::far - (conf::near - s.z);
				// This star is now the first we need to draw because it is the further away from us
				first = i;
			}
		}

        window.clear();

		for (uint32_t i{0}; i < conf::count; ++i)
		{
			uint32_t const idx = (i + first) % conf::count;
			Star const& s = stars[idx];
			updateGeometry(i, s, va);
		}

		sf::RenderStates states;
		states.transform.translate(conf::window_size_f * 0.5f);
		states.texture = &texture;

		window.draw(va, states);
        window.display();
    }
}

