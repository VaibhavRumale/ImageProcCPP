#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Function to perform horizontal blur
void blur_horizontal(const std::vector<uint16_t>& input, std::vector<uint16_t>& output, int width, int height, int channels) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                int sum = input[(y * width + std::max(x - 1, 0)) * channels + c] +
                          2 * input[(y * width + x) * channels + c] +
                          input[(y * width + std::min(x + 1, width - 1)) * channels + c];
                output[(y * width + x) * channels + c] = sum / 4;
            }
        }
    }
}

// Function to perform vertical blur with clamping
void blur_vertical(const std::vector<uint16_t>& input, std::vector<uint16_t>& output, int width, int height, int channels) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                int sum = input[(std::max(y - 1, 0) * width + x) * channels + c] +
                          2 * input[(y * width + x) * channels + c] +
                          input[(std::min(y + 1, height - 1) * width + x) * channels + c];
                output[(y * width + x) * channels + c] = sum / 4;
            }
        }
    }
}


int main() {
    int width, height, channels;
    
    // Load the image
    unsigned char* input_image = stbi_load("images/rgb.png", &width, &height, &channels, 0);
    if (input_image == nullptr) {
        std::cerr << "Error in loading the image" << std::endl;
        return 1;
    }
    auto start = std::chrono::high_resolution_clock::now();

    // Convert to 16-bit to prevent overflow
    std::vector<uint16_t> input_16(width * height * channels);
    std::vector<uint16_t> temp_buffer(width * height * channels);
    std::vector<uint16_t> output_buffer(width * height * channels);
    for (int i = 0; i < width * height * channels; ++i) {
        input_16[i] = input_image[i];
    }

    // Perform horizontal blur
    blur_horizontal(input_16, temp_buffer, width, height, channels);

    // Perform vertical blur
    blur_vertical(temp_buffer, output_buffer, width, height, channels);

    // Convert back to 8-bit
    std::vector<uint8_t> output_image(width * height * channels);
    for (int i = 0; i < width * height * channels; ++i) {
        output_image[i] = static_cast<uint8_t>(output_buffer[i]);
    }

    // Save the output image
    stbi_write_png("blurry_parrot_cpp.png", width, height, channels, output_image.data(), width * channels);

    // Free the input image memory
    stbi_image_free(input_image);
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start;
    std::cout << "Image processing took " << duration.count() << " milliseconds." << std::endl;

    std::cout << "Image processed and saved successfully." << std::endl;
    return 0;
}

