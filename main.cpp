#include "VulkanApp.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
    VulkanApp app(WIDTH, HEIGHT);
    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
