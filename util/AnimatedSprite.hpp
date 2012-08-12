#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <SFML/Graphics.hpp>
using namespace sf;

#include <map>
#include <vector>
using namespace std;

struct Animation;
struct AnimationFrame;

/** An animated sprite that is rendered from a sprite sheet.
 * Features:
 *    - Play, pause, and stop current animation.
 *    - Set predefined animation actions (eg: "Run" animation).
*/
class AnimatedSprite: public Sprite
{
    public:
        /** Creates a new AnimatedSprite. */
        AnimatedSprite();
        virtual ~AnimatedSprite();

        void addAnimation(Animation *animation);

        void setTransparentColour(unsigned int tColour);

        /** Updates the animation.
         * @param clock The clock to measure elapsed time since the last frame.
         */
        void update(Clock *clock);

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

        /** Set the display size of sprite frames.
         * @param size The new size.
         */
        void setFrameSize(Vector2f size);

        /** Set the origin of sprite frames.
         *@param center The new center.
         */
        void setFrameOrigin(Vector2f center);

    private:
        // Animation storage
        map<string, Animation*> animations;
        Animation *currentAnimation;
        int currentFrame;

        // Active animation state
        float timeSinceLastFrame;
        bool paused;
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
