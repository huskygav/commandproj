#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

int main()
{
    // Создаем окно SFML
    sf::RenderWindow window{{800, 600}, "TGUI example - SFML_GRAPHICS backend"};

    // Создаем GUI, привязанный к окну
    tgui::Gui gui{window};

    // Создаем кнопку
    auto button = tgui::Button::create("Click me");
    button->setPosition(350, 250);
    button->setSize(100, 50);
    gui.add(button);

    // Создаем текстовое поле
    auto editBox = tgui::EditBox::create();
    editBox->setPosition(300, 150);
    editBox->setSize(200, 40);
    editBox->setText("Type here...");
    gui.add(editBox);

    // Обработчик нажатия кнопки
    button->onPress([editBox](){
        editBox->setText("Button clicked!");
    });

    // Запуск основного цикла TGUI (он сам обрабатывает события и рисует GUI)
    gui.mainLoop();

    return 0;
}
