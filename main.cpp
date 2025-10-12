#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <fstream>
#include <sstream>

int main()
{
    sf::RenderWindow window{{1280, 900}, "TGUI Designer Form Example"};
    window.setFramerateLimit(60);

    tgui::Gui gui{window};

    try
    {
        // Загружаем интерфейс из файла form.tgui
        gui.loadWidgetsFromFile("form.tgui");
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "Failed to load form.tgui: " << e.what() << std::endl;
        return 1;
    }

    // Получаем доступ к виджетам
    auto tabs = gui.get<tgui::Tabs>("Tabs1");
    auto textArea = gui.get<tgui::EditBox>("TextArea1"); // или TextBox, в зависимости от версии

    //ПРИМЕР обработка выбора вкладки
    tabs->onTabSelect([](const tgui::String& tab){
        std::cout << "Selected tab: " << tab << std::endl;
    });

    // Основной цикл
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            gui.handleEvent(event);
        }

        window.clear(sf::Color(50, 50, 50));
        gui.draw();
        window.display();
    }

    return 0;
}
