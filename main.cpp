#include <iostream>
#include "./api/RiotAPI.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

template<typename K, typename V>
void printHashMap(const std::unordered_map<K, V>& map) {
    std::cout << "{\n";
    for (const auto& [key, value] : map) {
        std::cout << "  " << key << " : " << value << "\n";
    }
    std::cout << "}\n";
}

int main() {
    APIClient* ApiClient = new APIClient();
    std::string username;
    std::string tagline;
    while (true) {
        std::cout << "Username : ";

        std::getline(std::cin, username);

        if (username == "q") {
            break;
        }


        std::cout << "Tagline : ";

        std::getline(std::cin, tagline);

        std::cout << "Username=" << username << " Tagline=" << tagline << "\n";

        ApiClient->get_user_puuid(username, tagline);
        ApiClient->fill_data_map();

        printHashMap(ApiClient->data_map);

    }
    delete ApiClient;
}