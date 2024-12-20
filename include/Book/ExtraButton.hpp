#ifndef EXTRABUTTON_HPP
#define EXTRABUTTON_HPP

#include <Book/Component.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>


class SoundPlayer;

namespace GUI
{

    class ExtraButton : public Component
    {
    public:
        typedef std::shared_ptr<ExtraButton>		Ptr;
        typedef std::function<void()>		Callback;

        enum Type
        {
            Normal,
            Selected,
            Pressed,
            ButtonCount
        };


    public:
        ExtraButton(State::Context context);

        void					setCallback(Callback callback);
        void					setText(const std::string& text);
        void					setToggle(bool flag);

        virtual bool			isSelectable() const;
        virtual void			select();
        virtual void			deselect();

        virtual void			activate();
        virtual void			deactivate();

        virtual void			handleEvent(const sf::Event& event);
        virtual sf::FloatRect	getSizeSprite() const;


    private:
        virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void					changeTexture(Type buttonType);


    private:
        Callback				mCallback;
        sf::Sprite				mSprite;
        sf::Text				mText;
        bool					mIsToggle;
        SoundPlayer& mSounds;
    };

}

#endif // EXTRABUTTON_HPP
