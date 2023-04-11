#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H


class GraphicsSettings
	{
	public:
		GraphicsSettings();

		std::string title;
		sf::VideoMode resolution;
		bool fullScreen;
		bool verticalSync;
		unsigned frameRateLimit;
		sf::ContextSettings contextSettings;
		std::vector<sf::VideoMode> videoModes;

		void saveToFile(const std::string path);
		void loadFromFile(const std::string path);
};


#endif 