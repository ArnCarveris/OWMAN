
template<typename Archive>
void GraphicsSystem::load(Archive& ar) 
{
    std::string windowTitle;
    unsigned int width;
    unsigned int height;
    bool fullScreen;

    ar(cereal::make_nvp("window_title", windowTitle));
    ar(cereal::make_nvp("fullscreen", fullScreen));
    ar(cereal::make_nvp("x_resolution", width));
    ar(cereal::make_nvp("y_resolution", height));

    service::renderer::set
    (
        windowTitle,
        width, height,
        fullScreen
    );

    camera.setWidth(width / 2);
    camera.setHeight(height / 2);
}

template<typename Archive>
void GraphicsSystem::save(Archive&) const { }