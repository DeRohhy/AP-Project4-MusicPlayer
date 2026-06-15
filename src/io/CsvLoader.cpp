#include "io/CsvLoader.h"
#include <fstream>
#include <sstream>
#include "model/Song.h"


std::vector<std::unique_ptr<Song>> CsvLoader::loadAllMusic(const std::string& library_path)
{
   std::vector<std::unique_ptr<Song>> songs;

    std::ifstream library(library_path);

    if (!library)
        return songs;
    
    std::string line;
    while(std::getline(library, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string field;

        std::unique_ptr<Song> song = std::make_unique<Song>();

        std::getline(ss, song->title, ',');
        std::getline(ss, song->artist, ',');
        std::getline(ss, song->album, ',');
        std::getline(ss, song->genre, ',');
        
        std::getline(ss, field, ','); 
        try { song->year = std::stoi(field); }
        catch(...) { song->year = 0; }

        std::getline(ss, field, ','); 
        try { song->duration_sec = std::stoi(field); }
        catch(...) { song->duration_sec = 0; }

        std::getline(ss, song->file_path);

        songs.push_back(std::move(song));
    }

    return songs;
}