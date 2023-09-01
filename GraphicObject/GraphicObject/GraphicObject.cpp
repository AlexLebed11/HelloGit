#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class GraphicObject {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void move(float x, float y) = 0;
    virtual void save(ofstream& file) = 0;
    virtual void load(ifstream& file) = 0;
    virtual void changeColor(sf::Color color) = 0;
    virtual void changeSize(float size) = 0;    
    virtual void setVisible(bool visible) = 0;
    virtual bool isVisible() = 0;
};

class Circle : public GraphicObject {
private:
    sf::CircleShape shape;
    sf::Color fillColor;
    bool visible;

public:
    Circle(float radius = 10.f) {
        shape.setRadius(radius);
        shape.setFillColor(sf::Color::White);
        shape.setPosition(100.f, 100.f);
        visible = true;
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    } 
    void move(float x, float y) override {
        shape.move(x, y);
    }

    void save(ofstream& file) override {
        file << "Circle" << endl;
        file << shape.getPosition().x << " " << shape.getPosition().y << endl;
        file << shape.getRadius() << endl;
        file << shape.getFillColor().toInteger() << endl;
    }

    void load(ifstream& file) override {
        float x, y, radius;
        sf::Uint32 color;
        file >> x >> y >> radius >> color;
        shape.setPosition(x, y);
        shape.setRadius(radius);
        shape.setFillColor(sf::Color(color));
    }

    void changeColor(sf::Color color) override {
        fillColor = color;
        shape.setFillColor(color);
    }

    void changeSize(float size) override {
        shape.setRadius(size);
    }

    void setVisible(bool visible) override {
        this->visible = visible;
        if (visible)
            shape.setFillColor(fillColor);
        else
            shape.setFillColor(sf::Color::Transparent);
    }

    bool isVisible() override {
        return visible;
    }
};

class Rectangle : public GraphicObject {
private:
    sf::RectangleShape shape;
    bool visible;
    sf::Color fillColor;

public:
    Rectangle(float width = 20.f, float height = 30.f) {
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(200.f, 100.f);
        visible = true;
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }

    void move(float x, float y) override {
        shape.move(x, y);
    }

    void save(ofstream& file) override {
        file << "Rectangle" << endl;
        file << shape.getPosition().x << " " << shape.getPosition().y << endl;
        file << shape.getSize().x << " " << shape.getSize().y << endl;
        file << shape.getFillColor().toInteger() << endl;
    }

    void load(ifstream& file) override {
        float x, y, width, height;
        sf::Uint32 color;
        file >> x >> y >> width >> height >> color;
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color(color));
    }

    void changeColor(sf::Color color) override {
        fillColor = color;
        shape.setFillColor(color);
    }

    void changeSize(float size) override {
        sf::Vector2f newSize(size * (shape.getSize().x / max(shape.getSize().x, shape.getSize().y)), size * (shape.getSize().y / max(shape.getSize().x, shape.getSize().y)));
        shape.setSize(newSize);
    }

    void setVisible(bool visible) override {
        this->visible = visible;
        if (visible)
            shape.setFillColor(fillColor);
        else
            shape.setFillColor(sf::Color::Transparent);
    }

    bool isVisible() override {
        return visible;
    }
};

class Triangle : public GraphicObject {
private:
    sf::ConvexShape shape;
    sf::Color fillColor;
    bool visible;

public:
    Triangle(float size = 20.f) {
        shape.setPointCount(3);
        shape.setPoint(0, sf::Vector2f(0.f, -size));
        shape.setPoint(1, sf::Vector2f(size * sqrt(3) / 2, size / 2));
        shape.setPoint(2, sf::Vector2f(-size * sqrt(3) / 2, size / 2));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(300.f, 100.f);
        visible = true;
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }

    void move(float x, float y) override {
        shape.move(x, y);
    }

    void save(ofstream& file) override {
        file << "Triangle" << endl;
        file << shape.getPosition().x << " " << shape.getPosition().y << endl;
        file << shape.getPoint(0).x << " " << shape.getPoint(0).y << " "
             << shape.getPoint(1).x << " " << shape.getPoint(1).y << " "
             << shape.getPoint(2).x << " " << shape.getPoint(2).y << endl;
        file << shape.getFillColor().toInteger() << endl;
    }

    void load(ifstream& file) override {
        float x, y, x0, y0, x1, y1, x2, y2;
        sf::Uint32 color;
        file >> x >> y >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> color;
        shape.setPosition(x, y);
        shape.setPoint(0, sf::Vector2f(x0, y0));
        shape.setPoint(1, sf::Vector2f(x1, y1));
        shape.setPoint(2, sf::Vector2f(x2, y2));
        shape.setFillColor(sf::Color(color));
    }

    void changeColor(sf::Color color) override {
        fillColor = color;
        shape.setFillColor(color);
    }

    void changeSize(float size) override {
        float currentSize = sqrt(pow(shape.getPoint(0).x, 2) + pow(shape.getPoint(0).y, 2));
        float scaleFactor = size / currentSize;
        for (int i = 0; i < 3; i++) {
            float newX = shape.getPoint(i).x * scaleFactor;
            float newY = shape.getPoint(i).y * scaleFactor;
            shape.setPoint(i, sf::Vector2f(newX, newY));
        }
    }

    void setVisible(bool visible) override {
        this->visible = visible;
        if (visible)
            shape.setFillColor(fillColor);
        else
            shape.setFillColor(sf::Color::Transparent);
    }

    bool isVisible() override {
        return visible;
    }
};

class Aggregate : public GraphicObject {
private:
    vector<GraphicObject*> objects;
    bool visible;

public:
    void addObject(GraphicObject* object) {
        objects.push_back(object);
    }

    void draw(sf::RenderWindow& window) {
        for (auto object : objects) {
            object->draw(window);
        }
    }

    void move(float x, float y) {
        for (auto object : objects) {
            object->move(x, y);
        }
    }

    void save(ofstream& file) {
        file << "Aggregate" << endl;
        file << objects.size() << endl;
        for (auto object : objects) {
            object->save(file);
        }
    }

    void load(ifstream& file) {
        int size;
        file >> size;
        for (int i = 0; i < size; i++) {
            string type;
            file >> type;
            if (type == "Circle") {
                Circle* circle = new Circle();
                circle->load(file);
                objects.push_back(circle);
            }
            else if (type == "Rectangle") {
                Rectangle* rectangle = new Rectangle();
                rectangle->load(file);
                objects.push_back(rectangle);
            }
            else if (type == "Triangle") {
                Triangle* triangle = new Triangle();
                triangle->load(file);
                objects.push_back(triangle);
            }
            else if (type == "Aggregate") {
                Aggregate* aggregate = new Aggregate();
                aggregate->load(file);
                objects.push_back(aggregate);
            }
        }
    }

    void changeColor(sf::Color color) {
        for (auto object : objects) {
            object->changeColor(color);
        }
    }

    void changeSize(float size) {
        for (auto object : objects) {
            object->changeSize(size);
        }
    }

    void setVisible(bool visible) {
        this->visible = visible;
        for (auto object : objects) {
            object->setVisible(visible);
        }
    }

    bool isVisible() override {
        return visible;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graphic shapes");

    vector<GraphicObject*> objects;

    int currentObject = 0;
    bool trail = false;
    float scaleIncrement = 0.1f; 
    float currentScale = 1.0f;   

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {  
                window.close();
                break; 
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F1) {
                    sf::RenderWindow commandsWindow(sf::VideoMode(400, 300), "Help");
                    sf::Font font;
                    if (!font.loadFromFile("D:/Font_arial/arial.ttf")) {}

                    sf::Text text;
                    text.setFont(font);
                    text.setCharacterSize(14);

                    sf::Color textColor(17, 59, 8);
                    text.setFillColor(textColor);

                    text.setPosition(0, 0);

                    text.setString("Commands:\n"
                        "C - create circle\n"
                        "R - create rectangle\n"
                        "T - create triangle\n"
                        "A - create aggregate\n"
                        "Tab - switch between objects\n"
                        "Up/Down/Left/Right - move object\n"
                        "E - toggle trail\n"
                        "S - save state to file\n"
                        "L - load state from file\n"
                        "1 - change color to red\n"
                        "2 - change color to green\n"
                        "3 - change color to blue\n"
                        "+ - increase size\n"
                        "- - decrease size\n"
                        "V - toggle visibility");

                    while (commandsWindow.isOpen()) {
                        sf::Event event;
                        while (commandsWindow.pollEvent(event)) {
                            if (event.type == sf::Event::Closed) {
                                commandsWindow.close();
                            }
                            else if (event.type == sf::Event::KeyPressed) {
                                if (event.key.code == sf::Keyboard::Escape) {
                                    commandsWindow.close();
                                }
                            }
                        }
                        commandsWindow.clear(sf::Color::Black);
                        commandsWindow.draw(text);
                        commandsWindow.display();
                    }
                }
                if (event.key.code == sf::Keyboard::C) {
                    objects.push_back(new Circle());
                    currentObject = objects.size() - 1;
                }
                if (event.key.code == sf::Keyboard::R) {
                    objects.push_back(new Rectangle());
                    currentObject = objects.size() - 1;
                }
                if (event.key.code == sf::Keyboard::T) {
                    objects.push_back(new Triangle());
                    currentObject = objects.size() - 1;
                }
                if (event.key.code == sf::Keyboard::A) {
                    objects.push_back(new Aggregate());
                    currentObject = objects.size() - 1;
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Tab) {
                        currentObject++;
                        if (currentObject >= objects.size())
                            currentObject = 0;
                    }
                    if (event.key.code == sf::Keyboard::Up) {
                        if (!objects.empty())
                            objects[currentObject]->move(0, -10);
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        if (!objects.empty())
                            objects[currentObject]->move(0, 10);
                    }
                    if (event.key.code == sf::Keyboard::Left) {
                        if (!objects.empty())
                            objects[currentObject]->move(-10, 0);
                    }
                    if (event.key.code == sf::Keyboard::Right) {
                        if (!objects.empty())
                            objects[currentObject]->move(10, 0);
                    }
                    if (event.key.code == sf::Keyboard::E) {
                        trail = !trail;
                    }
                    if (event.key.code == sf::Keyboard::S) {
                        sf::RenderWindow saveWindow(sf::VideoMode(400, 100), "Save File");

                        sf::Font font;
                        if (!font.loadFromFile("D:/Font_arial/arial.ttf")) {}

                        sf::Text text;
                        text.setFont(font);
                        text.setCharacterSize(14);
                        text.setString("Enter filename:");
                        text.setPosition(10, 10);

                        sf::RectangleShape inputBox(sf::Vector2f(380, 30));
                        inputBox.setFillColor(sf::Color::White);
                        inputBox.setPosition(10, 50);

                        sf::Text inputText;
                        inputText.setFont(font);
                        inputText.setCharacterSize(14);
                        inputText.setFillColor(sf::Color::Black);
                        inputText.setPosition(15, 55);

                        string filename;

                        while (saveWindow.isOpen()) {
                            sf::Event saveEvent;
                            while (saveWindow.pollEvent(saveEvent)) {
                                if (saveEvent.type == sf::Event::Closed) {
                                    saveWindow.close();
                                }
                                else if (saveEvent.type == sf::Event::TextEntered) {
                                    if (saveEvent.text.unicode == 13) {
                                        ofstream file(filename);
                                        file << objects.size() << endl;
                                        for (auto object : objects)
                                            object->save(file);
                                        file.close();
                                        saveWindow.close();
                                    }
                                    else if (saveEvent.text.unicode == 8 && !filename.empty()) { 
                                        filename.pop_back();
                                        inputText.setString(filename);
                                    }
                                    else if (saveEvent.text.unicode < 128) {
                                        filename += static_cast<char>(saveEvent.text.unicode);
                                        inputText.setString(filename);
                                    }
                                }
                            }
                            saveWindow.clear(sf::Color::Black);
                            saveWindow.draw(text);
                            saveWindow.draw(inputBox);
                            saveWindow.draw(inputText);
                            saveWindow.display();
                        }
                    }
                    if (event.key.code == sf::Keyboard::L) {
                        sf::RenderWindow loadWindow(sf::VideoMode(400, 100), "Load File");

                        sf::Font font;
                        if (!font.loadFromFile("D:/Font_arial/arial.ttf")) {}

                        sf::Text text;
                        text.setFont(font);
                        text.setCharacterSize(14);
                        text.setString("Enter filename:");
                        text.setPosition(10, 10);

                        sf::RectangleShape inputBox(sf::Vector2f(380, 30));
                        inputBox.setFillColor(sf::Color::White);
                        inputBox.setPosition(10, 50);

                        sf::Text inputText;
                        inputText.setFont(font);
                        inputText.setCharacterSize(14);
                        inputText.setFillColor(sf::Color::Black);
                        inputText.setPosition(15, 55);

                        string filename;

                        while (loadWindow.isOpen()) {
                            sf::Event loadEvent;
                            while (loadWindow.pollEvent(loadEvent)) {
                                if (loadEvent.type == sf::Event::Closed) {
                                    loadWindow.close();
                                }
                                else if (loadEvent.type == sf::Event::TextEntered) {
                                    if (loadEvent.text.unicode == 13) {
                                        ifstream file(filename);
                                        if (file.is_open()) {
                                            int size;
                                            file >> size;
                                            for (int i = 0; i < size; i++) {
                                                string type;
                                                file >> type;
                                                if (type == "Circle") {
                                                    Circle* circle = new Circle();
                                                    circle->load(file);
                                                    objects.push_back(circle);
                                                }
                                                else if (type == "Rectangle") {
                                                    Rectangle* rectangle = new Rectangle();
                                                    rectangle->load(file);
                                                    objects.push_back(rectangle);
                                                }
                                                else if (type == "Aggregate") {
                                                    Aggregate* aggregate = new Aggregate();
                                                    aggregate->load(file);
                                                    objects.push_back(aggregate);
                                                }
                                            }
                                            file.close();
                                        }
                                        loadWindow.close();
                                    }
                                    else if (loadEvent.text.unicode == 8 && !filename.empty()) {
                                        filename.pop_back();
                                        inputText.setString(filename);
                                    }
                                    else if (loadEvent.text.unicode < 128) {
                                        filename += static_cast<char>(loadEvent.text.unicode);
                                        inputText.setString(filename);
                                    }
                                }
                            }
                            loadWindow.clear(sf::Color::Black);
                            loadWindow.draw(text);
                            loadWindow.draw(inputBox);
                            loadWindow.draw(inputText);
                            loadWindow.display();
                        }
                    }
                    if (event.key.code == sf::Keyboard::Num1) {
                        if (!objects.empty())
                            objects[currentObject]->changeColor(sf::Color::Red);
                    }
                    if (event.key.code == sf::Keyboard::Num2) {
                        if (!objects.empty())
                            objects[currentObject]->changeColor(sf::Color::Green);
                    }
                    if (event.key.code == sf::Keyboard::Num3) {
                        if (!objects.empty())
                            objects[currentObject]->changeColor(sf::Color::Blue);
                    }
                    if (event.key.code == sf::Keyboard::Add) { 
                        if (!objects.empty()) {
                            currentScale += scaleIncrement; 
                            objects[currentObject]->changeSize(currentScale);
                        }
                    }
                    if (event.key.code == sf::Keyboard::Subtract) { 
                        if (!objects.empty()) {
                            currentScale -= scaleIncrement; 
                            if (currentScale < 0.1f) { 
                                currentScale = 0.1f;
                            }
                            objects[currentObject]->changeSize(currentScale);
                        }
                    }
                    if (event.key.code == sf::Keyboard::V) {
                        if (!objects.empty()) {
                            bool currentVisibility = objects[currentObject]->isVisible();
                            objects[currentObject]->setVisible(!currentVisibility);
                        }
                    }
                }
            }
            if (!trail)
                window.clear();
            for (auto object : objects)
                object->draw(window);
            window.display();
        }
    }
    return 0;
}