#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <cstdint>
#include <string>

class Shader;
//Class texture: It gets a texture from a path to be used. It contains different options (wrap, filter)
class Texture {
    public:
        enum class Wrap {
            Repeat,
            Mirrored_Repeat,
            Clamp_To_Border,
            Clamp_To_Edge
        };

        enum class Filter {
            None,
            Nearest,
            Linear
        };

        enum class Format {
            RGB,
            RGBA
        };
		//Constructor
        Texture(const char* path, Format format);
        Texture() = delete;
        ~Texture();

        Texture(const Texture&) = default;
        Texture(Texture&&) = default;
        Texture& operator=(const Texture&) = default;
        Texture& operator=(Texture&&) = default;
		//use: Use the texture in a given shader
        void use(const Shader& shader, const char* name, uint32_t unit = 0) const;
		//setWrap
        void setWrap(Wrap s, Wrap t);
		//setFilter
        void setFilter(Filter mag, Filter min, Filter mipMapMag = Filter::None, Filter mipMapMin = Filter::None);

    private:
		//applyWrapping
        void applyWrapping() const;
		//applyFilter
        void applyFilter() const;

    private:
        uint32_t _id = 0;
        Format _format = Format::RGB;
        int32_t _width = 0, _height = 0;
        int32_t _depth = 0;
        std::pair<Wrap, Wrap>_wrap{ Wrap::Repeat, Wrap::Repeat };
        std::pair<Filter, Filter>_filter{ Filter::Linear, Filter::Linear };
        std::pair<Filter, Filter>_filterMipMap{ Filter::None, Filter::None };
};

#endif