#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <SFML/Graphics.hpp>
using namespace sf;

#include <string>
#include <map>
using namespace std;

/** A class for managing the creation and destruction of external resources.
    If a resource is requested that has not been loaded before, the new resource will be loaded and sent.
    If a resource is requested which has been loaded before, an ALIASED (duplicate) pointer will be sent.

    All resources loaded will be destroyed when the destructor is called.
*/
class ContentManager
{
    public:
        ContentManager();
        virtual ~ContentManager();

        /** Loads or retrieves an external resource.
            NULL is returned if the resource could not be loaded.
        */
        Image* loadImage(string path);

    private:
        map<string, Image*> *imageMap;

};

#endif // CONTENTMANAGER_H
