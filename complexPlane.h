#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

using namespace sf;
using namespace std;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum class State 
{
    CALCULATING,
    DISPLAYING
};

class ComplexPlane : public sf::Drawable
{
    public:
        ComplexPlane(int pixelWidth, int pixelHeight);
        void draw(RenderTarget& target, RenderStates states) const
        {
            target.draw(m_vArray);
        }
        void updateRender();
        void zoomIn();
        void zoomOut();
        void setCenter(Vector2i mousePixel);
        void setMouseLocation(Vector2i mousePixel);
        void loadText(Text& Text);
    private:
        size_t countIterations(Vector2f coord);
        Vector2f mapPixelToCoords(Vector2i mousePixel);
        void iterationToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
        VertexArray m_vArray;
        State m_state;
        Vector2i m_mouseLocation;
        Vector2i m_pixel_size;
        Vector2f m_plane_center;
        Vector2f m_plane_size;
        int m_zoomCount;
        float m_aspectRatio;
};     

#endif 
