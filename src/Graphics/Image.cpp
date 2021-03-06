#include <Gosu/Image.hpp>
#include <Gosu/Bitmap.hpp>
#include <Gosu/Graphics.hpp>
#include <Gosu/ImageData.hpp>
#include <Gosu/Math.hpp>
#include <Gosu/IO.hpp>
#include <stdexcept>

Gosu::Image::Image(const std::wstring& filename, unsigned flags)
{
	// Forward.
	Bitmap bmp;
	loadImageFile(bmp, filename);
	Image(bmp, flags).data.swap(data);
}

Gosu::Image::Image(const std::wstring& filename,
    unsigned srcX, unsigned srcY, unsigned srcWidth, unsigned srcHeight,
    unsigned flags)
{
	// Forward.
	Bitmap bmp;
	loadImageFile(bmp, filename);
	Image(bmp, srcX, srcY, srcWidth, srcHeight, flags).data.swap(data);
}

Gosu::Image::Image(const Bitmap& source, unsigned flags)
{
	// Forward.
	Image(source, 0, 0, source.width(), source.height(), flags).data.swap(data);
}

Gosu::Image::Image(const Bitmap& source,
        unsigned srcX, unsigned srcY, unsigned srcWidth, unsigned srcHeight,
        unsigned flags)
:   data(Graphics::createImage(source, srcX, srcY, srcWidth, srcHeight, flags).release())
{
}

Gosu::Image::Image(GOSU_UNIQUE_PTR<ImageData> data)
:   data(data.release())
{
    if (this->data.get() == 0)
        throw std::invalid_argument("Gosu::Image cannot be initialized with an ImageData null pointer");
}

unsigned Gosu::Image::width() const
{
    return data->width();
}

unsigned Gosu::Image::height() const
{
    return data->height();
}

void Gosu::Image::draw(double x, double y, ZPos z,
    double factorX, double factorY,
    Color c,
    AlphaMode mode) const
{
    double x2 = x + width() * factorX;
    double y2 = y + height() * factorY;

    data->draw(x, y, c, x2, y, c, x, y2, c, x2, y2, c, z, mode);
}

void Gosu::Image::drawMod(double x, double y, ZPos z,
    double factorX, double factorY,
    Color c1, Color c2, Color c3, Color c4,
    AlphaMode mode) const
{
    double x2 = x + width() * factorX;
    double y2 = y + height() * factorY;

    data->draw(x, y, c1, x2, y, c2, x, y2, c3, x2, y2, c4, z, mode);
}

void Gosu::Image::drawRot(double x, double y, ZPos z,
    double angle, double centerX, double centerY,
    double factorX, double factorY,
    Color c,
    AlphaMode mode) const
{
    double sizeX = width()  * factorX;
    double sizeY = height() * factorY;
    double offsX = offsetX(angle, 1);
    double offsY = offsetY(angle, 1);

    // Offset to the centers of the original Image's edges when it is rotated
    // by <angle> degrees.
    double distToLeftX   = +offsY * sizeX * centerX;
    double distToLeftY   = -offsX * sizeX * centerX;
    double distToRightX  = -offsY * sizeX * (1 - centerX);
    double distToRightY  = +offsX * sizeX * (1 - centerX);
    double distToTopX    = +offsX * sizeY * centerY;
    double distToTopY    = +offsY * sizeY * centerY;
    double distToBottomX = -offsX * sizeY * (1 - centerY);
    double distToBottomY = -offsY * sizeY * (1 - centerY);

    data->draw(x + distToLeftX  + distToTopX,
               y + distToLeftY  + distToTopY, c,
               x + distToRightX + distToTopX,
               y + distToRightY + distToTopY, c,
               x + distToLeftX  + distToBottomX,
               y + distToLeftY  + distToBottomY, c,
               x + distToRightX + distToBottomX,
               y + distToRightY + distToBottomY,
               c, z, mode);
}

Gosu::ImageData& Gosu::Image::getData() const
{
    return *data;
}

std::vector<Gosu::Image> Gosu::loadTiles(const Bitmap& bmp,
    int tileWidth, int tileHeight, unsigned flags)
{
    int tilesX, tilesY;
    std::vector<Image> images;
    
    if (tileWidth > 0)
        tilesX = bmp.width() / tileWidth;
    else
    {
        tilesX = -tileWidth;
        tileWidth = bmp.width() / tilesX;
    }
    
    if (tileHeight > 0)
        tilesY = bmp.height() / tileHeight;
    else
    {
        tilesY = -tileHeight;
        tileHeight = bmp.height() / tilesY;
    }
    
    for (int y = 0; y < tilesY; ++y)
        for (int x = 0; x < tilesX; ++x)
            images.push_back(Image(bmp, x * tileWidth, y * tileHeight, tileWidth, tileHeight, flags));
    
    return images;
}

std::vector<Gosu::Image> Gosu::loadTiles(const std::wstring& filename,
    int tileWidth, int tileHeight, unsigned flags)
{
    Bitmap bmp;
    loadImageFile(bmp, filename);
    return loadTiles(bmp, tileWidth, tileHeight, flags);
}

// Deprecated constructors.

Gosu::Image::Image(Graphics& graphics, const std::wstring& filename, bool tileable)
{
    Image(filename, tileable).data.swap(data);
}

Gosu::Image::Image(Graphics& graphics, const std::wstring& filename,
                   unsigned srcX, unsigned srcY, unsigned srcWidth, unsigned srcHeight,
                   bool tileable)
{
    Image(filename, srcX, srcY, srcWidth, srcHeight, tileable).data.swap(data);
}

Gosu::Image::Image(Graphics& graphics, const Bitmap& source, bool tileable)
{
    Image(source, tileable).data.swap(data);
}

Gosu::Image::Image(Graphics& graphics, const Bitmap& source,
                   unsigned srcX, unsigned srcY, unsigned srcWidth, unsigned srcHeight,
                   bool tileable)
{
    Image(source, srcX, srcY, srcWidth, srcHeight, tileable).data.swap(data);
}

// Deprecated helpers.

std::vector<Gosu::Image> Gosu::loadTiles(Gosu::Graphics& graphics, const Bitmap& bmp,
    int tileWidth, int tileHeight, bool tileable)
{
    return Gosu::loadTiles(bmp, tileWidth, tileHeight, tileable);
}

std::vector<Gosu::Image> Gosu::loadTiles(Gosu::Graphics& graphics, const std::wstring& filename,
    int tileWidth, int tileHeight, bool tileable)
{
    return Gosu::loadTiles(filename, tileWidth, tileHeight, tileable);
}

