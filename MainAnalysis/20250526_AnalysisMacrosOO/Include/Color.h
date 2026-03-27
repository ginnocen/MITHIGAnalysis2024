#include <TColor.h>
#include <vector>

// Converts HSV to RGB (0–1 range for TColor)
void HSVtoRGB(float h, float s, float v, float& r, float& g, float& b) {
    int i = int(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    i = i % 6;

    switch (i) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
}

// Returns color indices for rainbow-colored TColors
std::vector<int> getRainbow(int n, int baseColorIndex = 3000) {
    std::vector<int> colorIndices;
    for (int i = 0; i < n; ++i) {
        float hue = static_cast<float>(i) / n;
        float saturation = 0.6f; // Fixed saturation
        float value = 0.9f; // Fixed value
        float r, g, b;
        HSVtoRGB(hue, saturation, value, r, g, b);
        cout << "Color " << i << ": (" << r << ", " << g << ", " << b << ")" << endl;
        int colorIndex = baseColorIndex + i;
        new TColor(colorIndex, r, g, b);  // RGB values in [0,1]
        colorIndices.push_back(colorIndex);
    }
    return colorIndices;
}

std::vector<int> getRainbow2(int n, int baseColorIndex = 3000) {
    std::vector<int> colorIndices;

    // Limit hue to [0.0, 0.83] to stop at dark purple
    float hueStart = 0.0f;
    float hueEnd = 0.7f;

    for (int i = 0; i < n; ++i) {
        float hue = hueStart + (hueEnd - hueStart) * (static_cast<float>(i) / (n - 1));
        float saturation = 0.7f;
        float value = 0.85f;
        float r, g, b;
        HSVtoRGB(hue, saturation, value, r, g, b);
        int colorIndex = baseColorIndex + i;
        new TColor(colorIndex, r, g, b);
        colorIndices.push_back(colorIndex);
    }

    return colorIndices;
}
