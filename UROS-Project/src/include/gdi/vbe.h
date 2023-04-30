#include "../include/io/typedefs.h"

typedef struct
{
    // Mandatory info for all VBE revisions
    uint16_t mode_attributes;
    uint8_t window_a_attributes;
    uint8_t window_b_attributes;
    uint16_t window_granularity;
    uint16_t window_size;
    uint16_t window_a_segment;
    uint16_t window_b_segment;
    uint32_t window_function_pointer;
    uint16_t uint8_ts_per_scanline;

    // Mandatory info for VBE 1.2 and above
    uint16_t x_resolution;
    uint16_t y_resolution;
    uint8_t x_charsize;
    uint8_t y_charsize;
    uint8_t number_of_planes;
    uint8_t bits_per_pixel;
    uint8_t number_of_banks;
    uint8_t memory_model;
    uint8_t bank_size;
    uint8_t number_of_image_pages;
    uint8_t reserved1;

    // Direct color fields (required for direct/6 and YUV/7 memory models)
    uint8_t red_mask_size;
    uint8_t red_field_position;
    uint8_t green_mask_size;
    uint8_t green_field_position;
    uint8_t blue_mask_size;
    uint8_t blue_field_position;
    uint8_t reserved_mask_size;
    uint8_t reserved_field_position;
    uint8_t direct_color_mode_info;

    // Mandatory info for VBE 2.0 and above
    uint32_t physical_base_pointer; // Physical address for flat memory frame buffer (where we write the pixels)
    uint32_t reserved2;
    uint16_t reserved3;

    // Mandatory info for VBE 3.0 and above
    uint16_t linear_uint8_ts_per_scanline;
    uint8_t bank_number_of_image_pages;
    uint8_t linear_number_of_image_pages;
    uint8_t linear_red_mask_size;
    uint8_t linear_red_field_position;
    uint8_t linear_green_mask_size;
    uint8_t linear_green_field_position;
    uint8_t linear_blue_mask_size;
    uint8_t linear_blue_field_position;
    uint8_t linear_reserved_mask_size;
    uint8_t linear_reserved_field_position;
    uint32_t max_pixel_clock;

    uint8_t reserved4[190]; // Remainder of mode info block

} __attribute__((packed)) vbe_mode_info_t;

static vbe_mode_info_t * gfx_mode = (vbe_mode_info_t *)0x8400;