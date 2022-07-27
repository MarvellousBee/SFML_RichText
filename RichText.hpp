#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <string>

namespace Rich
{
    std::vector<std::string> split_by_newline(const std::string& s){
        std::vector<std::string> output;
        std::string curr{ "" };
        for (const char& c : s)
            if (c != '\n')
                curr += c;
            else{
                output.push_back(curr);
                curr = "";
            }
        output.push_back(curr);
        return output;
    }

    class Text : public sf::Drawable
    {
        std::list<sf::Text> texts;
        sf::Color curr_color{ sf::Color::White };
        sf::Text::Style curr_style{ sf::Text::Style::Regular };
        float char_size{ 0 };
        sf::Vector2f starting_pos{ 0.f, 0.f };
        sf::Font curr_font;
        sf::Vector2f last_text_end{ starting_pos };

    public:
        friend Text& operator<<(Text& os, std::string str);
        friend Text& operator<<(Text& os, const sf::Color& color);
        friend Text& operator<<(Text& os, const sf::Text::Style& style);
        void setCharacterSize(float size){
            char_size = size;
        }
        void setPosition(sf::Vector2f pos){
            starting_pos = pos;
        }
        void setPosition(float x, float y){
            setPosition({ x, y });
        }
        void setFont(sf::Font font){
            curr_font = font;
        }
        void clear(){
            texts.clear();
            curr_color = sf::Color::White;
            curr_style = sf::Text::Style::Regular;
        }

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
            for (auto& i : texts)
                target.draw(i);
        }
    };

    Text& operator<<(Text& os, std::string string){
        if (os.texts.size() < 1)
            os.last_text_end = os.starting_pos;
        auto arr{ split_by_newline(string) };
        int i{ 0 };
        for (std::string& bstr : arr){
            sf::Text text;
            text.setString(bstr);
            text.setFont(os.curr_font);
            text.setFillColor(os.curr_color);
            text.setStyle(os.curr_style);
            if (os.char_size)
                text.setCharacterSize(os.char_size);
            text.setPosition(os.last_text_end);
            if (++i < arr.size())
                os.last_text_end = sf::Vector2f(os.starting_pos.x, os.last_text_end.y + text.getCharacterSize());
            else
                os.last_text_end = text.findCharacterPos(-1);
            os.texts.push_back(text);
        }
        return os;
    }

    Text& operator<<(Text& os, const char& ch){
        os << std::string(1, ch);
        return os;
    }

    Text& operator<<(Text& os, const sf::Color& color){
        os.curr_color = color;
        return os;
    }
    Text& operator<<(Text& os, const sf::Text::Style& style){
        os.curr_style = style;
        return os;
    }
};
