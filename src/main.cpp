#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

std::vector<std::function<float(float, float, float)>> tween_functions =
{
    
    [](float a, float b, float t)
    {
        const float pi = 3.14159f;
        float ease = 1.0f - std::cos((t*pi)/2.0f);

        return a + (b-a)*ease;
    },

    [](float a, float b, float t)
    {
        const float pi = 3.14159f;
        float ease = std::sin((t*pi)/2.0f);

        return a + (b-a)*ease;
    },

    [](float a, float b, float t)
    {
        const float pi = 3.14159f;
        float ease = -(std::cos(t*pi) -1)/2.0f;

        return a + (b-a)*ease;
    },

    [](float a, float b, float t)
    {
        const float pi = 3.14159f;
        float ease = t*t;

        return a + (b-a)*ease;
    },

    [](float a, float b, float t)
    {
        const float pi = 3.14159f;
        float ease = 1.0f - (1.0f - t)*(1.0f -t);

        return a + (b-a)*ease;
    },

    [](float a, float b, float t)
    {
        const float pi = 3.14159f;
        float ease = t < 0.5f ? 2.0f*t*t : 1.0f - std::pow(-2.0f*t + 2.0f, 2.0f)/2.0f;

        return a + (b-a)*ease;
    },

    [](float a, float b, float t)
    {
        const float pi = 3.14159f;
        float ease = t*t*t;

        return a + (b-a)*ease;
    },

    [](float a, float b, float t)
    {
        const float pi = 3.14159f;
        float ease = 1 - std::pow(1.0f - t, 3.0f);

        return a + (b-a)*ease;
    },

    [](float a, float b, float t)
    {
        const float pi = 3.14159f;
        float ease = t == 0.0f ? 0.0f:std::pow(2.0f, 10.0f*t -10.0f);

        return a + (b-a)*ease;
    }
};

void handleInput(sf::Window& window, bool& shouldQuit) {

    const sf::Keyboard::Scan numberKeys[9] =
    {
    sf::Keyboard::Scan::Num1,
    sf::Keyboard::Scan::Num2,
    sf::Keyboard::Scan::Num3,
    sf::Keyboard::Scan::Num4,
    sf::Keyboard::Scan::Num5,
    sf::Keyboard::Scan::Num6,
    sf::Keyboard::Scan::Num7,
    sf::Keyboard::Scan::Num8,
    sf::Keyboard::Scan::Num9
    };

    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }
        if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
    
            for (int i=0; i<9; i++)
            {
                if (keyPressed->scancode == numberKeys[i])
                {
                    tween = tween_functions[i];
                    std::cout << "Currently using tween function " << (i+1) <<std::endl;
                }
            }
            
        }
        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
    }
}

void render(sf::RenderWindow& window, float time) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    sf::CircleShape circle(20.0f);
    circle.setFillColor(sf::Color::Blue);
    circle.setOrigin({20.0f, 20.0f});

    float y_pos = WINDOW_HEIGHT/3.0f;
    float initial_x = 50.0f;
    float final_x = WINDOW_WIDTH - 50.0f; 
    float current_x = tween(initial_x, final_x, time);
    
    circle.setPosition({current_x, y_pos});
    window.draw(circle);

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======
    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // ====== ====== ======


    float graph_x_beginning_pt = 50.0f;
    float graph_y_beginning_pt = WINDOW_HEIGHT - 50.0f;
    float graph_width = 700.0f;
    float graph_height = 300.0f;
    const int num_points = 200;

    //used this as referene: https://www.sfml-dev.org/documentation/3.0.0/group__graphics.html
    sf::VertexArray graph_line(sf::PrimitiveType::LineStrip, num_points);
    //making the x and y axes
    sf::VertexArray graph_axes(sf::PrimitiveType::Lines, 4);    
    graph_axes[0].position = sf::Vector2f(graph_x_beginning_pt, graph_y_beginning_pt);
    graph_axes[1].position = sf::Vector2f(graph_x_beginning_pt +graph_width, graph_y_beginning_pt);
    graph_axes[2].position = sf::Vector2f(graph_x_beginning_pt, graph_y_beginning_pt);
    graph_axes[3].position = sf::Vector2f(graph_x_beginning_pt, graph_y_beginning_pt - graph_height);

    for (int pt=0; pt<4; pt++) {
        graph_axes[pt].color = sf::Color::White; 
    }

    for (int i = 0; i < num_points; i++)
    {
        float normalized_time = static_cast<float>(i)/(num_points - 1);
        float eased_value = tween(0.0f, 1.0f, normalized_time);
        float pixel_x = graph_x_beginning_pt + normalized_time*graph_width;
        float pixel_y = graph_y_beginning_pt - eased_value*graph_height;
        
        graph_line[i].position = sf::Vector2f(pixel_x, pixel_y);
        graph_line[i].color = sf::Color::Blue;
    }
    
    float small_circle_y_percent = tween(0.0f, 1.0f, time);
    float small_circle_x_screen = graph_x_beginning_pt + (time * graph_width);
    float small_circle_y_screen = graph_y_beginning_pt - (small_circle_y_percent * graph_height);

    sf::CircleShape small_circle(7.0f);
    small_circle.setFillColor(sf::Color::Yellow);
    small_circle.setOrigin({7.0f, 7.0f});
    small_circle.setPosition({small_circle_x_screen, small_circle_y_screen});
    
    window.draw(graph_axes);
    window.draw(graph_line);
    window.draw(small_circle);
    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        const float frames_per_animation = 60.0f;
        int current_frame = 0;
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }

            float time = current_frame/frames_per_animation;
            render(window, time);
            current_frame += 1;
            if (current_frame > frames_per_animation)
            {
                current_frame = 0;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
