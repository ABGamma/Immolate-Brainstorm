#define IMMOLATE_API __declspec(dllexport)
#include "functions.hpp"
#include"immolate.hpp"
#include "search.hpp"
#include <cstring>

Item BRAINSTORM_PACK = Item::RETRY;
Item BRAINSTORM_TAG = Item::Charm_Tag;
Item BRAINSTORM_VOUCHER = Item::RETRY;
double BRAINSTORM_SOULS = 1;
bool BRAINSTORM_OBSERVATORY = false;
bool BRAINSTORM_PERKEO = false;

long filter(Instance inst) {
    if (BRAINSTORM_PACK != Item::RETRY) {
        inst.cache.generatedFirstPack = true; //we don't care about Pack 1
        if (inst.nextPack(1) != BRAINSTORM_PACK) {
            return 0;
        }
    }

    if (BRAINSTORM_VOUCHER != Item::RETRY && !BRAINSTORM_OBSERVATORY) {
        if (inst.nextVoucher(1) != BRAINSTORM_VOUCHER) {
            return 0;
        }
    }

    if (BRAINSTORM_TAG != Item::RETRY) {
        if (inst.nextTag(1) != BRAINSTORM_TAG) {
            return 0;
        }
    }
    if (BRAINSTORM_SOULS > 0) {
        for (int i = 1; i <= BRAINSTORM_SOULS; i++) {
            auto tarots = inst.nextArcanaPack(5, 1); //Mega Arcana Pack, assumed from a Charm Tag
            bool found_soul = false;
            for (int t = 0; t < 5; t++) {
                if (tarots[t] == Item::The_Soul) {
                    found_soul = true;
                    break;
                }
            }
            if (!found_soul) {
                return 0;
            }
        }
    }
    if (BRAINSTORM_OBSERVATORY) {
        if (inst.nextVoucher(1) == Item::Telescope) {
            inst.activateVoucher(Item::Telescope);
            if (inst.nextVoucher(2) != Item::Observatory) {
                return 0;
            }
        }
        else return 0;
    }
    if (BRAINSTORM_PERKEO) {
        int antes[5] = { 1, 1, 2, 2, 2 };
        for (int i = 0; i < 5; i++) {
            Pack pack = packInfo(inst.nextPack(antes[i]));
            std::vector<Item> packContents;
            if (pack.type == Item::Arcana_Pack) {
                packContents = inst.nextArcanaPack(pack.size, antes[i]);
            }
            else if (pack.type == Item::Spectral_Pack) {
                packContents = inst.nextSpectralPack(pack.size, antes[i]);
            }
            else continue;
            for (int x = 0; x < pack.size; x++) {
                if (!(packContents[x] == Item::The_Soul && inst.nextJoker(ItemSource::Soul, antes[i], true).joker == Item::Perkeo)) return 0;
            }
        }
    }
    return 1; // Return a score of 1 if all conditions are met
};

std::string brainstorm_cpp(std::string seed, std::string voucher, std::string pack, std::string tag, double souls, bool observatory, bool perkeo) {
    BRAINSTORM_PACK = stringToItem(pack);
    BRAINSTORM_TAG = stringToItem(tag);
    BRAINSTORM_VOUCHER = stringToItem(voucher);
    BRAINSTORM_SOULS = souls;
    BRAINSTORM_OBSERVATORY = observatory;
    BRAINSTORM_PERKEO = perkeo;
    Search search(filter, seed, 1, 100000000);
    search.exitOnFind = true;
    return search.search();
}

extern "C" {
    const char* brainstorm(const char* seed, const char* voucher, const char* pack, const char* tag, double souls, bool observatory, bool perkeo) {
        std::string cpp_seed(seed);
        std::string cpp_pack(pack);
        std::string cpp_voucher(voucher);
        std::string cpp_tag(tag);
        std::string result = brainstorm_cpp(cpp_seed, cpp_voucher, cpp_pack, cpp_tag, souls, observatory, perkeo);

        char* c_result = (char*)malloc(result.length() + 1);
        strcpy(c_result, result.c_str());

        return c_result;
    }

    void free_result(const char* result) {
        free((void*)result);
    }
}