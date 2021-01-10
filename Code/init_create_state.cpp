/**
 * init_create_state.cpp
 * Implementation of InitCreateState class.
 */

#include "init_create_state.hpp"
#include "definitions.hpp"
#include "main_menu_state.hpp"
#include "creator_state.hpp"
#include <iostream>
#include <memory>

namespace zpr {

    /**
     * Parametrized constructor of InitCreateState class.
     * @param data - Struct containing data of current application. (eg. window, assets)
     */
	InitCreateState::InitCreateState(SimulatorDataRef data) : data_(data){}

    /**
     * Methods which initializes all elements in the current state to display it properly.
     */
	void InitCreateState::init() {
		this->data_->assets_.loadTexture("Background", MENU_BACKGROUND_FILEPATH);
		this->data_->assets_.loadTexture("Button", BUTTON_FILEPATH);
		this->data_->assets_.loadFont("Text font", TEXT_FONT_FILEPATH);

        this->initializeButtons();

		this->background_.setTexture(this->data_->assets_.getTexture("Background"));
        this->background_.scale(2.35, 2);
	}

    /**
     * Method which initializes every button in the window.
     */
    void InitCreateState::initializeButtons(){
        sf::Vector2f button_size(150, 66);
        int font_size = 30;
        this->buttons_.push_back(Button(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 3 * button_size.y), button_size, "16x16",
            this->data_->assets_.getFont("Text font"), font_size, sf::Color::White, this->data_->assets_.getTexture("Button")));
        
        this->buttons_.push_back(Button(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 1 * button_size.y), button_size, "32x32",
            this->data_->assets_.getFont("Text font"), font_size, sf::Color::White, this->data_->assets_.getTexture("Button")));
        
        this->buttons_.push_back(Button(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 1 * button_size.y), button_size, "64x64",
            this->data_->assets_.getFont("Text font"), font_size, sf::Color::White, this->data_->assets_.getTexture("Button")));
        
        this->buttons_.push_back(Button(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 5 * button_size.y), button_size, "Back",
            this->data_->assets_.getFont("Text font"), font_size, sf::Color::White, this->data_->assets_.getTexture("Button")));
    }

    /**
     Method which handles user input in the current state.
     */
	void InitCreateState::handleInput() {
		sf::Event event;
		while (this->data_->window_.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				this->data_->window_.close();
			}
			for (Button button : this->buttons_)
			{
				if (button.isClicked(sf::Mouse::Left, this->data_->window_))
				{
					if (button.getText() != "Back")
					{
						int grid_size = getGridSizeFromButton(button);
						this->data_->machine_.addState(StateRef(new CreatorState(this->data_, grid_size)), false);
					}
					else
					{
						this->data_->machine_.addState(StateRef(new MainMenuState(this->data_)), false);
					}
				}
			}
        }          
	}
    /**
     * Method returning size of grid depending from which button was clicked.
     * @param button - Clicked button.
     * @return - Grid size.
     */
	int InitCreateState::getGridSizeFromButton(Button button)
	{
		std::string button_text = button.getText();
		std::string text_to_change = button_text.substr(0, 2);
        return changeTextToInt(button_text);
	}
    /**
     * Method returning size of grid from button text
     * @param text_to_change - Text which should be converted to int
     * @return - Grid size.
     */
	int InitCreateState::changeTextToInt(std::string text_to_change)
	{
		return std::stoi(text_to_change, nullptr);
	}

    /**
     * Method which updates the window.
     * @param dt - Frequency of updating.
     */
	void InitCreateState::update(float dt) {
	}

    /**
     * Methods which draws elements of state on the screen.
     * @param dt - Frequency of drawing.
     */
	void InitCreateState::draw(float dt) {
		this->data_->window_.clear(sf::Color::Black);
		this->data_->window_.draw(this->background_);
		for (Button button : this->buttons_) {
			this->data_->window_.draw(button);
		}
		this->data_->window_.display();
	}

	}
