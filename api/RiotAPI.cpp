#include "RiotAPI.hpp"

using json = nlohmann::json;


size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = nmemb * size;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), total_size);
    return total_size;
}



APIClient::APIClient(): m_curl {curl_easy_init()} {
    if (!m_curl) {
        std::cerr << "Error during curl initialization\n";
        exit(2);
    }
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_response);

    data_map["killParticipation"] = 0;
    data_map["damagePerMinute"] = 0;
    data_map["teamDamagePercentage"] = 0;
    data_map["minionsKilled"] = 0;
    data_map["totalGold"] = 0;
}

APIClient::~APIClient() {
    curl_easy_cleanup(m_curl);
}


//implement a caching system 

std::string APIClient::get_user_puuid(const std::string& username, const std::string& tagline) {


    char* escaped = curl_easy_escape(m_curl, username.c_str(), username.length());
    std::string url = "https://europe.api.riotgames.com/riot/account/v1/accounts/by-riot-id/";
    url += escaped;
    url += "/" + tagline;
    url += "?api_key=RGAPI-cef733e4-2f0a-4058-8df6-54fdbf1d4884";


    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());


    CURLcode res = curl_easy_perform(m_curl);

    if (res != CURLE_OK) {
        std::cout << "Error during the request : " <<  curl_easy_strerror(res) << "\n";
        exit(1);
    }


    json response_json = json::parse(m_response);

    std::string puuid = response_json["puuid"];                                                     

    m_puuid = puuid;

    m_response.clear();

    return puuid;

}

json APIClient::get_matches_id(const std::string& puuid, int count) {
    std::string url = "https://europe.api.riotgames.com/lol/match/v5/matches/by-puuid/" + puuid + "/ids";
    url += "?start=0&count=" + std::to_string(count);
    url += "&api_key=RGAPI-cef733e4-2f0a-4058-8df6-54fdbf1d4884";

    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());

    CURLcode res = curl_easy_perform(m_curl);

    if (res != CURLE_OK) {
        std::cout << "Error during the request : " <<  curl_easy_strerror(res) << "\n";
        exit(1);
    }

    json response_json = json::parse(m_response);

    m_response.clear();

    return response_json;
}   

json APIClient::get_match_by_id(const std::string& id) {
    std::ofstream file_stream("../match.json");
  
    std::string url = "https://europe.api.riotgames.com/lol/match/v5/matches/" + id;
    url += "?api_key=RGAPI-cef733e4-2f0a-4058-8df6-54fdbf1d4884";

    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());

    CURLcode res = curl_easy_perform(m_curl);

    if (res != CURLE_OK) {
        std::cout << "Error during the request : " <<  curl_easy_strerror(res) << "\n";
        exit(1);
    }

    file_stream << m_response;

    file_stream.flush();

    file_stream.close();

    json response_json = json::parse(m_response);

    m_response.clear();
    return response_json;

}


void APIClient::fill_data_map() {
    json matches_id = this->get_matches_id(m_puuid, 25);
    std::cout << m_puuid << "\n";
    int i = 0;
    for (auto& match_id: matches_id) {
        json match = get_match_by_id(match_id);
        if (match["info"]["queueId"].get<int>() != 420) { //420 = ranked solo duo
            std::cout << "Match Id=" << match_id << "\n"; 
            std::cout << "xd\n";
            continue;
        }
        std::cout << "Match Id=" << match_id << "\n"; 
        for (auto& participant: match["info"]["participants"]) {  // participants list
            if (participant["puuid"] != m_puuid) {
                continue;
            }
            std::cout << "Sent1\n"; 
            if (!participant["challenges"].contains("killParticipation")) {
                continue;
            }
            if (!participant["challenges"].contains("teamDamagePercentage")) {
                continue;
            }
            if (!participant["challenges"].contains("damagePerMinute")) {
                continue;
            }
            data_map["killParticipation"] += participant["challenges"]["killParticipation"].get<double>();
            data_map["teamDamagePercentage"] += participant["challenges"]["teamDamagePercentage"].get<double>();
            data_map["damagePerMinute"] += participant["challenges"]["damagePerMinute"].get<double>();
            std::cout << "Sent4\n"; 
            data_map["minionsKilled"] += 0;
            std::cout << "Sent5\n"; 
            data_map["totalGold"] += 0;
            i++;
        }
    }
    std::cout << "i=" << i << "\n";
}
