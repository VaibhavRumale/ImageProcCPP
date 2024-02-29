#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void brightenImage(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; ++i) {
        int value = static_cast<int>(input[i]) * 1.5;
        output[i] = static_cast<uint8_t>(std::min(value, 255));
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

    // Create output image buffer
    std::vector<uint8_t> output_image(width * height * channels);
    auto start = std::chrono::high_resolution_clock::now();

    // Brighten the image
    brightenImage(std::vector<uint8_t>(input_image, input_image + width * height * channels), output_image, width, height, channels);
    
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start;
    std::cout << "Image processing took " << duration.count() << " milliseconds." << std::endl;


    // Save the output image
    stbi_write_png("images/brighter.png", width, height, channels, output_image.data(), width * channels);

    // Free the input image memory
    stbi_image_free(input_image);

    std::cout << "Image processed and saved successfully." << std::endl;
    return 0;
}
