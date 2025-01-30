//tamil string to bmp image
//g++ -o free_program tailstringorr.cpp -lfreetype -lharfbuzz -I /usr/include/freetype2 EasyBMP.cpp -I./easybmp


#include <iostream>
#include "EasyBMP.h"
#include <ft2build.h>
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <string.h>
#include <locale.h>

void render_text_to_bmp(const char *text, const char *font_path, int width, int height, int count, int x, int y, int FONT_SIZE) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not initialize FreeType library!" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face)) {
        std::cerr << "Could not load font!" << std::endl;
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);
    hb_font_t *hb_font = hb_ft_font_create(face, NULL);
    hb_buffer_t *hb_buffer = hb_buffer_create();

    BMP bmp;
    bmp.SetSize(width, height);
    bmp.SetBitDepth(24);

    // Fill the background with white
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bmp(x, y)->Red = 255;
            bmp(x, y)->Green = 255;
            bmp(x, y)->Blue = 255;
        }
    }

    hb_buffer_add_utf8(hb_buffer, text, -1, 0, -1);
    hb_buffer_guess_segment_properties(hb_buffer);
    hb_shape(hb_font, hb_buffer, NULL, 0);

    unsigned int glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(hb_buffer, &glyph_count);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(hb_buffer, &glyph_count);

    int pen_x = x;
    int pen_y = y + FONT_SIZE;

    for (unsigned int i = 0; i < glyph_count; i++) {
        FT_Load_Glyph(face, glyph_info[i].codepoint, FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        FT_Bitmap *bitmap = &face->glyph->bitmap;

        for (int row = 0; row < bitmap->rows; row++) {
            for (int col = 0; col < bitmap->width; col++) {
                int pixel_x = pen_x + face->glyph->bitmap_left + col;
                int pixel_y = pen_y - face->glyph->bitmap_top + row;

                if (pixel_x >= 0 && pixel_x < width && pixel_y >= 0 && pixel_y < height) {
                    unsigned char value = bitmap->buffer[row * bitmap->pitch + col];
                    if (value > 0) {
                        bmp(pixel_x, pixel_y)->Red = 0;
                        bmp(pixel_x, pixel_y)->Green = 0;
                        bmp(pixel_x, pixel_y)->Blue = 0;
                    }
                }
            }
        }

        pen_x += glyph_pos[i].x_advance / 64;
        pen_y += glyph_pos[i].y_advance / 64;
    }

    char filename[250];
    sprintf(filename, "%d.bmp", count);
    bmp.WriteToFile(filename);

    hb_buffer_destroy(hb_buffer);
    hb_font_destroy(hb_font);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

int main() {
    setlocale(LC_ALL, ""); // Set locale to handle UTF-8 encoding

    const char *text = "கிளம்பகம் கிளம்பகம்";
    const char *font_path = "/home/jeff/Downloads/check/FREETYPE/TiroTamil-Regular.ttf";
    int width = 600;
    int height = 200;
    int FONT_SIZE = 24;

    render_text_to_bmp(text, font_path, width, height, 0, 100, 100, FONT_SIZE);

    return 0;
}
