#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <hidapi/hidapi.h>
#include <iostream>
#include "yunzii_anim.h"

#define COUNT       0x05
#define COUNTDOWN   0x08
#define PACKET_SIZE   65

struct UserOptions{
    uint8_t anim = 0;
    uint8_t anim_mode = 0;
    uint8_t r = 0xFF;
    uint8_t g = 0xFF;
    uint8_t b = 0xFF;
    uint8_t brightness = 0x04;
    uint8_t speed = 0x02;
    bool fixed_color = true;
};

struct UserOptions user_options;

hid_device* open_yunzii_rgb_interface()
{
    struct hid_device_info* devs = hid_enumerate(0x3151, 0x4015);
    struct hid_device_info* cur  = devs;
    hid_device* dev = nullptr;

    while(cur)
    {
        if(cur->usage_page == 0xFFFF && cur->usage == 0x0002) // led interface
        {
            dev = hid_open_path(cur->path);
            break;
        }
        cur = cur->next;
    }

    hid_free_enumeration(devs);
    return dev;
}

void send_flush(hid_device* dev) {
    unsigned char packet[PACKET_SIZE] = {0};
    packet[0x01] = 0x0C;
    packet[0x02] = 0x00;
    packet[0x03] = 0x80;
    packet[0x04] = 0x01;
    packet[COUNT] = 0x05;
    packet[COUNTDOWN] = 0x6d;
    send_packet(dev, packet);

    packet[COUNT] = 0x06;
    packet[COUNTDOWN] = 0x6c;
    send_packet(dev, packet);
}

void set_perkey(hid_device* dev)
{

    unsigned char packet[PACKET_SIZE] = {0};
    
    // initialization packet
    packet[0x00] = 0x00;
    packet[0x01] = 0x07;
    packet[0x02] = 0x0d;
    packet[0x03] = 0x05;
    packet[0x04] = 0x04;
    packet[0x05] = 0x00;
    packet[0x06] = 0x00;
    packet[0x07] = 0xc8;
    packet[0x08] = 0xc8;
    packet[0x09] = 0x52;

    send_packet(dev, packet);

    memset(packet, 0, PACKET_SIZE);

    unsigned char rgb[PACKET_SIZE * 5] = {0};

    int i = 0;
    int j = 0;
    while (i < (sizeof(rgb) / sizeof(rgb[0]))) {
        while (j < KEY_COUNT) {
            switch (i) {
                case 30: i += 3;  continue;
                case 33: i += 3;  continue;
                case 87: i += 3;  continue;
                case 105: i += 3; continue;
                case 141: i += 3; continue;
                case 159: i += 3; continue;
                case 222: i += 3; continue;
                case 249: i += 3; continue;
            }

            rgb[i] = keys[j].r;
            rgb[i + 1] = keys[j].g;
            rgb[i + 2] = keys[j].b;
            j++;
            i += 3;
        }
        break;
    }

    for(int pkt = 0; pkt < 5; pkt++)
    {
        unsigned char buf[PACKET_SIZE] = {0};

        buf[0x01] = 0x0C;
        buf[0x02] = 0x00;
        buf[0x03] = 0x80;
        buf[0x04] = 0x01;
        buf[0x05] = pkt;
        buf[0x08] = 0x72 - pkt;

        int data_offset = pkt * 56;
        memcpy(&buf[0x09], &rgb[data_offset], 56);

        send_packet(dev, buf);
    }

    send_flush(dev);

    hid_close(dev);
}

void set_animation(hid_device* dev, struct UserOptions) {
    unsigned char packet[PACKET_SIZE] = {0};
    unsigned char fixed_color_byte = 0x07;
    unsigned char random_color_byte = 0x08;

    packet[0x00] = 0x00;
    packet[0x01] = 0x07;
    packet[0x02] = animations[user_options.anim].anim_b;
    packet[0x03] = user_options.speed;
    packet[0x04] = user_options.brightness;
    if(user_options.fixed_color) {
        switch (user_options.anim_mode) {
            case 1:
                packet[0x05] = MODE_1 + fixed_color_byte;
                break;
            case 2:
                packet[0x05] = MODE_2 + fixed_color_byte;
                break;
            case 3:
                packet[0x05] = MODE_3 + fixed_color_byte;
                break;
            case 4:
                packet[0x05] = MODE_4 + fixed_color_byte;
                break;
            default: packet[0x05] = MODE_1 + fixed_color_byte;
                break;
        }
        packet[0x06] = user_options.r;
        packet[0x07] = user_options.g;
        packet[0x08] = user_options.b;
    } else {
        switch (user_options.anim_mode) {
            case 1:
                packet[0x05] = MODE_1 + random_color_byte;
                break;
            case 2:
                packet[0x05] = MODE_2 + random_color_byte;
                break;
            case 3:
                packet[0x05] = MODE_3 + random_color_byte;
                break;
            case 4:
                packet[0x05] = MODE_4 + random_color_byte;
                break;
            default: packet[0x05] = MODE_1 + fixed_color_byte;
                break;
        }}
    packet[0x09] = get_checksum(packet);

    send_packet(dev, packet);
}

void set_static_color(hid_device* dev, struct UserOptions) {
    unsigned char packet[PACKET_SIZE] = {0};
    packet[0x00] = 0x00;  
    packet[0x01] = 0x07; 
    packet[0x02] = 0x01;  
    packet[0x03] = 0x05;  
    packet[0x04] = user_options.brightness;
    
    if (!user_options.fixed_color) {
        packet[0x05] = 0x08;
    } else {
        packet[0x05] = 0x07;
    }

    packet[0x06] = user_options.r; 
    packet[0x07] = user_options.g; 
    packet[0x08] = user_options.b;  
    packet[0x09] = get_checksum(packet);

    send_packet(dev, packet);

    hid_close(dev);
}

void send_packet(hid_device* dev, unsigned char *data) {
    
    int res = hid_send_feature_report(dev, data, PACKET_SIZE);

    if(res < 0)
        std::cout << "ERROR: " << hid_error(dev);
}

unsigned char get_checksum(unsigned char *data) {

    int checksum;
    unsigned char tmp = 0x00;

    for (unsigned char i = 0x01; i < 0x09; i++) {
        tmp += data[i];
    }

    checksum = (0xFF - tmp) & 0xFF;

    return checksum;
}

int main() {

    unsigned char user_choice = 0;

    hid_device* dev = open_yunzii_rgb_interface();
    if (!dev) {
        std::cout << "Device not found\n";
        exit(-1);
    }

    printf("Animation or static or perkey or testing perkey? (0 or 1 or 2 or 3): ");
    scanf("%hhu", &user_choice);
    getchar();

    if(user_choice == 0) {

        printf("Random or fixed color? (0 or 1): ");
        scanf("%hhu", &user_choice);
        getchar();
        if(user_choice == 0) {
            user_options.fixed_color = false;
        } else {
            printf("Enter red value: ");
            scanf("%hhu", &user_options.r);
            getchar();

            printf("Enter green value: ");
            scanf("%hhu", &user_options.g);
            getchar();

            printf("Enter blue value: ");
            scanf("%hhu", &user_options.b);
            getchar();
        }

        printf("Enter brightness value: ");
        scanf("%hhu", &user_options.brightness);

        for (int i = 0; i < 17; i++) {
            printf("%d: %s\n", i + 1, animations[i].name);
        }
        printf("Enter animation you want: ");
        scanf("%hhu", &user_choice);
        getchar();

        user_options.anim = user_choice - 1;

        if (animations[user_options.anim].mode_count != 0) {
            printf("%s has %d mode's\n", animations[user_options.anim].name, animations[user_options.anim].mode_count);
            printf("Enter animation mode: ");
            scanf("%hhu", &user_options.anim_mode);
            getchar();
        }

        printf("Enter speed of animation: ");
        scanf("%hhu", &user_options.speed);
        getchar();

        set_animation(dev, user_options);
    } else if (user_choice == 1) {

        printf("Random or fixed color? (0 or 1): ");
        scanf("%hhu", &user_choice);
        getchar();

        if (user_choice == 0) {
            user_options.fixed_color = false;
        } else {
            printf("Enter red value: ");
            scanf("%hhu", &user_options.r);
            getchar();

            printf("Enter green value: ");
            scanf("%hhu", &user_options.g);
            getchar();

            printf("Enter blue value: ");
            scanf("%hhu", &user_options.b);
            getchar();
        }

        printf("Enter brightness value: ");
        scanf("%hhu", &user_options.brightness);

        set_static_color(dev, user_options);
    } else if (user_choice == 2) {

        keys[ESC].r = 255;
        keys[ESC].b = 255;
        keys[ESC].g = 255;
        keys[F1].b = 255;
        keys[F1].r = 255;
        keys[F1].g = 255;
        keys[F1].b = 255;
        keys[F2].r = 255;
        keys[F2].g = 255;
        keys[F2].b = 255;
        keys[F3].r = 255;
        keys[F3].g = 255;
        keys[F3].b = 255;
        keys[F4].r = 255;
        keys[F4].g = 255;
        keys[F4].b = 255;
        keys[F5].r = 255;
        keys[F5].g = 255;
        keys[F5].b = 255;
        keys[F6].r = 255;
        keys[F6].g = 255;
        keys[F6].b = 255;
        keys[F7].r = 255;
        keys[F7].g = 255;
        keys[F7].b = 255;
        keys[F8].r = 255;
        keys[F8].g = 255;
        keys[F8].b = 255;
        keys[F9].r = 255;
        keys[F9].g = 255;
        keys[F9].b = 255;
        keys[F10].r = 255;
        keys[F10].g = 255;
        keys[F10].b = 255;
        keys[F11].r = 255;
        keys[F11].g = 255;
        keys[F11].b = 255;
        keys[F12].r = 255;
        keys[F12].g = 255;
        keys[F12].b = 255;
        keys[DEL].r = 255;
        keys[DEL].g = 255;
        keys[DEL].b = 255;

        set_perkey(dev);
    } else if (user_choice == 3) {
        unsigned char rand_color = 0;
        for(int i = 0; i <= 81; i++){
            rand_color = rand() % (255 + 1 - 1) + 1;
            keys[i].r = rand_color;
            rand_color = rand() % (255 + 1 - 1) + 1;
            keys[i].g = rand_color;
            rand_color = rand() % (255 + 1 - 1) + 1;
            keys[i].b = rand_color;
            printf("Key: %s \nR: %d G: %d B: %d \n", keys[i].name, keys[i].r, keys[i].g, keys[i].b);
        }

        set_perkey(dev);
    }

    return 0;
}
