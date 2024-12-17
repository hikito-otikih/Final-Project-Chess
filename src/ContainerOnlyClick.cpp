#include <Book/ContainerOnlyClick.hpp>
#include <Book/Foreach.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{

	ContainerOnlyClick::ContainerOnlyClick()
		: mChildren()
		, mSelectedChild(-1)
	{
	}

	void ContainerOnlyClick::pack(Component::Ptr component)
	{
		mChildren.push_back(component);

		if (!hasSelection() && component->isSelectable())
			select(mChildren.size() - 1);
	}

	void ContainerOnlyClick::ressetSelectedChild()
	{
		mSelectedChild = 0;
		select(0);
	}

	void ContainerOnlyClick::changeComponent()
	{
		mChildren.clear();
	}

	bool ContainerOnlyClick::isSelectable() const
	{
		return false;
	}

	void ContainerOnlyClick::handleEvent(const sf::Event& event)
	{
		// If we have selected a child then give it events
		if (hasSelection() && mChildren[mSelectedChild]->isActive())
		{
			mChildren[mSelectedChild]->handleEvent(event);
		}
		else
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
					FOREACH(const Component::Ptr & child, mChildren)
					{
						sf::Vector2f hitBox = child->getPosition();
						sf::FloatRect hitBox2 = child->getSizeSprite();
						hitBox2.left += hitBox.x;
						hitBox2.top += hitBox.y;
						if (hitBox2.contains(mousePos))
						{
							if (child->isSelectable())
							{
								child->activate();
								break;
							}
						}
					}
				}
			}
			else
				if (event.type == sf::Event::MouseMoved)
				{
					sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
					std::size_t loop = 0;
					FOREACH(const Component::Ptr & child, mChildren)
					{
						sf::Vector2f hitBox = child->getPosition();
						sf::FloatRect hitBox2 = child->getSizeSprite();
						hitBox2.left += hitBox.x;
						hitBox2.top += hitBox.y;
						if (hitBox2.contains(mousePos))
						{
							select(loop);
							break;
						}
						loop++;
					}
				}
	}

	void ContainerOnlyClick::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		FOREACH(const Component::Ptr & child, mChildren)
			target.draw(*child, states);
	}

	bool ContainerOnlyClick::hasSelection() const
	{
		return mSelectedChild >= 0;
	}

	void ContainerOnlyClick::select(std::size_t index)
	{
		if (mChildren[index]->isSelectable())
		{
			if (hasSelection())
				mChildren[mSelectedChild]->deselect();

			mChildren[index]->select();
			mSelectedChild = index;
		}
	}

	void ContainerOnlyClick::selectNext()
	{
		if (!hasSelection())
			return;

		// Search next component that is selectable, wrap around if necessary
		int next = mSelectedChild;
		do
			next = (next + 1) % mChildren.size();
		while (!mChildren[next]->isSelectable());

		// Select that component
		select(next);
	}

	void ContainerOnlyClick::selectPrevious()
	{
		if (!hasSelection())
			return;

		// Search previous component that is selectable, wrap around if necessary
		int prev = mSelectedChild;
		do
			prev = (prev + mChildren.size() - 1) % mChildren.size();
		while (!mChildren[prev]->isSelectable());

		// Select that component
		select(prev);
	}

}
