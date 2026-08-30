package good.damn.sdk2.models.mesh.eye;

public final class SDKMMeshEyeTextureDescription {
    /// The texture with eye pixels.
    ///
    /// When using OpenGL ES 2.x and OpenGL ES 3.x, this field corresponds to a
    /// GLuint variable.
    ///
    /// When using Vulkan, this field corresponds to an uint64_t address pointing
    /// to a @c VkImage variable.The SDK client is expected to manage the
    /// object ownership and to guarantee the pointer validity during the
    /// @c ::CardboardDistortionRenderer_renderEyeToDisplay function execution
    /// to ensure it is properly retained. Usage example:
    ///
    /// @code{.cc}
    /// VkImage image;
    /// // Initialize and set up the image...
    /// CardboardEyeTextureDescription leftEye;
    /// leftEye.texture = reinterpret_cast<uint64_t>(image)
    /// // Fill remaining fields in leftEye...
    /// CardboardDistortionRenderer_renderEyeToDisplay(..., &leftEye, ...);
    /// // Clear previous image if it is needed.
    /// @endcode
    ///
    /// When using Metal, this field corresponds to a @c CFTypeRef
    /// variable pointing to a @c MTLTexture object. The SDK client is expected
    /// to manage the object ownership and to guarantee the pointer validity
    /// during the @c ::CardboardDistortionRenderer_renderEyeToDisplay function
    /// execution to ensure it is properly retained. Usage example:
    ///
    /// @code{.m}
    /// CardboardEyeTextureDescription leftEye;
    /// leftEye.texture = CFBridgingRetain(_texture);
    /// // Fill remaining fields in leftEye...
    /// CardboardDistortionRenderer_renderEyeToDisplay(..., &leftEye, ...);
    /// CFBridgingRelease(leftEye.texture);
    /// @endcode
    public long texture = 0;
    /// u coordinate of the left side of the eye.
    public float left_u = .0f;
    /// u coordinate of the right side of the eye.
    public float right_u = .0f;
    /// v coordinate of the top side of the eye.
    public float top_v = .0f;
    /// v coordinate of the bottom side of the eye.
    public float bottom_v = .0f;
}
