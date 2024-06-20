#include "complexPlane.h"
#include <cmath>
#include <sstream>

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
                m_vArray[j + i * m_pixel_size.x].color = {r, g, b};
            }
        }
        m_state = State::DISPLAYING;
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
    Vector2i currLocation = Mouse::getPosition();
    ostringstream texter;
    texter << "Mandlebrot Set" << endl 
    << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")" << endl 
    << "Cursor: (" << currLocation.x << ", " << currLocation.y << ")" << endl 
    << "Left click to zoom in" << endl 
    << "Right click to zoom out" << endl;
    Text.setString(texter.str());
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
   double real = coord.x;
   double imga = coord.y;
   
   double x = 0.0;
   double y = 0.0;

   size_t iterations = 0;
   double threshold = 4.0;
   
   while(x  * x + y * y < threshold && iterations < MAX_ITER)
   {
        double xTemp = x * x - y * y + real;
        y = 2.0 * x *y + imga;
        x = xTemp;
        iterations ++;
   }
   return iterations;
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    double pixelW = static_cast<double>(m_pixel_size.x);
    double pixelH = static_cast<double>(m_pixel_size.y);

    double minX = m_plane_center.x - m_plane_size.x / 2.0;
    double maxX = m_plane_center.x + m_plane_size.x / 2.0;
    double minY = m_plane_center.y - m_plane_size.y / 2.0;
    double maxY = m_plane_center.y + m_plane_size.y / 2.0;

    double x = ((static_cast<double>(mousePixel.x)/ pixelW) *(maxX - minX)) + minX;
    double y = ((1.0 - (static_cast<double>(mousePixel.y) / pixelH)) * (maxY - minY)) + minY;

    return Vector2f(x, y);
}

void ComplexPlane::iterationToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    /*
    if (count == MAX_ITER)
    {
        r = 0;
        g = 0;
        b = 0;
    }
    else if (count <= 12 && count > 0)
    {
        r = 128;
        g = 0;
        b = 128;
    }
    else if (count <= 24)
    {
        r = 0;
        g = 128;
        b = 128;
    }
    else if (count <= 36)
    {
        r = 0;
        g = 128;
        b = 0;
    }
    else if (count <= 48)
    {
        r = 255;
        g = 255;
        b = 0;
    }
    else if (count < MAX_ITER)
    {
        r = 255;
        g = 0;
        b = 0;
    }*/
    
    
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
