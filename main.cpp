#include <iostream>
#include "compare.hpp"


int main(int argc, const char* argv[]) 
{
    if (argc!=2) //if argc!=2, argv includes more or less than one command line argument
    {
        std::cout<<"To use this program correctly, one should type..."<<std::endl;
        std::cout<<"./hw1 [directory containing directories each of which contains text files]"<<std::endl;
    }
    else //input has right amount of command line arguments
    {
        std::string arg_string(argv[1]); //turns command line arg into a string
        fs::path input_path(argv[1]); //turns command line arg into path
        fs::directory_entry path_entry(input_path);

        if (path_entry.exists() && path_entry.is_directory())//checks if input is a directory and is in same directory as the executable hw1
        {
            std::vector<fs::path> paths = get_paths(arg_string); //gets paths of all files that need to be compared
            std::vector<CompareFiles> pairs = get_pairs(paths); //makes pairs for comparison

            for (size_t i=0;i<pairs.size();++i) //shows stats for all pairs
            {
                pairs[i].show_stats(); 
            }
            
        }
        else if (!path_entry.exists())
        {
            std::cout<<"Path doesn't exist!"<<std::endl;
        }
        else if (path_entry.is_regular_file())
        {
            std::cout<<"This is a regular file! Please input a directory"<<std::endl;
        }
    }

    return 0;
}