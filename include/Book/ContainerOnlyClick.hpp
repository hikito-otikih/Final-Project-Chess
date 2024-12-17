#ifndef CONTAINERONLYCLICK_HPP
#define CONTAINERONLYCLICK_HPP

#include <Book/Component.hpp>

#include <vector>
#include <memory>


namespace GUI
{

    class ContainerOnlyClick : public Component
    {
    public:
        typedef std::shared_ptr<ContainerOnlyClick> Ptr;


    public:
        ContainerOnlyClick();

        void				pack(Component::Ptr component);
		void                changeComponent();
		void                ressetSelectedChild();
		void                changeSelectedChild(int i) { mSelectedChild = i; }

        virtual bool		isSelectable() const;
        virtual void		handleEvent(const sf::Event& event);


    private:
        virtual void		draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool				hasSelection() const;
        void				select(std::size_t index);
        void				selectNext();
        void				selectPrevious();


    private:
        std::vector<Component::Ptr>		mChildren;
        int								mSelectedChild;
    };

}

#endif // CONTAINERONLYCLICK_HPP