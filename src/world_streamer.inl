

template<typename Archive>
void WorldStreamer::load(Archive& ar)
{
    std::string worldFile;
    int windowSize;

    ar(cereal::make_nvp("world_folder", worldFolder));
    ar(cereal::make_nvp("world_file", worldFile));
    ar(cereal::make_nvp("cell_size", cellSize));
    ar(cereal::make_nvp("window_size", windowSize));

    worldWindow.setWindowSize(windowSize);

    loadAvailableCells(worldFile.c_str());
}

template<typename Archive>
void WorldStreamer::save(Archive&) const { }

