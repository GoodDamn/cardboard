//
// Created by gooddamn on 6/14/26.
//

#ifndef CARDBOARD_SDK_INCLUDE_VIEWPORT_PARAMS_H
#define CARDBOARD_SDK_INCLUDE_VIEWPORT_PARAMS_H

namespace cardboard {
    // All values in tanangle units.
    struct ViewportParams {
        float width;
        float height;
        float x_eye_offset;
        float y_eye_offset;
    };
}

#endif //CARDBOARD_SDK_INCLUDE_VIEWPORT_PARAMS_H
