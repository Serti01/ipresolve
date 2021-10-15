#include "main.hpp"

using nlohmann::json;

struct memory {
    char *r;
    size_t s;
};

size_t cb(void *buf, size_t size, size_t nmemb, void *userp){
    size_t realsize = size*nmemb;
    struct memory *mem = (struct memory *)userp;

    char *ptr = (char *)realloc(mem->r, mem->s + realsize + 1);
    if (ptr == NULL) 
        return 0lu;

    mem->r = ptr;
    memcpy(&(mem->r[mem->s]), buf, realsize);
    mem->s += realsize;
    mem->r[mem->s] = 0;

    return realsize;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s <IP> [IP...]\nie: %s 1.1.1.1\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    std::cout << "IP Resolve Results" << std::endl;

    char buffer[2048] = {0};
    curl_global_init(0);
    CURL *hnd = curl_easy_init();

    for (int i = 1; i < argc; i++) {
        sprintf(buffer, "http://ip-api.com/json/%s?fields=18601977", argv[i]);
        curl_easy_setopt(hnd, CURLOPT_URL, buffer);

        memset(buffer,0,2048);
        
        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, cb);

        struct memory chunk = {0};
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)&chunk);

        int success = curl_easy_perform(hnd);

        json res = json::parse(chunk.r);

        if (res["status"] == "fail") {
            std::cout << "Failed to resolve " << argv[i] << ": " << res["message"] << std::endl << std::endl;
            continue;
        }
        
        std::string continent   = res["continent"];
        std::string country     = res["country"];
        std::string region      = res["regionName"];
        std::string city        = res["city"];
        std::string district    = res["district"];
        std::string zip         = res["zip"];
        std::string isp         = res["isp"];
        std::string org         = res["org"];
        std::string hostn       = res["reverse"];
        std::string tz          = res["timezone"];
        float       lat         = res["lat"];
        float       lon         = res["lon"];
        bool        hosting     = res["hosting"];
        bool        mobile      = res["mobile"];
        bool        proxy       = res["proxy"];

#define clear       "\x1b[0m"
#define underline   "\x1b[4m"
#define stringc     "\x1b[38;2;255;183;59m"
#define intc        "\x1b[38;2;33;166;255m"
#define falsec      "\x1b[38;2;255;104;59m"
#define truec       "\x1b[38;2;128;255;102m"

#define ts(v,n)\
    if (!v.empty())\
        std::cout << "\t" << n << "\t" << stringc << v << clear << std::endl
#define tf(v,n)\
    if (v != 0)\
        std::cout << "\t" << n << "\t" << intc << v << clear << std::endl
#define tb(v,n)\
    std::cout << "\t" << n << "\t" << (v ? truec : falsec) << (v ? "True" : "False") << clear << std::endl
        
        std::cout << argv[i] << std::endl;
        ts(continent,   "Continent");
        ts(country,     "Country\t");
        ts(region,      "Region\t");
        ts(city,        "City\t");
        ts(district,    "District");
    tf(atoi(zip.c_str()),"Zip\t");
        ts(isp,         "ISP\t");
        ts(org,         "ORG\t");
        ts(hostn,       "Hostname");
        ts(tz,          "Timezone");
        tf(lat,         "Latitude");
        tf(lon,         "Longitude");
        tb(hosting,     "Corporate");
        tb(mobile,      "Cellular");
        tb(proxy,       "Proxy/TOR");
        std::cout << std::endl;
    }

    curl_easy_cleanup(hnd);

    // int sock = 0, valread;
    // struct sockaddr_in server;
    // if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    //     perror("Couldn't create socket");
    //     return EXIT_FAILURE;
    // }

    // server.sin_family = AF_INET;
    // server.sin_port = htons(443);

    // struct hostent *hoste = gethostbyname("ip-api.com");

    // char *host = strdup(inet_ntoa(
    //         *((struct in_addr*)hoste->h_addr_list[0])));

    // if (inet_pton(AF_INET, host, &server.sin_addr) <=0) {
    //     perror("Invalid address");
    //     return EXIT_FAILURE;
    // }

    // for (int i = 1; i < argc; i++) {
    //     if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    //         perror("Conenction failed");
    //         return EXIT_FAILURE;
    //     }

    //     sprintf(buffer, "GET /json/%s?fields=18601979 HTTP/1.1\nHost: ip-api.com\nConnection: keep-alive\n\n", argv[i]);
    //     printf("> %s\n", buffer);
    //     send(sock, buffer, strlen(buffer), 0);
    //     memset(buffer, 0, 2048);

    //     int bytesRead;

    //     while ((bytesRead = recv(sock, buffer, 2048, 0)) != 0) {
    //         buffer[bytesRead] = '\0';
    //     }

    //     printf("< %s\n", buffer);

    //     return 0;
    // }
}