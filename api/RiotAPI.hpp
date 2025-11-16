#pragma once

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>


using json = nlohmann::json;

class APIClient {
    public:
        APIClient();
        ~APIClient();
        std::string get_user_puuid(const std::string& username, const std::string& tagline);
        json get_matches_id(const std::string& puuid, int count);
        json get_match_by_id(const std::string& id);
        void fill_data_map();
    private:
        CURL* m_curl;
        std::string m_response;
        std::string m_puuid;
    public: 
        std::unordered_map<std::string, double> data_map; 
        
};