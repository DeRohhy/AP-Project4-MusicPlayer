#pragma once

#include <vector>
#include <string>
#include <memory>
#include "model/Song.h"
#include "io/CsvLoader.h"

const std::string LIBRARY = "data/library.csv";

class MusicLibrary
{
public:
    MusicLibrary() : songs{ CsvLoader::loadAllMusic(LIBRARY) } {}
private:
    std::vector<std::unique_ptr<Song>> songs;
};