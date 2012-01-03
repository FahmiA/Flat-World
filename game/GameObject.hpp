#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class World;

#include <SFML/Graphics.hpp>
using namespace sf;

/** Provides a prototcole for all objects that must be rendered to the screen.
  * Such objects must respond to update and draw requests as well as provide
  * functionality for getting their current position and dimentions.
  * All GameObjects also have a settable ID for use during "instanceof" checks.
  */
class GameObject
{
    public:
        /** Updates the state of the game object.
          * As much logic as possible should be processed here and not in the draw method.
          * update(..) will always be called before draw(..).
          * NO draw(..) operations should occour in update(...).
          * @param clock The game clock to be used for checking time and frame delays.
          * @param window The RenderWIndow to be used for checking program state.
          * @param world The state of the game world.
          */
        virtual void update(Clock *clock, RenderWindow *window, World *world) = 0;

        /** Draws the GameObject and related elements to the screen.
          * As much as possible no game logic should occour here, only drawing.
          * @param window The RenderWindow to use for drawing.
          */
        virtual void draw(RenderWindow *window) = 0;

        /** Gets the (x, y) position of the GameObject. */
        virtual const Vector2f& getPosition() = 0;

        /** Gets the (width, height) size (in pixels) of the GameObject. */
        virtual const Vector2f& getSize() = 0;

        /** Gets the ID assigned to the GameObject. */
        int getID();

    protected:
        /** Assigns an ID to the GameObject. */
        void setID(int id);

    private:
        /** ID use to determin the type of the GameObject in "instanceof" checks. */
        int id;
};

// For your conveniance, copy and paste the below into your new GameObject subclasses:
/* .hpp
    void update(Clock *clock, RenderWindow *window, World *world);
    void draw(RenderWindow *window);

    const Vector2f& getPosition();
    const Vector2f& getSize();
*/
/* .cpp
void myClass::update(Clock *clock, RenderWindow *window, World *world)
{
}

void myClass::draw(RenderWindow *window)
{
}

const Vector2f& myClass::getPosition()
{
}

const Vector2f& myClass::getSize()
{
}
*/

#endif // GAMEOBJECT_H
