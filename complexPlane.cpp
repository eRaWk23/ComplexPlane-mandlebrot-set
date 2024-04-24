#include "complexPlane.h"
#include <cmath>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixel_size = {pixelWidth, pixelHeight};
    m_aspectRatio = static_cast<float>(pixelHeight)/pixelWidth;
    m_plane_center = {0,0};
    m_plane_size = {BASE_WIDTH,BASE_HEIGHT * m_aspectRatio};
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    // Init VertexArray
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}
        
void ComplexPlane::updateRender()
{
    if(m_state == State::CALCULATING)
    {
        for (int i = 0; i < m_pixel_size.y; i++)
        {
            for (int j = 0; j < m_pixel_size.x; j++)
            {
                m_vArray[j + i *m_pixel_size.x].position = Vector2f(j, i);
                Vector2f coord = mapPixelToCoords(sf::Vector2i(j,i));
                size_t iterations = countIterations(coord);
                Uint8 r,g,b;
                iterationToRGB(iterations,r, g, b);
            }
        }
    }
}

void ComplexPlane::zoomIn()
{
    ++m_zoomCount;

    float zoomFactor = pow(BASE_ZOOM, m_zoomCount);
    float newWidth = BASE_WIDTH * zoomFactor;
    float newHeight = BASE_HEIGHT * m_aspectRatio * zoomFactor;
    m_plane_size = {newWidth, newHeight};

    m_state = State::CALCULATING;

}

void ComplexPlane::zoomOut()
{
    --m_zoomCount;
    float zoomFactor = pow(BASE_ZOOM, m_zoomCount);
    float newWidth = BASE_WIDTH * zoomFactor;
    float newHeight = BASE_HEIGHT * m_aspectRatio * zoomFactor;
    m_plane_size = {newWidth, newHeight};

    m_state = State::CALCULATING;

}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
    Vector2f coords = mapPixelToCoords(mousePixel);
    m_plane_center = coords;
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    Vector2f coords = mapPixelToCoords(mousePixel);
    m_mouseLocation.x = static_cast<int>(coords.x);
    m_mouseLocation.y = static_cast<int>(coords.y);
}

void ComplexPlane::loadText(Text& Text)
{

}

int ComplexPlane::countIterations(Vector2f coord)
{
    
}

Vector2f mapPixelToCoords(Vector2i mousePixel);

void ComplexPlane::iterationToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count == MAX_ITER)
    {
        r = g = b = 0;
    }
    else
    {
        size_t region = count * 5 / MAX_ITER;
        size_t remaining = count * 5 % MAX_ITER;

        Color color[] = 
        { 
            Color(128, 0 , 128),
            Color(0, 128, 128),
            Color(0,128, 0),
            Color(255, 255, 0),
            Color(255, 0 ,0 )
        };

        Color colorOne = color[region];
        Color colorTwo = color[region + 1];
        r = colorOne.r + (colorTwo.r - colorOne.r) * remaining / MAX_ITER;
        g = colorOne.g + (colorTwo.g - colorOne.g) * remaining / MAX_ITER;
        b = colorOne.b + (colorTwo.b - colorOne.b) * remaining / MAX_ITER;
    }
}
