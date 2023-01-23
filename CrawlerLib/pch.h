#ifndef PCH_HH
#define PCH_HH


//#include "framework.h"

#include <iostream>	
#include <fstream>
#include <exception>
#include <vector>
#include <list>


#include <curl/curl.h>      //vcpkg integrated  //also here https://github.com/curl/curl.git          

//included in headers
#include <gumbo-query/gumbo.h>  // included
#include <nlohmann/json.hpp>	//#include
#include "Image.h"  //#include <nlohmann/json.hpp>    //included    //#include <string>     //#include <unordered_map>
#include <mutex>
#include <set>
#include <gumbo-query/gumbo.h>  // included
//

using std::string;


#include "AddressCombiner.h"
#include "HtmlRequest.h"


#endif //PCH_HH