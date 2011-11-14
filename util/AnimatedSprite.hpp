#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <SFML/Graphics.hpp>
using namespace sf;

#include <map>
using namespace std;

struct AnimationDetail;

/** An animated sprite that is rendered from a sprite sheet.
    Features:
        * Play, pause, and stop current animation.
        * Set predefined animation actions (eg: "Run" animation from frames x - y)
*/
class AnimatedSprite
{
    public:
        AnimatedSprite(Sprite *spriteSheet, int framesPerSecond,
                       int frameHeight, int frameWidth, int frameGap,
                       int startOffsetX = 0, int startOffsetY = 0);
        virtual ~AnimatedSprite();

        /** Add a new animation from the sprite sheet.
            id = Assigns an id for future reference of the animation.
            startFrameX = The column number of the start frame (not the pixel position).
            startFrameY = The row number of the start frame (not the pixel position).
            noOfFrames = The number of sequential frames that make up the animation.
        */
        void addAnimation(int id, int startFrameX, int startFrameY, int noOfFrames);

        void update(RenderWindow *window, Clock *clock);
        bool play(int animationId);
        bool stop();

    private:
        // Sprite Sheet
        Sprite* spriteSheet;

        // Sprite Sheet configuration
        float frameDelay;
        int frameHeight;
        int frameWidth;
        int frameGap;
        int startOffsetX;
        int startOffsetY;

        // Animation storage
        map<int, AnimationDetail*> *animations;

        // Active animation state
        AnimationDetail *currentAnimation;
        int currentFrame;
        float lastFrameTime;

};

struct AnimationDetail
{
    int startFrameX; // Pixel position
    int startFrameY; // Pixel position
    int noOfFrames;
};

#endif // ANIMATEDSPRITE_H
