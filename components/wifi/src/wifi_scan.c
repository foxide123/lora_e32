#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "wifi_scan.h"

#define DEFAULT_SCAN_LIST_SIZE 10
//#define DEFAULT_SCAN_LIST_SIZE CONFIG_EXAMPLES_SCAN_LIST_SIZE

static const char *TAG = "scan";

void print_auth_mode(int authmode)
{
    switch(authmode){
        case WIFI_AUTH_OPEN: // No authentication is required.
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OPEN");
            break;
        case WIFI_AUTH_OWE: // Opportunistic Wireless Encryption, A type of open authentication that provides encryption
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OWE");
            break;
        case WIFI_AUTH_WEP: //Wired Equivalent Privacy
        //older and less secure method of wireless encryption and auth
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WEP");
            break;
        case WIFI_AUTH_WPA_PSK: // Wi-Fi Protected Access Pre-Shared Key
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_PSK");
            break;
        case WIFI_AUTH_WPA2_PSK: // Wi-Fi Protected Access 2 Pre-Shared Key
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_PSK");
            break;
        case WIFI_AUTH_WPA_WPA2_PSK: // Mixed mode supporting WPA and WPA2
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_WPA2_PSK");
            break;
        case WIFI_AUTH_ENTERPRISE: //WPA/WPA2 with 802.1X authentication
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_ENTERPRISE");
            break;
        case WIFI_AUTH_WPA3_PSK: //Wi-Fi Protected Access 3 Pre-Shared Key
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_PSK");
            break;
        case WIFI_AUTH_WPA2_WPA3_PSK: // Mixed mode of WPA2 and WPA3
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_WPA3_PSK");
            break;
        case WIFI_AUTH_WPA3_ENT_192: //WPA3 tailored for enterprise networks, using 192-bit cryptographic strength and 802.1X authentication
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_ENT_192");
            break;
        default:
            ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_UNKNOWN");
            break;
    }
}


void print_cipher_type(int pairwise_cipher, int group_cipher)
{
    switch (pairwise_cipher) {
    case WIFI_CIPHER_TYPE_NONE: // no encryption
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40: // WEP encryption with a 40-bit key
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP: // encryption protocol used in WPA
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP: // encryption mode used in WPA2
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP: // mixed mode supporting TKIP and CCMP
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    case WIFI_CIPHER_TYPE_AES_CMAC128: //Cipher-based Message Authentication Code with AES and 128-bit key
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_AES_CMAC128");
        break;
    case WIFI_CIPHER_TYPE_SMS4: //An encryption standard used in Chinese WLAN systems
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_SMS4");
        break; 
    case WIFI_CIPHER_TYPE_GCMP: //An encryption protocol providing both confidentiality and data integrity
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_GCMP");
        break;
    case WIFI_CIPHER_TYPE_GCMP256: // GCMP with 256-bit key for enhanced security
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_GCMP256");
        break;
    default:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }

    switch (group_cipher) {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    case WIFI_CIPHER_TYPE_SMS4:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_SMS4");
        break;
    case WIFI_CIPHER_TYPE_GCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_GCMP");
        break;
    case WIFI_CIPHER_TYPE_GCMP256:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_GCMP256");
        break;
    default:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }
}

/* Initialize Wi-Fi as sta (station mode) and set scan method */
void wifi_scan(void)
{
    ESP_ERROR_CHECK(esp_netif_init()); // initializes TCP/IP stack
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta(); // creates default network interface for Wi-Fi station mode
    assert(sta_netif); // ensures network interface was creatd successfully

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE]; // array that will hold the records of discovered APs
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info)); //initialize all elements of an array of structures to zero.

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    esp_wifi_scan_start(NULL, true);
    ESP_LOGI(TAG, "Max AP number ap_info can hold = %u", number);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info)); //retrieves the scan results
    ESP_LOGI(TAG, "Total APs scanned = %u, actual AP number ap_info holds = %u", ap_count, number);

    for (int i = 0; i < number; i++) { // iterates over the discovered AP records
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
        print_auth_mode(ap_info[i].authmode);
        if (ap_info[i].authmode != WIFI_AUTH_WEP) {
            print_cipher_type(ap_info[i].pairwise_cipher, ap_info[i].group_cipher);
        }
        ESP_LOGI(TAG, "Channel \t\t%d", ap_info[i].primary);
    }

}