#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <SFML/Graphics.hpp>
using namespace sf;

#include <map>
#include <vector>
using namespace std;

#include "util/Commons.hpp"

struct Animation;
struct AnimationFrame;

/** An animated sprite that is rendered from a sprite sheet.
 * Features:
 *    - Play, pause, and stop current animation.
 *    - Set predefined animation actions (eg: "Run" animation).
*/
class AnimatedSprite
{
    public:
        /** Creates a new AnimatedSprite. */
        AnimatedSprite(const Image &image);
        virtual ~AnimatedSprite();

        void addAnimation(Animation *animation);

        /** Updates the animation.
         * @param clock The clock to measure elapsed time since the last frame.
         */
        void update(Clock *clock);

        /** Clears the animations of any animation-specific transformations
         * produced by update().
         */
        void clear();

        /** Stops the current animation (if any) and starts a new aniamtion from the beginning.
         * @param animationName The name of the animation to play.
         * @return True if animationId references a set animation. False otherwise.
         */
        bool play(string animationName);

        /** Pauses the current animation (if one exists). */
        void pause();

        /** Resumes the current animation (if one exists). */
        void resume();

        /** Stops the current animation (if one exists). */
        void stop();

        /** Faces the sprite to the right. */
        void lookLeft();

        /** Faces the sprite to the left. */
        void lookRight();

        Vector2f getPosition();

        void setPosition(float x, float y);

        Vector2f getSize();

        bool collide(AnimatedSprite &other);

        Vector2f toGlobal(const Vector2f &point);

        Vector2f toLocal(const Vector2f &point);

        Image* getImage();

        void setSize(float x, float y);

    private:
        // Animation storage
        map<string, Animation*> animations;
        Animation *currentAnimation;
        int currentFrame;

        // Active animation state
        float timeSinceLastFrame;
        bool paused;
        Direction direction;

        Vector2f originalScale;
        Vector2f originalPosition;

        // Underlying Sprite and image
        Sprite sprite;
        Image image;

        // Helper methods
        void updateDirection();
};

struct Animation
{
    string name;
    int frameRate; // Frames per second
    bool loop;
    bool pingPong;
    vector<AnimationFrame*> frames; // Ordered list of frames
};

struct AnimationFrame
{
    string name;
    int id;
    int x;
    int y;
    int width;
    int height;
};

#endif // ANIMATEDSPRITE_H
