#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <SFML/Graphics.hpp>
using namespace sf;

#include <map>
using namespace std;

struct AnimationDetail;

/** An animated sprite that is rendered from a sprite sheet.
 * Features:
 *    - Play, pause, and stop current animation.
 *    - Set predefined animation actions (eg: "Run" animation from frames x - y).
*/
class AnimatedSprite: public Sprite
{
    public:
        /** Creates a new AnimatedSprite. */
        AnimatedSprite();
        virtual ~AnimatedSprite();

        /** Configures how to interprit a sprite sheet.
         * @param framesPerSecond The number of frames per second for all animations.
         * @param frameWidth The width of each frame (all frames must be of uniform width).
         * @param frameHeight The height of each frame (all frames must be of uniform height).
         * @param frameGap The number of pixels between each frame (both vertical and horizontal).
         * @param startOffsetX The x position of the first frame in the sprite sheet (usually, to accommodate  a banner).
         * @param startOffsetY The y position of the first frame in the sprite sheet (usually, to accommodate  a banner).
         */
        void setSpriteSheet(int framesPerSecond,
                            int frameWidth, int frameHeight, int frameGap,
                            int startOffsetX, int startOffsetY);

        /** Add a new animation from the sprite sheet.
         * @param id Assigns an id for future reference of the animation.
         * @param startFrameX The column number of the start frame (not the pixel position).
         * @param startFrameY The row number of the start frame (not the pixel position).
         * @param noOfFrames The number of sequential frames that make up the animation.
         */
        void addAnimation(int id, int startFrameX, int startFrameY, int noOfFrames);

        /** Updates the animation.
         * @param clock The clock to measure elapsed time since the last frame.
         */
        void update(Clock *clock);

        /** Stops the current animation (if any) and starts a new aniamtion from the beginning.
         * @param animationId The ID of the animation to play.
         * @return True if animationId references a set animation. False otherwise.
         */
        bool play(int animationId);

        /** Pauses the current animation (if one exists). */
        void pause();

        /** Resumes the current animation (if one exists). */
        void resume();

        /** Stops the current animation (if one exists). */
        void stop();

        /** Gets the height of the sprite sheet frames.
         * @param Frame height.
         */
        int getFrameHeight();

        /** Gets the width of the sprite sheet frames.
         * @param Frame width.
         */
        int getFrameWidth();

    private:

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
        float timeSinceLastFrame;
        bool paused;

};

/** Describes information about an animation. */
struct AnimationDetail
{
    int startFrameX; // Pixel position
    int startFrameY; // Pixel position
    int noOfFrames;
};

#endif // ANIMATEDSPRITE_H
