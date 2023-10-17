#include "compare.hpp"

std::vector<fs::path> get_paths(const std::string& directory)
{
    fs::directory_iterator it(directory); //iterator 
    std::vector<fs::path> out_vec; //vector of paths that will be returned

    for (fs::directory_entry entry : it) 
    {
        if (entry.is_regular_file())
        {
            int length_string=entry.path().string().length(); //length of the path of string
            std::string end_string(entry.path().string().substr(length_string-10,10)); //last 10 characters of path string
           
            if (length_string<10)// if length of string is less than 10, not possible for it to end with /.DS_Store
            {
                out_vec.push_back(entry.path()); //valid path
            }
            else if (length_string>=10 && end_string!="/.DS_Store")//if string length >10, need to check that dosn't end in .DS_Store
            {
                out_vec.push_back(entry.path()); //valid path
            }
        }
        else if (entry.is_directory())
        {
            for (fs::path recurse_path: get_paths(entry.path().string()))
            //uses recursion to get all paths within directory
            //if the entry is a directory, call function again to get all files from within this directory, then push_back to out_vec
            {
                out_vec.push_back(recurse_path);
            }

        }
    }

    return out_vec;
}

CompareFiles::CompareFiles(const fs::path& path1, const fs::path& path2)
{
    //set member functions
    p1=path1;
    p2=path2;
    std::string common();

    compare(); //updates the common string
}

void CompareFiles::show_stats() const
{
    // uses member variable info to show stats
    std::cout << "Pair: " << p1.string() << ", " << p2.string()<<std::endl;
    std::cout << "Common subsequence length: "<<common.size()<<std::endl;
    std::cout<<"Overlap: "<<common<<std::endl;
    std::cout<<std::endl;
    
    
}

std::vector<CompareFiles> get_pairs(const std::vector<fs::path>& path_vec)
{
    std::vector<CompareFiles> pairs_vec;

    for (size_t i=0;i<path_vec.size();++i)
    {
        for (size_t j=i+1; j<path_vec.size();++j)
        {
           pairs_vec.push_back(CompareFiles(path_vec[i],path_vec[j])); //creates all possible pairs and pushes back a compare file object to vector
        }
    }
    return pairs_vec;
} 

void CompareFiles::compare() 
{
    //Getting file 1 info ready
    std::ifstream in_p1(p1,std::ios::ate);  //reads in first file's contents
    auto length_1= in_p1.tellg();           //retuns length of first file
    std::string str_1(length_1, '\0');      //creates null string with length of first file
    in_p1.seekg(0);                         //points to begining
    in_p1.read(&str_1[0],length_1);         //reads file data into string
    std::string p1_str('\0'+str_1);         //adds null char to beginnig og string
    in_p1.close();                          //closes file

    //Getting file 2 info ready
    std::ifstream in_p2(p2,std::ios::ate);  //reads in second file's contents
    auto length_2= in_p2.tellg();           //length of second file
    std::string str_2(length_2, '\0');      //null string with length of second file
    in_p2.seekg(0);                         //points to beginning
    in_p2.read(&str_2[0],length_2);         //reads file data into string
    std::string p2_str('\0'+str_2);         //adds null char to beginning
    in_p2.close();                          //closes file

    
    std::string* horiz_str; //string that goes along top of matrix
    std::string* vert_str; //string that goes along side of matrix
    
    if (p1_str.size()<p2_str.size()) 
    //sets a preference for longest string to go along the side of matrix
    {
        horiz_str = &p1_str;
        vert_str = &p2_str;
    }
    else
    {
        horiz_str = &p2_str;
        vert_str = &p1_str;
    }

    //step 1: matches matrix
    std::vector<std::vector<bool>> matches; //bool valued matrix
    matches.push_back(std::vector<bool>(horiz_str->size(),0)); //first row only has zeros
    for (size_t i=1; i<vert_str->size();++i)
    {
        std::vector<bool> temp;
        
        for (size_t j=0; j<horiz_str->size();++j)
        {
            if (j==0)
            {
                temp.push_back(0); //ensures that first column only has zeros
            }
            else
            {
                if (horiz_str->at(j)== vert_str->at(i)) //same character in both strings at positions i and j
                {
                    temp.push_back(1); 
                }
                else //different characters 
                {
                    temp.push_back(0);
                }
            }

        }
        matches.push_back(temp);
    }

    //step 2: count matrix
    std::vector<std::vector<int>> count(vert_str->size(), std::vector<int>(horiz_str->size(),0)); //creates appropriately sized matrix
    for (size_t i=1; i<vert_str->size();++i)
    { 
        for (size_t j=1; j<horiz_str->size();++j)
        {
            if (matches.at(i).at(j)==0) //matches[i][j]==0
            {
                //takes max of left and upper and use it for count[i][j] value
                if (count.at(i).at(j-1)>count.at(i-1).at(j)) 
                {
                    count.at(i).at(j)=count.at(i).at(j-1);
                }
                else
                {
                    count.at(i).at(j)=count.at(i-1).at(j);
                }
            }
            else //matches[i][j]==1
            {
                count.at(i).at(j)=1+count.at(i-1).at(j-1); //add one to upper left diagonal to get value of count[i][j]
            }

        }
    }

    //Step 3:backtracking
    size_t i=vert_str->size()-1; //last row index
    size_t j=horiz_str->size()-1; //last column index
    while(count.at(i).at(j)>0)
    {
        if (matches.at(i).at(j)==0) 
        {
            //move in way as to not change value
            if (count.at(i).at(j-1)==count.at(i).at(j))
            //establishes a preference for left
            {
                j-=1;
            }
            else
            {
                i-=1;
            }
        }
        else
        {
            common=vert_str->at(i)+common; //updates common string
            i-=1;
            j-=1;
        }
    }
}

