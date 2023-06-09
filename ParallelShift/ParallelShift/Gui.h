#ifndef GUI_H
#define GUI_H 

enum button_states{ BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE};

namespace gui
{ 
	class Button
	{
	private:
	
		short unsigned buttonState;
		short unsigned id;

		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlinedIdleColor;
		sf::Color outlinedHoverColor;
		sf::Color outlinedActiveColor;

	public:
		Button(float x, float y, float width, float height, 
			sf::Font* font, std::string text, unsigned character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
			sf::Color outlined_idle_color = sf::Color::Transparent,
			sf::Color outlined_hover_color = sf::Color::Transparent, 
			sf::Color outlined_active_color = sf::Color::Transparent,
			short unsigned id = 0);
		virtual ~Button();

		const bool isPressed() const;
		const std::string getText() const;
		const short unsigned& getId() const;

		void setText(const std::string text);
		void setId(const short unsigned id);

		void updatePressButton(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};

	class DropDownList {

	private :
		float keytime;
		const float keytimeMax;
		sf::Font& font;
		gui::Button* activeElement;
		std::vector<gui::Button*> list;
		bool showList;

	public:
		DropDownList(float x, float y, float width, float height, sf::Font& font,
			std::string list[], unsigned nrOfelements, unsigned default_index = 0);
		virtual ~DropDownList();

		const unsigned short getActiveElementId() const;

		const bool getKeytime();
		void updateKeytime(const float& dt);
		void updatePressButton(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};

	class TextureSelector 
	{
	private :
		sf::RectangleShape bounds;
		sf::Sprite sheet;
		sf::RectangleShape selector;
		sf::Vector2u mousePosGrid;
		sf::IntRect textureRect;
		gui::Button* hide_btn;
		float gridSize;
		float keytime;
		const float keytimeMax;
		bool active;
		bool hidden;

	public :
		TextureSelector(float x, float y, float width, float height, float gridSize, const sf::Texture* texture_sheet,
			sf::Font& font, std::string text);
		~TextureSelector();

		const bool& getActive() const;
		const bool getKeytime();
		const sf::IntRect& getTextureRect() const;
		void updateKeytime(const float& dt);

		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};
}

#endif 