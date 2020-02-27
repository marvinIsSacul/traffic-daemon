#include <cstdlib>
#include <iostream>
#include <thread>
#include <map>

#include "exception/dump.h"
#include "src/model/UserMap.h"
#include "src/net/PacketHandler.h"
#include "src/net/PCap.h"
#include "net/util.h"
#include "net/DefaultDevice.h"



struct Cmd{
    int flags = 0;
    int captureTimeout = 0;
    std::string deviceName = "";
    std::map<int, std::string> addresses;
};



/**
 * Prints the device information.
*/
void printDeviceInfo(pcap_if_t *device);

/**
 * Processes the command-line arguments.
*/
Cmd parseCmd(int argc, char **argv);

/**
 * Prints the program usage information.
*/
void printUsage(const char *programName, std::ostream &out = std::cerr, bool shouldExit = true);


void cleanUpUserMap(){
    model::UserMap um;

    while (true){
        return;
        um.remove();
        Sleep(1);
    }
}


int main(int argc, char **argv)
{
    net::PCap pcap;
    net::PacketHandler ph;
    std::thread bgCleanUp;
    Cmd cmd;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    try{
        cmd = parseCmd(argc, argv);
    }
    catch (const std::exception &e){
        std::cerr << e.what() << std::endl;
        printUsage(argv[0]);
    }

   // printDeviceInfo(device);

   // bgCleanUp = std::thread(cleanUpUserMap);
   // bgCleanUp.detach();

    ph.setFlags(cmd.flags);

    pcap.setPacketHandler(&ph);
    pcap.doCapture(cmd.deviceName);

    curl_global_cleanup();

    return EXIT_SUCCESS;
}

Cmd parseCmd(int argc, char **argv)
{
    Cmd cmd;
    net::DefaultDevice dd;

    if (argc < 2){
        throw std::runtime_error("no arguments specified");
    }

    for (int i = 1; i < argc; ++i){
        if (strcmp(argv[i], "-help") == 0){
            throw std::runtime_error("help required");
        }

        else if (strcmp(argv[i], "-tcp") == 0){
            cmd.flags |= net::AbstractPacketHandler::Protocol::TCP;
        }

        else if (strcmp(argv[i], "-udp") == 0){
            cmd.flags |= net::AbstractPacketHandler::Protocol::UDP;
        }

        else if (strcmp(argv[i], "-timeout") == 0){
            if (argv[i + 1] == nullptr)
                throw std::runtime_error("no capture timeout specified");
            cmd.captureTimeout = atoi(argv[i + 1]);
        }

        else if (strcmp(argv[i], "-device") == 0){
            if (argv[i + 1] == nullptr)
                throw std::runtime_error("no device name specified");
            cmd.deviceName = argv[i + 1];
        }

        else if (strcmp(argv[i], "-list") == 0){
            int i = 0;
            for (auto dn : dd.getDeviceNames()){
                std::cout << (++i) << ". " << dn << std::endl;
            }
            exit(EXIT_SUCCESS);
        }
    }

    if (cmd.flags == 0){
        throw std::runtime_error("no or incorrect protocol specified");
    }

    // if no device was specified
    if (cmd.deviceName.empty()){
        auto device = dd.getDefaultDevice();
        if (device == nullptr){
            throw std::runtime_error("There was no device name specified and there was an error finding a default network device. "
                                     + dd.getError());
        }
        cmd.deviceName = device->name;

        // this can be removed from here.
        printDeviceInfo(device);
    }



    return cmd;
}

void printUsage(const char *programName, std::ostream &out, bool shouldExit){
    out << "usage: ";
    out << programName << " [options...]\n\n";

    out << "available options\n";
    out << "\t-help\t\t Show this help menu.\n";
    out << "\t-list\t\t Show the available devices of this system.\n";
    out << "\t-device <name>\t Name of the device to capture from.\n";
    out << "\t-timeout <milliseconds>\t The number of milliseconds.\n";
    out << "\t-tcp\t\t Capture TCP frames.\n";
    out << "\t-udp\t\t Capture UDP frames.\n";

    if (shouldExit){
        exit(EXIT_FAILURE);
    }
}

void printDeviceInfo(pcap_if_t *device)
{
    printf("using %s\n", pcap_lib_version());
    printf("-> capturing on device: %s (%s)\n", device->name,
           device->description == nullptr ? "no description" : device->description);

    printf("-> loopback: %s\n", (device->flags & PCAP_IF_LOOPBACK) ? "yes" : "no");
   // AF_INET

    printf("\n");

    for (auto a = device->addresses; a; a = a->next){
        std::map<const char*, uint32_t> data;

        data["-> ip address: %s\n"] = ((struct sockaddr_in*)a->addr)->sin_addr.s_addr;
        data["-> mask: %s\n"] = ((struct sockaddr_in*)a->netmask)->sin_addr.s_addr;
        data["-> broadcast: %s\n"] = ((struct sockaddr_in*)a->broadaddr)->sin_addr.s_addr;
        //data["-> gateway: %s\n"] = ((struct sockaddr_in*)a->dstaddr)->sin_addr.s_addr;

        for (auto values : data){
            if (values.second == 0u) continue;
            printf(values.first, net::ipString(values.second).c_str());
        }

        //printf("\n");
    }

    printf("\n");
}
